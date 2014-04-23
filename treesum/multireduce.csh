#!/bin/csh -f
\rm  gpfile.jr_reduce gpfile.MPI_Reduce
touch gpfile.jr_reduce
touch gpfile.MPI_Reduce

@ limit = 256
@ ntasks = 2
while ($ntasks < $limit)
  echo "testing on $ntasks tasks"
  mpiexec -n $ntasks ./jr_reduce >&! stdout

  set time = `grep 'Time for jr_reduce=' stdout | cut -c 20-`
  echo $ntasks $time >> gpfile.jr_reduce

  set time = `grep 'Time for MPI_Reduce=' stdout | cut -c 21-`
  echo $ntasks $time >> gpfile.MPI_Reduce
  @ ntasks++
end
