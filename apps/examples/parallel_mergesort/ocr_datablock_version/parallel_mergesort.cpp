#include "sequential_mergesort.h"
#include "parallel_mergesort.h"
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "ocr.h"
#include <sys/time.h>

long int start_time; /* Make this a global rather than passing it
			around like a crazy person */

int main(int argc, char ** argv){

  int array_size;
  extern long int start_time;

  int chunk_size;

  /* Init OCR */
  OCR_INIT(&argc, argv, splitter, merger);

  if (argc != 3){
    std::cerr << "Usage: " << argv[0] << " <array_size> <sequential_chunk_size>, where sequential_chunk_size is the number of elements at which the algorithm should switch from parallel recursion to just sequentially solving the chunk\n";
      }
  else {
    array_size = atoi(argv[1]);
    chunk_size = atoi(argv[2]);    
   }

  if(array_size <= chunk_size){
    /* be lazy and flag an error */
    std::cerr << "array_size must be larger than chunk_size\n";
    exit(0);
  }

  std::cout << "array size: " << array_size << " chunk size: " << chunk_size << " run time (microseconds): ";

  int *sort_array, *temp_array, i;

  /* Create the arary to be sorted and populate it with random values */
  sort_array = new int[array_size];
  temp_array = new int[array_size];
  for (i = 0; i < array_size; i++){
    //    sort_array[i] = rand() % 1000;
    sort_array[i] = array_size - i;  //input array starts exactly backwards 
    // to eliminate time spent in rand() for hotspot analysis.
  }

  /* Getting into the actual sorter, so compute the start time */
  struct timeval st;
  gettimeofday(&st, NULL);
  start_time = (st.tv_sec * 1000000) + st.tv_usec; 

  /* Create a placeholder event to pass into the top-level splitter
  that pretends to be the event that will fire that splitter's parent
  merger */

  ocrGuid_t placeholder_event;
  ocrEventCreate(&(placeholder_event), OCR_EVENT_STICKY_T, false);


  //  std::cout << "at start: start_time = " << start_time << " and merger_event = " << placeholder_event << "\n";
  /* Create and start a splitter task for the whole array */
  ocrGuid_t splitterInputGuid, splitterResultGuid;
  intptr_t *splitterInputDb, *splitterResultDb;
   
  if(int res = ocrDbCreate(&splitterInputGuid, (void **) &splitterInputDb, 7*sizeof(intptr_t), 0, NULL, NO_ALLOC)){
    std::cout << "DbCreate #1 failed\n";
  }
  /*  else{
    std::cout << "DbCreate #1 allocated db for guid " << splitterInputGuid << "\n";
    }*/

  splitterInputDb[0]= (intptr_t) sort_array;
  splitterInputDb[1]= (intptr_t) temp_array;
  splitterInputDb[2]= (intptr_t) 0;
  splitterInputDb[3]= (intptr_t) array_size-1;
  splitterInputDb[4]= (intptr_t) array_size;
  splitterInputDb[5]= (intptr_t) chunk_size;
  splitterInputDb[6]= (intptr_t) placeholder_event;  
  
  ocrGuid_t splitter_guid, splitter_event, splitter_event2;
  /* create the EDT for the worker */
  ocrEdtCreate(&(splitter_guid), splitter, 0, NULL, NULL, 0, 1, NULL);
  
  /* create the event for passing the inputs */
  ocrEventCreate(&(splitter_event), OCR_EVENT_STICKY_T, true);
   
  /* Link the input events to splitter */
  ocrAddDependency(splitter_event, splitter_guid, 0);

  /* Schedule the splitter */
  ocrEdtSchedule(splitter_guid);
  
  /* and satisfy the input events to start the splitter */
  ocrEventSatisfy(splitter_event, splitterInputGuid);  

  ocrCleanup();
      return 0;
}

