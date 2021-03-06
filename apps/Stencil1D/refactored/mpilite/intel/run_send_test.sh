#!/bin/bash
set -x
[ $# -ne 2 ] && { echo "Usage: $0 NX NITER "; exit 1; }
make -f Makefile.x86 TARGET=stencil_send clean install
echo "Testing NX = $1, NITER = $2"
make -f Makefile.x86 TARGET=stencil_send WORKLOAD_ARGS="-r 4 -t 0 $1 $2" run 2>&1 |  tee log_send.$1
#./stencil_send.exe -r 4 $1 $2  2>&1 | tee log_send.$1

