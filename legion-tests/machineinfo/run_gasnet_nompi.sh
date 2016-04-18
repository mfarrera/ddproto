#!/bin/bash
export GASNET_NODEFILE=machine.file
. /etc/profile.d/modules.sh
module load gcc/5.2.0
#PROF_OPTIONS=" -level 4,legion_prof=2 -logfile prof_%.log -hl:prof 3" 
PROF_OPTIONS=
LL_OPTIONS="-ll:cpu 16  -ll:csize 65000 -ll:gsize 1975" 
#LL_OPTIONS=""
export PATH=$HOME/GASNETNoMPI/bin/:$PATH
export GASNET_BACKTRACE=1
gasnetrun_mxm -n 3 -N 3 -E PATH,LD_LIBRARY_PATH -spawner=ssh $1 $LL_OPTIONS $PROF_OPTIONS
