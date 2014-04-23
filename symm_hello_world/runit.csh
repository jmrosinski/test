#!/bin/csh                                                                      
setenv MIC_PPN 1
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

ibrun.symm -c "./a.out" $ibarg "./mic/a.out"
