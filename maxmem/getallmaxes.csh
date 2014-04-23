#!/bin/csh

limit
# discover max heap size
make heap
#./heap

# discover max stack size (tests are in MB)
foreach stacksize (1 10 100 200 400 800 1600 3200 6400 12800 25600)
  sed -e "s/\#NMB/${stacksize}/g" stack.ftemplate >! stack.f90 || echo 'sed failure' && exit 1
  make stack || echo 'make failure' && exit 1
  ./stack
  if ($status != 0) then
    set maxstack = $stacksize
    break
  endif
end

# discover max thread stack size (tests are in MB)
foreach threadstacksize (1 10 100 200 400 800 1600 3200 6400 12800 25600)
  sed -e "s/\#NMB/${threadstacksize}/g" threadstack.ftemplate >! threadstack.f90 || echo 'sed failure' && exit 1
  make threadstack || echo 'make failure' && exit 1
  env OMP_NUM_THREADS=16 ./threadstack
  if ($status != 0) then
    set maxthreadstack = $threadstacksize
    break
  endif
end

echo "max stack that worked = ", $maxstack
echo "max thread stack that worked = ", $maxthreadstack
exit 0
