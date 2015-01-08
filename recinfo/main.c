#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv)
{
  char *fn;    // file name
  int fd;      // file descriptor
  int rcw;     // record control word
  int endrcw;  // ending record control word (contents must match rcw)
  int n;       // record number

  if (argc < 2) {
    printf ("Usage: %s input_file\n", argv[0]);
    return 1;
  }

  fn = argv[1];
  if ((fd = open (fn, O_RDONLY)) == -1) {
    printf ("Error opening file=%s\n", fn);
    return 1;
  }

  // Test is on successfully reading 4 byte rcw
  // Action is to jump ahead rcw bytes plus the size of a rcw (4)
  n = 1;
  while (read (fd, &rcw, 4) == 4) {
    printf ("Record %d is %d bytes long (%d words)\n", n, rcw, rcw/4);
    lseek (fd, (off_t) rcw, SEEK_CUR);  // move ahead rcw bytes
    if (read (fd, &endrcw, 4) != 4) {
      printf ("Premature error attempting ending rcw for record=%d\n",n);
      return 1;
    }
    if (rcw != endrcw) {
      printf ("Record=%d start rcw=%d end rcw=%d don't match!\n", n, rcw, endrcw);
      return 1;
    }
    ++n;
  }
  printf ("Successfully read all records\n");
}
