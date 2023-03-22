make -C src
make[1]: Entering directory '/home/rosinski/testgit/emulate_gptl_cuda/src'
nvc -c -g f_wrappers.c
nvfortran -c -g gptlf.F90
ar cr libgptl.a f_wrappers.o gptlf.o
make[1]: Leaving directory '/home/rosinski/testgit/emulate_gptl_cuda/src'
make -C tests
make[1]: Entering directory '/home/rosinski/testgit/emulate_gptl_cuda/tests'
nvc -c -g cputest.c
nvc -o cputest cputest.o -L../src -lgptl
make[1]: Leaving directory '/home/rosinski/testgit/emulate_gptl_cuda/tests'
make -C cuda
make[1]: Entering directory '/home/rosinski/testgit/emulate_gptl_cuda/cuda'
make -C src
make[2]: Entering directory '/home/rosinski/testgit/emulate_gptl_cuda/cuda/src'
nvcc -c -gencode arch=compute_61,code=compute_61 -g -G -lineinfo -src-in-ptx -ftz=true -rdc=true gptl_device.cu
nvcc warning : '--device-debug (-G)' overrides '--generate-line-info (-lineinfo)'
nvfortran -c -g -O0 -acc gpu -Minfo=accel -m64 gptl_acc.F90
gptl_acc.F90:
ar cr libgptl_cuda.a gptl_device.o gptl_acc.o
make[2]: Leaving directory '/home/rosinski/testgit/emulate_gptl_cuda/cuda/src'
make -C tests
make[2]: Entering directory '/home/rosinski/testgit/emulate_gptl_cuda/cuda/tests'
nvcc -c -gencode arch=compute_61,code=compute_61 -g -G -lineinfo -src-in-ptx -ftz=true -rdc=true basic.cu
nvcc warning : '--device-debug (-G)' overrides '--generate-line-info (-lineinfo)'
basic.cu(9): warning: variable "ret" was declared but never referenced

basic.cu(20): warning: variable "ret" was set but never used

nvcc -o basic -gencode arch=compute_61,code=compute_61 -g -G -lineinfo -src-in-ptx -ftz=true -rdc=true basic.o -L../../src -lgptl -L../src -lgptl_cuda
nvcc warning : '--device-debug (-G)' overrides '--generate-line-info (-lineinfo)'
ptxas warning : Conflicting options --device-debug and --generate-line-info specified, ignoring --generate-line-info option
ptxas warning : Conflicting options --device-debug and --generate-line-info specified, ignoring --generate-line-info option
make[2]: Leaving directory '/home/rosinski/testgit/emulate_gptl_cuda/cuda/tests'
make[1]: Leaving directory '/home/rosinski/testgit/emulate_gptl_cuda/cuda'
make -C cacctests
make[1]: Entering directory '/home/rosinski/testgit/emulate_gptl_cuda/cacctests'
cacctests placeholder
make[1]: Leaving directory '/home/rosinski/testgit/emulate_gptl_cuda/cacctests'
make -C facctests
make[1]: Entering directory '/home/rosinski/testgit/emulate_gptl_cuda/facctests'
nvfortran -c -o basic.o -I../src -I../cuda/src -g -O0 -acc gpu -Minfo=accel -m64 basic.F90
basic.F90:
basic:
     10, Generating Tesla code
         11, !$acc loop gang ! blockidx%x
nvfortran -o basic -g -O0 -acc gpu -Minfo=accel -m64 -Mcuda=cc61 -lstdc++ basic.o -L../src -lgptl -L../cuda/src -lgptl_cuda
ptxas warning : Conflicting options --device-debug and --generate-line-info specified, ignoring --generate-line-info option
nvlink error   : Undefined reference to 'GPTLstop_gpu' in 'basic.o'
nvlink error   : Undefined reference to 'GPTLstart_gpu' in 'basic.o'
pgacclnk: child process exit status 2: /opt/nvidia/hpc_sdk/Linux_x86_64/21.7/compilers/bin/tools/nvdd
make[1]: *** [Makefile:7: basic] Error 2
make[1]: Leaving directory '/home/rosinski/testgit/emulate_gptl_cuda/facctests'
make: *** [Makefile:9: facctests] Error 2
