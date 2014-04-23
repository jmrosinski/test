#!/bin/csh
setenv MIC_PPN 1
# This one is useful after doing:
# srun --pty -p development -t 01:00:00 -n 2 -N 2 /bin/tcsh -l
#ibrun.symm -c ./a.out -m ./mic/a.out

# This one is useful after doing:
# srun --pty -p normal-2mic -t 01:00:00 -n 2 -N 2 /bin/tcsh -l
if ($?SLURM_QUEUE) then
  set queue = $SLURM_QUEUE
else
  set queue = unknown
endif

if ($queue == "normal-2mic") then
  set ibarg = "-2"
# srun -n2 -N2 means total host tasks=2, nodes=2
  @ total_host_tasks = $SLURM_NTASKS
# The hardwired 2 in next line is for 2 MICs per node
# MIC_MY_NSLOTS is the total number of ranks on MICs
  @ mic_my_nslots = $MIC_PPN * 2 * $total_host_tasks
  setenv MIC_MY_NSLOTS $mic_my_nslots
else
  set ibarg = "-m"
endif

# normal-2mic
#foreach skip ( 1 3 4 6 )
# normal-mic or development
foreach skip ( 1 2 4 )
  foreach oneatatime ( 0 1 )
    set append = s_${skip}.o_${oneatatime}.queue_$queue
    echo "Running with -s $skip -o $oneatatime ..." |& tee out.whatsrunning
#JR Here is where the equivalent of "mpiexec" is run
    ibrun.symm -c "./a.out -s $skip -o $oneatatime" $ibarg "./mic/a.out -s $skip -o $oneatatime" |& tee out.$append
    mv timing.summary timing.summary.$append
    foreach i ( timing.[0-9] timing.[0-9][0-9] )
      mv $i ${i}.$append
    end
  end
end
exit 0
