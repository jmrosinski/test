#!/bin/csh
echo "" >! outfile

foreach n (1 2 4 8 16 32 64 128)
  echo copying data to flush disk cache...
  cp zzz.nc test.nc
  echo running on $n tasks...
  mpiexec -n $n ~/test/nim_netcdf/readnc
  set time = `grep get_var timing.summary | cut -c 44-53`
  echo $n $time
  echo $n $time >> outfile
end
exit 0

