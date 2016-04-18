#!/bin/bash
export GASNET_NODEFILE=machine.file
. /etc/profile.d/modules.sh
module load gcc/5.2.0
OPTIONS="$2 $3"
PROF_OPTIONS="-cat legion_prof -level 2 -hl:prof 3 -logfile prof_%.log" 
PROF_OPTIONS=" -level 4,legion_prof=2 -logfile prof_%.log -hl:prof 2" 
#PROF_OPTIONS=
LL_OPTIONS="-ll:cpu 2  -ll:csize 65000 -ll:gsize 1975" 

export PATH=$HOME/GASNETNoMPI/bin/:$PATH
gasnetrun_mxm -n 2 -N 2 -E PATH,LD_LIBRARY_PATH -spawner=ssh $1 $OPTIONS $LL_OPTIONS $PROF_OPTIONS
