#ifdef __cplusplus
extern "C" {
#endif

#pragma acc routine seq
int GPTLstart_gpu (int);
#pragma acc routine seq
int GPTLstop_gpu (int);

#ifdef __cplusplus
}
#endif
