#!/usr/bin/env python


import getopt, logging, os, re, shutil, sys, time
from datetime import datetime
from operator import itemgetter

# Update the search path for external dependences
sys.path.append('./external')

from junit_xml import TestSuite, TestCase
from RECSortedList import RECSortedList

# Import other objects
from JobObject import JobObject, LocalJobObject, JobTypeObject


# Set up the logger
logging.getLogger().setLevel(logging.DEBUG)
streamHandler = logging.StreamHandler()
streamHandler.setLevel(logging.INFO)
streamFormatter = logging.Formatter('%(levelname)-8s %(message)s')
streamHandler.setFormatter(streamFormatter)
logging.getLogger().addHandler(streamHandler)

# Global variables used to keep track of jobs
"""Initial directory fetched from
GIT. This is the reference directory that
contains the entire checked-out tree (absolute path)
"""
cleanDirectory = None

""""Root" directory that is shared (with LUSTRE)
across the various nodes. All tests will have their
directories created here if needed (absolute path)
"""
sharedRoot = None

""""Root" directory that is private the machine (faster).
The cleanDirectory is a sub-directory of this directory and
all tests will have their directories created here as well
if needed (absolute path)
"""
privateRoot = None

"""Number of seconds the script will sleep before
checking for more jobs to run
"""
sleepInterval = None

"""All jobs that are left to run.
key: Name of the job
value: JobObject
"""
allRemainingJobs = dict()

"""All job types that can be used.
key: Name of the job type
value: JobTypeObject
"""
allJobTypes = dict()

"""Contains a list of all jobs keyed by the length of time
they have been running. Each element of the list is a JobObject of
a running job
"""
allRunningJobs = []

"""Contains the list of all jobs that are ready to run
ordered by their depth (priority). Contains a pair where the
first element is the priority of the ready job and the second
element is the JobObject
"""
allReadyJobs = RECSortedList(key=itemgetter(0))

"""Number of jobs blocked"""
countBlockedJobs = 0

"""Jobs that are "terminal" (ie: no waiters). The
results of the jobs in this list will be printed out"""
allTerminalJobs = []

"""All running jobs by paths. The dictionary contains:
    - Key: The path where the job is running
    - Value: A pair: 
        - 1st Element is True if one of the jobs is not OK with sharing
        - 2nd Element is a count of jobs running in this directory
"""
allUsedPaths = dict()

# Check whether a dictionary has the proper format
def checkDict(inputDict, typeStr, fieldTypes, mandatoryFields):
    """Checks whether input matches a certain format. This
    checks to see if the fields are of the proper type
    and if mandatory fields are present once and only once"""
   
    myLog = logging.getLogger();
    countPresence = [0] * len(mandatoryFields)
    if type(inputDict) != type(dict()):
        myLog.error("%s is not a dictionary", typeStr)
    for k, v in inputDict.items():
        t = fieldTypes.get(k, None)
        if t is not None:
            if type(t) != type(v):
                myLog.error("%s has invalid type for key '%s'" % (typeStr, k))
                return False
            if k in mandatoryFields:
                countPresence[mandatoryFields.index(k)] += 1
        else:
            myLog.error("%s has invalid key '%s'" % (typeStr, k))
            return False
    for i in range(0, len(countPresence)):
        if countPresence[i] == 0:
            myLog.error("%s is missing key '%s'" % (typeStr, mandatoryFields[i]))
            return False
        if countPresence[i] > 1:
            myLog.error("%s has multiple values for key '%s'" % (typeStr, mandatoryFields[i]))
            return False
    return True

# Check whether a job has the proper format
def checkJob(inputDict):
    checkStruct = {'name': "", 'depends': (),
                   'jobtype': "", 'run-args': "",
                   'param-args': "",
                   'sandbox': (), 'timeout': int(0)}
    checkPresence = ('name', 'depends', 'jobtype',
                     'run-args')
    return checkDict(inputDict, "JobObject", checkStruct, checkPresence)

