#!/bin/csh
set mpiruncmd = "mpiexec -np 2"
set cmd = "$mpiruncmd ENVVAR=GOTIT ./a.out"
echo "Testing via $cmd ..."
$cmd
if ($status == 0) then
  echo Worked
else
  echo Failed
endif

set cmd = "$mpiruncmd -env ENVVAR GOTIT ./a.out"
echo "Testing via $cmd ..."
$cmd
if ($status == 0) then
  echo Worked
else
  echo Failed
endif

set cmd = "$mpiruncmd ./a.out"
echo "via setenv ENVVAR GOTIT; $cmd"
setenv ENVVAR GOTIT
$cmd
if ($status == 0) then
  echo Worked
else
  echo Failed
endif