u8 splitter(u32 paramc, u64 * params, void *paramv[], u32 depc, ocrEdtDep_t depv[]){
  /* Splitter is the recursive-subdivision part of mergesort.  Given a
     region of the array to be sorted, if the region is larger than
     CHUNK_SIZE, it splits the region into two equal or near-equal
     pieces and creates a splitter task for each region, as well as a
     merger task to merge the sub-regions once they've been sorted.

     Splitter takes eight arguments: a pointer to the array to be
     sorted, a pointer to an equally-large array for temporary values
     (not used directly, but passed to the merger that splitter creates),
     the start index of the region that splitter is responsible for, the
     end index of the region that splitter is responsible for, the
     size of the array being sorted, the
     chunk size (the number of elements where the algorithm should
     switch from recursive descent to a sequential sort), the time in
     microseconds when the sort started and the
     dependency that should be satisfied to trigger the merger above
     the splitter (This is NULL for the top-level splitter). The array
     size and the start time are not used internally by splitter, but
     are passed to its children so that the run time can be computed
     at the end and so that the output array can be checked for
     correctness.

     It has one dependency: an input dependency that passes the
     arguments to splitter.
  */

  intptr_t* typed_paramv = (intptr_t*) depv[0].ptr;
  int *sort_array = (int *) typed_paramv[0];
  int *temp_array = (int *) typed_paramv[1];
  int start_index = (int) typed_paramv[2];
  int end_index = (int) typed_paramv[3];
  int array_size = (int) typed_paramv[4];
  int chunk_size = (int) typed_paramv[5];
  ocrGuid_t merger_event = (ocrGuid_t) typed_paramv[6];

  //  std::cout << "Splitter called on indices " << start_index << " to " << end_index << " with array_size " << array_size << " chunk_size " << chunk_size << " start_time " << start_time << " and merger_event " << merger_event << "\n";

  if((end_index - start_index) < chunk_size){
    /* Our piece of the array is small enough that we should just sort
       it sequentially */
    // std::cout << "Splitter doing a sequential sort of indices " << start_index << " to " << end_index << "\n";
    
    sequential_mergesort(sort_array, temp_array, start_index, end_index);
    /* Create the array to hold the sorted sub-array */
    ocrGuid_t result_array_guid;
    int *result_array;
    if(int res = ocrDbCreate(&result_array_guid, (void **) &result_array, ((end_index - start_index) +1) *sizeof(int), 0, NULL, NO_ALLOC)){
      std::cout << "DBcreate #2 failed with result " << res << "\n";
    }
    /*  else{
    std::cout << "DbCreate #2 allocated db for guid " <<
    result_array_guid << "\n"; 
    }*/
    int sort_position = start_index;
    /* Copy the sorted sub-array into our result datablock */
    for(int i = 0; i < (end_index - start_index) + 1; i++){
      result_array[i] = sort_array[sort_position];
      sort_position++;
    }

    ocrEventSatisfy(merger_event, result_array_guid);

    //   ocrDbRelease(result_array_guid);
  }
  else{
    /* Do the recursive split into two pieces */
    int new_chunk_size = ((end_index + 1) - start_index)/2; 
    /* +1 because of zero-indexed arrays */

    int start_index_1 = start_index;
    int end_index_1 = start_index + (new_chunk_size -1);
    int start_index_2 = start_index + new_chunk_size;
    int end_index_2 = end_index;

    //    std::cout << "Splitter creating two sub-chunks, one from " << start_index_1 << " to " << end_index_1 << " and the other from " << start_index_2 << " to " << end_index_2 << "\n";

    /* First, create the merger task so that we have all of the events
       to pass to the splitters */
   
    /* Create the events that the splitters (actually the mergers that
       the splitters will create) will satisfy to trigger the merger */
    ocrGuid_t left_splitter_done_event_guid, right_splitter_done_event_guid, merger_guid, mergerInputReadyGuid, mergerOutputArrayReadyGuid;
    ocrEventCreate(&(left_splitter_done_event_guid), OCR_EVENT_STICKY_T, true);
    ocrEventCreate(&(right_splitter_done_event_guid), OCR_EVENT_STICKY_T, true);

      /* Create the merger task */
    ocrEdtCreate(&(merger_guid), merger, (end_index -start_index) + 1, (u64 *) (u64) array_size, (void **) merger_event, 0, 2, NULL);

    /* Add the splitter events as input dependencies */
    ocrAddDependency(left_splitter_done_event_guid, merger_guid, 0);
    ocrAddDependency(right_splitter_done_event_guid, merger_guid, 1);

    /* Schedule the merger */
    ocrEdtSchedule(merger_guid);

    /* create and fill out the parameter lists for the new splitter
       tasks */
    ocrGuid_t leftSplitterInputGuid, rightSplitterInputGuid;
    intptr_t *leftSplitterInputDb, *rightSplitterInputDb;
 
    if(int res =ocrDbCreate(&leftSplitterInputGuid, (void **) &leftSplitterInputDb, 7*sizeof(intptr_t), 0, NULL, NO_ALLOC)){
      std::cout << "DBcreate #4 failed with result " << res << "\n";
    }
    /*  else{
    std::cout << "DbCreate #4 allocated db for guid " << leftSplitterInputGuid << "\n";
    }*/
    if(int res =    ocrDbCreate(&rightSplitterInputGuid, (void **) &rightSplitterInputDb, 7*sizeof(intptr_t), 0, NULL, NO_ALLOC)){
      std::cout << "DBcreate #5 failed with result " << res << "\n";
    } 
    /*    else{
      std::cout << "DbCreate #5 allocated db for guid " << rightSplitterInputGuid << "\n";
      } */
    leftSplitterInputDb[0]= (intptr_t) sort_array;
    leftSplitterInputDb[1]= (intptr_t) temp_array;
    leftSplitterInputDb[2]= (intptr_t) start_index_1;
    leftSplitterInputDb[3]= (intptr_t) end_index_1;
    leftSplitterInputDb[4]= (intptr_t) array_size;  
    leftSplitterInputDb[5]= (intptr_t) chunk_size;  
    leftSplitterInputDb[6]= (intptr_t) left_splitter_done_event_guid;  

    rightSplitterInputDb[0]= (intptr_t) sort_array;
    rightSplitterInputDb[1]= (intptr_t) temp_array;
    rightSplitterInputDb[2]= (intptr_t) start_index_2;
    rightSplitterInputDb[3]= (intptr_t) end_index_2;
    rightSplitterInputDb[4]= (intptr_t) array_size;  
    rightSplitterInputDb[5]= (intptr_t) chunk_size;  
    rightSplitterInputDb[6]= (intptr_t) right_splitter_done_event_guid;  

    /* now, create the two splitter tasks */
    ocrGuid_t left_splitter_guid, right_splitter_guid;
    ocrGuid_t left_event_guid, right_event_guid;
    
    /* create the EDTs for the splitters */
    ocrEdtCreate(&(left_splitter_guid), splitter, 0, NULL, NULL, 0, 1, NULL);
    ocrEdtCreate(&(right_splitter_guid), splitter, 0, NULL, NULL, 0, 1, NULL);
    
    /* create the input events for the splitters */
    ocrEventCreate(&(left_event_guid), OCR_EVENT_STICKY_T, true);
    ocrEventCreate(&(right_event_guid), OCR_EVENT_STICKY_T, true);

  
    /* and link the events to the EDTs */
    ocrAddDependency(left_event_guid, left_splitter_guid, 0);
    ocrAddDependency(right_event_guid, right_splitter_guid, 0);

    /* schedule the splitters */
    ocrEdtSchedule(left_splitter_guid);
    ocrEdtSchedule(right_splitter_guid);

    /* and satisfy the input events to start them */
    ocrEventSatisfy(left_event_guid, leftSplitterInputGuid);
    ocrEventSatisfy(right_event_guid, rightSplitterInputGuid);
   }

  if(int res=ocrDbDestroy(depv[0].guid)){  /* Free the space used by our inputs */
    std::cout << "Splitter ocrDbDestroy #1 failed with code " << res << "\n";
  }
  /*  else{
    std::cout << "Splitter ocrDbDestroy #1 destroyed guid " << depv[0].guid << "\n";
    }*/
  return 0;  /* We're done */

}