# Check whether a job type has the proper format
def checkJobType(inputDict):
    checkStruct = {'name': "", 'keywords': (), 'isLocal': False,
                   'run-cmd': "", 'param-cmd': "",
                   'sandbox': (), 'timeout': int(0)}
    checkPresence = ('name', 'keywords', 'isLocal', 'run-cmd', 'param-cmd',
                     'sandbox')
    return checkDict(inputDict, "JobObject type", checkStruct, checkPresence)
    
# Print usage
class Usage:
    def __init__(self, msg):
        self.msg = msg

# Main loop: loops that continuously looks for
# tasks to run and collects their results. It will return
# when there are no tasks that can be run
def mainLoop():
    global cleanDirectory, sharedRoot, privateRoot, sleepInterval
    global allRemainingJobs, allJobTypes, allRunningJobs, allReadyJobs, allTerminalJobs
    global allUsedPaths
    global countBlockedJobs

    myLog = logging.getLogger()
    while len(allRemainingJobs) > 0 or len(allReadyJobs) or len(allRunningJobs):
        myLog.debug("Looping with %d remaining jobs" % (len(allRemainingJobs)))
        myLog.debug("Have %d blocked jobs and %d ready jobs" % 
                    (countBlockedJobs, len(allReadyJobs)))
        needWholeMachine = False;
        # Go over the running jobs and see if they
        # are done
        tempAllRunning = []
        for job in allRunningJobs:
            if not job.poll():
                tempAllRunning.append(job)
                if job.getIsWholeMachine():
                    needWholeMachine = True
            else:
                if job.getIsTerminalJob():
                    allTerminalJobs.append(job)
        allRunningJobs = tempAllRunning
    
        # We now go over the jobs that are in allRemainingJobs
        # and distribute them to the other lists as appropriate
        keysToRemove = []
        for k, v in allRemainingJobs.iteritems():
            if v.getStatus() == JobObject.UNCONFIGURED_JOB:
                pass
            elif v.getStatus() == JobObject.WAITING_JOB:
                pass
            elif v.getStatus() == JobObject.READY_JOB:
                myLog.info("%s is now ready to run" % (v))
                keysToRemove.append(k)
                allReadyJobs.add((v.getDepth(), v))
            elif v.getStatus() == JobObject.BLOCKED_JOB:
                myLog.warning("%s is now blocked -- it will never run." % (v))
                keysToRemove.append(k)
                countBlockedJobs += 1
                if v.getIsTerminalJob():
                    allTerminalJobs.append(v)
        # Now update the allRemainingJobs
        for k in keysToRemove:
            del allRemainingJobs[k]

        # We now have all the jobs in allReadyJobs and
        # we can try to execute them
        # TODO: This is ugly. Is there a better way??!?!
        effectiveEnd = len(allReadyJobs) - 1
        for i in range(0, len(allReadyJobs)):
            if i > effectiveEnd:
                break
            if needWholeMachine:
                break # We can't run any more jobs right now
            myLog.debug("Trying to run %s" % (str(allReadyJobs[i][1])))
            if allReadyJobs[i][1].execute() > JobObject.READY_JOB:
                myLog.info("Starting %s" % (allReadyJobs[i][1]))
                allRunningJobs.append(allReadyJobs[i][1])
                if allReadyJobs[i][1].getIsWholeMachine():
                    myLog.debug("%s requires the entire machine, not launching others" % (str(allReadyJobs[i][1])))
                    needWholeMachine = True
                allReadyJobs.pop(i)
                effectiveEnd -= 1
            else:
                myLog.debug("%s did not start... will retry later")
    
        # At this stage, we have launched all jobs that
        # could be launched. We will sleep before resuming the
        # loop
        time.sleep(sleepInterval)
    # End of while loop on allRemainingJobs
    
