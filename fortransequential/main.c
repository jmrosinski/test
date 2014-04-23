#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h>    // read
#include <stdarg.h>    // va_* routines
#include <stdlib.h>    // exit

#include <stdio.h>

extern void err_exit (const char *fmt, ...);

int main (int argc, char **argv)
{
  typedef union {
    int ival[2];
    float fval[2];
    double dval;
  } IntFloatDouble;

  IntFloatDouble ifd;
    
  const int rcwlen = 4;
  int rcw;    // record control word
  int rcw2;   // end of record rcw
  int fd;
  int recnum = 0; // record number
  int ret;

  if (argc != 2)
    err_exit ("Usage: %s filename\n", argv[0]);

  if ((fd = open (argv[1], O_RDONLY)) < 0)
    err_exit ("File %s cannot be opened in read-only mode\n", argv[1]);

  printf ("Successfully opened file %s\n", argv[1]);

  while (1) {
    if ((ret = read (fd, &rcw, rcwlen)) == 0) {
      printf ("End of file reached\n");
      exit (0);
    } else if (ret < 0) {
      err_exit ("Error reading\n");
    }

    // Print 1st and last values of record if it contains at least 8 bytes
    if (rcw >= 16) {
      if ((ret = read (fd, &ifd, 8)) > 0) {
	printf ("1st val of record %d as int=%d float=%g double=%g\n", recnum+1, ifd.ival[0], ifd.fval[0], ifd.dval);
      }

      if (lseek (fd, rcw-16, SEEK_CUR) < 0)  // Move to last 8 bytes of record
	err_exit ("Error in lseek: maybe not a Fortran seqential file?\n");

      if ((ret = read (fd, &ifd, 8)) > 0) {
	printf ("last val of record %d as int=%d float=%g double=%g\n", recnum+1, ifd.ival[0], ifd.fval[0], ifd.dval);
      }
    } else {
      if (lseek (fd, rcw, SEEK_CUR) < 0)  // Just move to where rcw2 is
	err_exit ("Error in lseek: maybe not a Fortran seqential file?\n");
    }

    if ((ret = read (fd, &rcw2, rcwlen)) == 0) {
      err_exit ("End of file reached\n");
    } else if (ret < 0) {
      err_exit ("Error reading\n");
    }

    if (rcw != rcw2)
      err_exit ("Record %d: rcw=%d does not match rcw2=%d\n", recnum, rcw, rcw2);

    recnum++;
    printf ("Record %d is %d bytes long\n", recnum, rcw);
  }
}

void err_exit (const char *fmt, ...)
{
  va_list args;
  
  va_start (args, fmt);
  
  if (fmt != NULL)
    (void) vfprintf (stderr, fmt, args);
  
  va_end (args);
  exit (-1);
}