u8 merger(u32 paramc, u64 * params, void *paramv[], u32 depc, ocrEdtDep_t depv[]){
  /* Merger is the routine that merges two sorted sub-arrays into one
     sorted array.  The sub-arrays are expected to be adjacent regions
     of the array being sorted.  Merger takes seven inputs: a pointer
     to the array being sorted, a pointer to an equally-sized array
     for holding temporary values, the start index of the region to be
     merged (low index of the first sub-array), the end index of
     the region to be merged (high index of the second sub-array), the
     total length of the array being sorted, the start time of the
     sort, and
     the event that merger should satisfy when it is done.

     Merger has two input dependencies from its children, and one that
     contains its inputs from the task that created it*/

  /* For now, just do the dependency stuff so the program structure
     works */


  intptr_t* typed_paramv = (intptr_t *) depv[2].ptr;
  int merge_length = paramc;;
  u64 array_size = (u64) params;
  ocrGuid_t output_event = (ocrGuid_t) paramv;
  
  //std::cout << "Merger called on range " << start_index << " to " << end_index << " with start_time " << start_time << " and array_size " << array_size << "\n";

  int *left_subarray = (int *) depv[0].ptr;
  int *right_subarray = (int *) depv[1].ptr;
  
  /* allocate the space for the merged array */
  ocrGuid_t outputArrayGuid;
  int *output_array;
  if(int res =ocrDbCreate(&outputArrayGuid, (void **) &output_array, merge_length*sizeof(int), 0, NULL, NO_ALLOC)){
      std::cout << "DBcreate #6 failed with result " << res << "\n";
    }
  /*  else{
    std::cout << "DbCreate #6 allocated db for guid " << outputArrayGuid << "\n";
    }*/
  /* Merge the sub-arrays */

  
  int left_index = 0;
  int right_index = 0;
  int merged_index = 0;

  while((left_index < merge_length/2) && (right_index < merge_length/2)){
    if(left_subarray[left_index] < right_subarray[right_index]){
      output_array[merged_index] = left_subarray[left_index];
      left_index++;
    }
    else{
      output_array[merged_index] = right_subarray[right_index];
      right_index++;
    }
    merged_index++;
  }

  // ok, we've reached the end of at least one of the sub-arrays, so finish up
  while(left_index < merge_length/2){
    output_array[merged_index] = left_subarray[left_index];
    left_index++;
    merged_index++;
  }
  while(right_index < merge_length/2){
    output_array[merged_index] = right_subarray[right_index];
    right_index++;
    merged_index++;
  }

  //test loop
  /*  for(int q = 1; q < merge_length; q++){
    if (output_array[q-1] > output_array[q]){
      std::cout << "Mis-sorted value found at position " << q << " with start_index = " << start_index << " and end_index " << end_index << "\n";
    }
    }*/
  if(merge_length == array_size){
    /* I'm the top-level merger, and the program is done when I'm finished */
    /* Compute run time (not counting result check */
    extern long int start_time;
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    long int end_microseconds = (end_time.tv_sec * 1000000) + end_time.tv_usec;
    /* std::cout << "start time " << start_time << "\n";
    std::cout << "end time   " << end_microseconds << "\n";
    std::cout << "Core run time " << (end_microseconds - start_time) << " microseconds\n"; */
    std::cout << (end_microseconds - start_time) << "\n";

    /* check the result, then exit */
    int bad_sort = 0;
     for(int i = 1; i < array_size; i++){
  
       if(output_array[i-1] > output_array[i]){
	std::cout << "Mis-sorted value found at positions " << i-1 << " and " << i << "\n";
	bad_sort = 1;
      }
      }   //comment out result check for vtune analysis
    if (bad_sort == 0){
      std::cout << "Sorted array checks out\n";
    }

    /*   for(int i = 0; i < array_size; i++){
      std::cout << sort_array[i] << " ";
    }
    std::cout << "\n";
    */
    ocrFinish();  /* Tell OCR we're done */
  }
  ocrEventSatisfy(output_event, outputArrayGuid);
  if(int res = ocrDbDestroy(depv[1].guid)){  /* Free the space used by our inputs */
    std::cout << "Merger ocrDbDestroy #2 failed with code " << res << "\n";
  }
  /*  else{
    std::cout << "Merger ocrDbDestroy #2 destroyed guid " << depv[1].guid << "\n";
    }*/
  if(int res = ocrDbDestroy(depv[0].guid)){  /* Free the space used by our inputs */
    std::cout << "Merger ocrDbDestroy #3 failed with code " << res << "\n";
  }
  /*  else{
    std::cout << "Merger ocrDbDestroy #1 destroyed guid " << depv[0].guid << "\n";
    }*/
  return(0);
}