# Main function
def main(argv=None):
    global cleanDirectory, sharedRoot, privateRoot, sleepInterval
    global allRemainingJobs, allJobTypes, allRunningJobs, allReadyJobs, allTerminalJobs
    global allUsedPaths
    global streamHandler
    global countBlockedJobs
    testKeywords = []

    myLog = logging.getLogger()

    if argv is None:
        argv = sys.argv

    myLog.info("---- Starting ----")
    tempJobs = dict() # Contains the jobs until we have parsed all the job types
    resultFileName = None
    try:
        try:
            opts, args = getopt.getopt(argv[1:], "hc:i:s:p:t:k:o:fd", ["help", "config=", "initdir=", "shared=",
                                                                       "private=", "time=", "keyword=", "output=",
                                                                       "full-help", "debug"])
        except getopt.error, err:
            raise Usage(err)
        for o, a in opts:
            if o in ("-h", "--help"):
                raise Usage(\
"""
    -h,--help:      Prints this message
    -f,--full-help: Prints a detailed message on the format of jobs and job types
    -d,--debug:     Enable debugging
    -c,--config:    Test files to import (without .py extension). The files should contain
                    the description of the tests to run. Can be specified multiple times
    -i,--initdir:   Initial checked-out directory containing the entire GIT tree
    -s,--shared:    Root directory to use for "shared" workspaces (shared by LUSTRE)
    -p,--private:   Root directory to use for "private" (local) workspaces
    -t,--time:      Time to wait before each iteration of the job scheduling algorithm
    -k,--keyword:   (optional) Keywords to use to select the test to run. If specified multiple times,
                    only the tests that match ALL the keywords will be run.
    -o,--output:    Filename to produce containing the results of the tests
""")
            elif o in ("-f", '--full-help'):
                raise Usage(\
"""
    Each job type must specify the following arguments:
        - name:        (string) Name of the job type (must be unique)
        - keywords:    (tuple of strings) Used to restrict the tests run. If the user specifies
                       keywords when launching the test harness, only those job types that match the
                       specified keywords will be run
        - isLocal:     (bool) If True, the job will run on the local machine. If False, it will be
                       farmed out by Torque
        - run-cmd:     (string) Command to launch the job.
        - param-cmd:   (string) Command to respond to parameter queries for jobs. The following
                       arguments will be passed:
                           - for all jobs:
                               - output: Returns the absolute path of a XML output file with JUnit
                                         formatted result or the empty string if no such file is
                                         generated (in which case the job itself is the testcase)
                           - for non-local jobs:
                               - resources: Returns the formatted string of resource requirements
                                            for Torque
        - sandbox:     (tuple of strings) See below for an explanation of the sandbox parameters
    It can also optionally specify:
        - timeout:     (int) Number of seconds before a job is killed. If non specified or 0,
                       the job will never timeout

    Each job must specify the following arguments:
        - name:        (string) Name of the job (must be unique)
        - depends:     (tuple of strings): Name of other jobs on which this job depends
        - jobtype:     (string) Name of the job type of this job
        - run-args:    (string) Arguments for this job (will be concatenated with 'run-cmd'
                       from its job type to actually run the job
    It can optionally specify:
        - param-args:  (string) Arguments for this job to get parameter information for this
                       job. Will be concatenated with 'param-cmd' from its job type AFTER
                       the type of parameter required.
        - sandbox:     (tuple of strings) See below for an explanation of the sandbox parameters.
                       This complements the job type's sandbox information for inheritX information
                       and over-rides all other parameters.
        - timeout:     (int) Number of seconds before this job is killed. It overrides anything
                       specified by the job type

    Explanation of the parameters of the 'sandbox' argument:
        - The 'sandbox' argument determines the directories this job will be given to run in
          and whether or not it "inherits" data from some of its parent jobs. The valid strings are:
            - inheritX:     Where X is an integer from 0 to the number of dependences minus 1. This specifies
                            which working directory from a parent this job should inherit. For example, jobs
                            that run regression tests may want to inherit from the job that built the executables.
                            If this option is not specified, the job will get a copy of the initial check-out. This
                            option can only be specified in the job's sandbox description.
            - createRoot:   Specifies whether this job should be given a new directory (a copy from whomever it
                            inherits from). Note that this parameter has no impact if inheritX is not specified
                            as the job is always given a copy when inheriting from the initial check-out.
            - noCreateRoot: Can only be specified in a job's sandbox list to over-ride a 'createRoot' in a job type's
                            sandbox list.
            - shareOK:      If inheriting from a parent, specifies whether it is OK to run other jobs in that
                            same directory.
            - noShareOK:    Can only be specified in a job's sandbox list to over-ride a 'shareOK' in the job type's
                            sandbox list.
            - single:       Specifies whether the job requires the entire machine to run. This only applies to
                            local jobs as the requirements for remote jobs are returned through param-cmd.
            - noSingle:     Can only be specified in a job's sandbox list to over-ride a 'single' in the job type's
                            sandbox list.
            - local:        Specifies whether this job requires a local home directory (non-shared over LUTRE).
                            If specified before 'shared', the initial directory for the job will be the
                            local one. The environment variable JJOB_PRIVATE_HOME will be set to this
                            directory as well as JJOB_START_HOME if this is the starting directory.
            - noLocal:      Can only be specified in a job's sandbox list to over-ride a 'local' in the job type's
                            sandbox list.
            - shared:       Specifies whether this job requires a shared home directory (shared over LUSTRE).
                            Same comments as for 'local'. Sets JJOB_SHARED_HOME.
            - noShared:     Like 'noLocal' but for 'shared'
""")
            elif o in ("-d", "--debug"):
                streamHandler.setLevel(logging.DEBUG)
            elif o in ("-c", "--config"):
                try:
                    (importPath, name) = os.path.split(a)
                    myLog.debug("Importing file in '%s' called '%s'" % (importPath, name))
                    sys.path.append(importPath)
                    tFile = __import__(name)
                    for k,v in tFile.__dict__.items():
                        if k.startswith('job_'):
                            # This is a job so we insert it in the allJobs dictionary
                            try:
                                if tempJobs.has_key(v['name']):
                                    raise Usage("Duplicate job name '%s'" % (v['name']))
                                myLog.info("Checking job '%s' for correctness" % (v['name']))
                                if checkJob(v):
                                    tempJobs[v['name']] = v
                                else:
                                    raise Usage("JobObject '%s' is incorectly formated" % (v['name']))
                            except KeyError:
                                raise Usage("JobObject '%s' in file '%s' does not define a name" %
                                            (k, a))
                        elif k.startswith('jobtype_'):
                            # This is a job type so we insert it in the allJobTypes dictionary
                            try:
                                if allJobTypes.has_key(v['name']):
                                    raise Usage("Duplicate job type name '%s'" % (v['name']))
                                myLog.info("Checking job type '%s' for correctness" % (v['name']))
                                if checkJobType(v):
                                    allJobTypes[v['name']] = JobTypeObject(v)
                                else:
                                    raise Usage("JobObject type '%s' is incorectly formated" % (v['name']))
                            except KeyError:
                                raise Usage("JobObject type '%s' in file '%s' does not define a name" %
                                            (k, a))
                except ImportError:
                    raise Usage("File '%s' could not be imported" % (a))
            elif o in ("-i", "--initdir"):
                cleanDirectory = a
            elif o in ("-s", "--shared"):
                sharedRoot = a
            elif o in ("-p", "--private"):
                privateRoot = a
            elif o in ("-t", "--time"):
                sleepInterval = int(a)
            elif o in ("-k", "--keyword"):
                testKeywords.append(a)
            elif o in ("-o", "--output"):
                resultFileName = a
            else:
                raise Usage("Unhandled option")
        if args is not None and len(args) > 0:
            raise Usage("Extraneous arguments")
    except Usage, msg:
        print >> sys.stderr, msg.msg
        print >> sys.stderr, "For help, use -h"
        return 2

    # Set the globals
    JobObject.setGlobals(cleanDirectory=cleanDirectory, sharedRoot=sharedRoot,
                         privateRoot=privateRoot, allUsedPaths=allUsedPaths)

    try:
        # At this point we have all the job types and we can do other checks
        # and create the jobs
        # TODO: We need to check keywords around here
        for k, v in tempJobs.iteritems():
            jobType = allJobTypes.get(v['jobtype'])
            if jobType is None:
                raise Usage("JobObject '%s' uses a job type '%s' which is not defined" % (k, v['jobtype']))
            if jobType.isLocal:
                allRemainingJobs[k] = LocalJobObject(v, jobType, len(v['depends']))
            else:
                raise Usage("Unsupported job type (for now)")
                
        # Do another loop to set up the dependences properly
        for k, v in tempJobs.iteritems():
            if len(v['depends']):
                waitingJob = allRemainingJobs[k]
                for dep in v['depends']:
                    signalingJob = allRemainingJobs[dep]
                    slot = waitingJob.addDependence(signalingJob)
                    signalingJob.addWaiter(waitingJob, slot)
        # Got all dependence information
        # Check if we missed some dependence
        toRemoveKeys = []
        for k, v in allRemainingJobs.iteritems():
            if v.getStatus() == JobObject.UNCONFIGURED_JOB:
                myLog.warning("%s has missing dependences" % (str(v)))
                v.signalJobDone(-2)
                toRemoveKeys.append(k)
        for k in toRemoveKeys:
            del allRemainingJobs[k]

        # Now check if we have everything we need
        # TODO: Add permission checks maybe
        if cleanDirectory is None:
            raise Usage("Missing checked-out directory")
        if privateRoot is None:
            raise Usage("Missing private root directory")
        if sharedRoot is None:
            raise Usage("Missing shared root directory")
        if sleepInterval is None:
            myLog.warning("Sleep interval not specified... assuming 5 seconds")
            sleepInterval = 5
        if resultFileName is None:
            myLog.warning("No output filename specified, results will be dumped to stdout")
    except Usage, msg:
        print >> sys.stderr, msg.msg
        print >> sys.stderr, "For help, use -h"
        return 2

    myLog.info("---- Initializing ---")
    # Build the initial list of ready jobs
    totalJobCount = len(allRemainingJobs)
    toRemoveKeys = []
    for k, v in allRemainingJobs.iteritems():
        if v.getStatus() == JobObject.READY_JOB:
            myLog.info("%s is initially ready to run" % (str(v)))
            allReadyJobs.add((v.getDepth(), v))
            toRemoveKeys.append(k)
    # Remove things from allRemainingJobs
    for k in toRemoveKeys:
        del allRemainingJobs[k]

    if len(allReadyJobs) == 0:
        myLog.error("No jobs are initially ready, nothing to execute")
        return 0 # This may not be an "error" as such if no jobs match the keywords
    
    startTime = datetime.now()
    myLog.info("---- Starting jobs (have %d total jobs) @ %s ----" %
               (totalJobCount, str(startTime)))
    mainLoop()
    endTime = datetime.now()
    myLog.info("---- Finished jobs (took %d seconds, %d terminal jobs; %d could not run), now cleaning up ----" %
               ((endTime - startTime).seconds, len(allTerminalJobs), countBlockedJobs))
    
    # Output results
    allTestSuites = [job.getTestSuite() for job in allTerminalJobs]
    if resultFileName:
        resultFile = open(resultFileName, "w")
        TestSuite.to_file(resultFile, allTestSuites)
    else:
        TestSuite.to_file(sys.stdout, allTestSuites)

    # Remove directories (do this now because some of the result files
    # may be in the directories)
    for k, v in allUsedPaths.iteritems():
        assert(not v[0])
        assert(v[1] == 0)
        myLog.debug("Removing work directory '%s'" % (k))
        shutil.rmtree(k)

    myLog.info("---- Done ----")

# Starts the program
if __name__ == '__main__':
    sys.exit(main())

