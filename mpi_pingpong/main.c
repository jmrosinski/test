/*
** Program to time sending and receiving of large (1 MB) MPI messages.
** Arguments: 
**   -s <skip value>: Adjusts pairing of sender and receiver. Default is 1
**      which means ranks 0 and 1 exchange data, 2 and 3, etc. -s 2 means
**      rank 0 exchanges with 2, 1 with 3, etc.
**   -o: Messages are exchanged between pairs with nothing else going on.
**       Default is messages are exchanged simultaneously, i.e 0 is exchanging
**       with 1 while 2 is exchanging with 3, etc.
**
** Jim Rosinski 2/7/2014
** NOAA/ESRL
*/              

#include <unistd.h>  /* getopt, usleep */
#include <stdlib.h>  /* atoi */
#include <mpi.h>
#include <gptl.h>
#include <string.h>  /* memset */
#include <stdio.h>

#ifdef __GNUC__
#define onek 1024
#define onem (onek * onek)
#define onemb ((onek * onek) / sizeof(int))
#else
static const int onek = 1024;
static const int onem = onek * onek;
static const int onemb = (onek * onek) / sizeof(int);
#endif

static const float oneg = (float) onem * onek;
static int sendarr[onemb];
static int recvarr[onemb];
static char host[MPI_MAX_PROCESSOR_NAME];
static char otherhost[MPI_MAX_PROCESSOR_NAME];
static FILE *fp;

void setname (const char *, char *);
void latency (int, int);
void pingpong_sendrecv (int, int);
void pingpong_IsendIrecv (int, int);
void pingpong_IrecvIsend (int, int);
void pingpong_Ssend (int, int);
void verify (int);

int main (int argc, char **argv)
{
  int c;
  int skip = 1;            /* interval between pairs of MPI ranks */
  int oneatatime = 0;      /* whether to execute pairs one at a time */
  int n;
  int ret;
  int iam;
  int nranks;
  int other;               /* MPI partner */
  int doit;                /* our turn this iteration (when oneatatime=1) */
  int *done;               /* flag array indicates our turn done */
  int *partner;
  int resultlen;
  MPI_Status status;
  char *queue;             /* batch queue name we're running in */
  char outfile[64];        /* file to write to */

  setlinebuf (stdout);

  ret = GPTLsetutr (GPTLnanotime);
  ret = GPTLinitialize ();

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &nranks);
  if (nranks % 2 != 0) {
    printf ("nranks must be even. Got %d\n", nranks);
    ret = MPI_Abort (MPI_COMM_WORLD, -1);
  }
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  ret = MPI_Get_processor_name (host, &resultlen);

  partner = (int *) malloc (nranks * sizeof (int));
  done = (int *) malloc (nranks * sizeof (int));
  for (n = 0; n < nranks; ++n) {
    partner[n] = -1;  /* uninitialized */
  }

  while ((c = getopt (argc, argv, "o:s:")) != -1) {
    switch (c) {
    case 'o':
      oneatatime = atoi (optarg);
      break;
    case 's':
      skip = atoi (optarg);
      if (nranks % skip != 0) {
	printf ("skip value=%d must divide evenly into nranks=%d\n", skip, nranks);
	ret = MPI_Abort (MPI_COMM_WORLD, -1);
      }
      if ((nranks / skip) % 2 != 0) {
	printf ("nranks=%d skip=%d does not satisfy mod(nranks/skip,2)==0\n", skip, nranks);
	ret = MPI_Abort (MPI_COMM_WORLD, -1);
      }
      break;
    default:
      printf ("unknown option %c\n", c);
      return 1;
      break;
    }
  }

  // Discover partner
  for (n = 0; n < nranks; ++n) {
    other = (n + skip) % nranks;
    if (partner[n] < 0 && partner[other] < 0) {
      partner[n] = other;
      partner[other] = n;
    }
  }
  if (partner[iam] == -1) {
    printf ("partner[iam=%d] == -1: Sitting out this dance\n", iam);
  }
  other = partner[iam];

  printf ("iam=%d total=%d on host=%s\n", iam, nranks, host);
  ret = MPI_Barrier (MPI_COMM_WORLD);  // Prevent mixing with next print

  printf ("iam=%d skip=%d oneatatime=%d\n", iam, skip, oneatatime);
  ret = MPI_Barrier (MPI_COMM_WORLD);  // Prevent mixing with next print

  // Set sendarr contents
  for (n = 0; n < onemb; ++n)
    sendarr[n] = n + iam;

  printf ("iam=%d other=%d\n", iam, other);
  ret = MPI_Barrier (MPI_COMM_WORLD);  // Prevent mixing with next print

  if (other >= 0) {
    if (iam < other) {
      ret = MPI_Recv (otherhost, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, other, 999, MPI_COMM_WORLD, &status);
      ret = MPI_Send (host,      MPI_MAX_PROCESSOR_NAME, MPI_CHAR, other, 999, MPI_COMM_WORLD);
    } else {
      ret = MPI_Send (host,      MPI_MAX_PROCESSOR_NAME, MPI_CHAR, other, 999, MPI_COMM_WORLD);
      ret = MPI_Recv (otherhost, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, other, 999, MPI_COMM_WORLD, &status);
    }
  }
  
  // Gather timings using different MPI primitives
  if (oneatatime) {
    for (n = 0; n < nranks; ++n)
      done[n] = 0;      /* our iteration is not complete */
    for (n = 0; n < nranks; ++n) {
      doit = ! done[n] && partner[iam] >= 0 && (n == iam || partner[n] == iam);
      if (doit) {
	done[n] = 1;
	done[partner[n]] = 1;
      }
      ret = MPI_Barrier (MPI_COMM_WORLD);
      if (doit)
	latency (iam, other);
      
      ret = MPI_Barrier (MPI_COMM_WORLD);
      if (doit)
	pingpong_sendrecv (iam, other);
      
      ret = MPI_Barrier (MPI_COMM_WORLD);
      if (doit)
	pingpong_IsendIrecv (iam, other);
      
      ret = MPI_Barrier (MPI_COMM_WORLD);
      if (doit)
	pingpong_IrecvIsend (iam, other);
      
      ret = MPI_Barrier (MPI_COMM_WORLD);
      if (doit)
	pingpong_Ssend (iam, other);
    }
  } else {
    ret = MPI_Barrier (MPI_COMM_WORLD);
    latency (iam, other);

    ret = MPI_Barrier (MPI_COMM_WORLD);
    pingpong_sendrecv (iam, other);

    ret = MPI_Barrier (MPI_COMM_WORLD);
    pingpong_IsendIrecv (iam, other);

    ret = MPI_Barrier (MPI_COMM_WORLD);
    pingpong_IrecvIsend (iam, other);

    ret = MPI_Barrier (MPI_COMM_WORLD);
    pingpong_Ssend (iam, other);
  }

  ret = GPTLpr (iam);
  ret = GPTLpr_summary (MPI_COMM_WORLD);
  ret = MPI_Finalize ();
  return 0;
}

void latency (int iam, int other)
{
  int ret;
  int n;
  double val;
  MPI_Status status;
  char name[32];
  int handle;       // for gptl

  // Determine timer name based on where iam and other live
  setname ("latency", name);

  // Initialize gptl handle
  ret = GPTLinit_handle (name, &handle);

  for (n = 0; n < 1000000; ++n) {
    if (iam < other) {
      // Only time the guy who receives after sending
      ret = GPTLstart_handle (name, &handle);
      ret = MPI_Send (sendarr, 1, MPI_INT, other, 999, MPI_COMM_WORLD);
      ret = MPI_Recv (recvarr, 1, MPI_INT, other, 999, MPI_COMM_WORLD, &status);
      ret = GPTLstop_handle (name, &handle);
    } else {
      ret = MPI_Recv (recvarr, 1, MPI_INT, other, 999, MPI_COMM_WORLD, &status);
      ret = MPI_Send (sendarr, 1, MPI_INT, other, 999, MPI_COMM_WORLD);
    }
  }

  if (iam < other) {
    ret = GPTLget_wallclock (name, 0, &val);
    val = 0.5 * val;
    printf ("iam=%d host=%s latency to rank %d host=%s=%g usec\n", 
	    iam, host, other, otherhost, val);
  }
}

void pingpong_sendrecv (int iam, int other)
{
  int ret;
  int n;
  double bw;
  MPI_Status status;
  char name[32];
  int handle;       // for gptl

  // Determine timer name based on where iam and other live
  setname ("sendrecv", name);

  // Initialize gptl handle
  ret = GPTLinit_handle (name, &handle);

  for (n = 0; n < onek; ++n) {
    if (iam < other) {
      // Only time the guy who recvs after sending
      ret = GPTLstart_handle (name, &handle);
      ret = MPI_Send (sendarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD);
      ret = MPI_Recv (recvarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &status);
      ret = GPTLstop_handle (name, &handle);
    } else {
      ret = MPI_Recv (recvarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &status);
      ret = MPI_Send (sendarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD);
    }
  }
  verify (other);

  if (iam < other) {
    ret = GPTLget_wallclock (name, 0, &bw);
    bw = 2. / bw;
    printf ("iam=%d host=%s %s BW to rank %d host=%s=%g GB/s\n", 
	    iam, host, name, other, otherhost, bw);
  }
}

void pingpong_IsendIrecv (int iam, int other)
{
  int ret;
  int n;
  double bw;
  MPI_Status status;
  MPI_Request srequest, rrequest;
  char name[32];
  int handle;       // for gptl

  // Determine timer name based on where iam and other live
  setname ("IsendIrecv", name);

  // Initialize gptl handle
  ret = GPTLinit_handle (name, &handle);

  for (n = 0; n < onek; ++n) {
    ret = GPTLstart_handle (name, &handle);
    ret = MPI_Isend (sendarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &srequest);
    ret = MPI_Irecv (recvarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &rrequest);
    ret = MPI_Wait (&srequest, &status);
    ret = MPI_Wait (&rrequest, &status);
    ret = GPTLstop_handle (name, &handle);
  }
  verify (other);

  if (iam < other) {
    ret = GPTLget_wallclock (name, 0, &bw);
    bw = 2. / bw;
    printf ("iam=%d host=%s %s BW to rank %d host=%s=%g GB/s\n", 
	    iam, host, name, other, otherhost, bw);
  }
}

void pingpong_IrecvIsend (int iam, int other)
{
  int ret;
  int n;
  double bw;
  MPI_Status status;
  MPI_Request srequest, rrequest;
  char name[32];
  int handle;       // for gptl

  // Determine timer name based on where iam and other live
  setname ("IrecvIsend", name);

  // Initialize gptl handle
  ret = GPTLinit_handle (name, &handle);

  ret = GPTLstart_handle (name, &handle);
  for (n = 0; n < onek; ++n) {
    ret = MPI_Irecv (recvarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &rrequest);

    // Synchronize with partner indicating both have posted recv
    if (iam < other)
      ret = MPI_Ssend (sendarr, 0, MPI_INT, other, 111, MPI_COMM_WORLD);
    else
      ret = MPI_Recv (recvarr, 0, MPI_INT, other, 111, MPI_COMM_WORLD, &status);

    ret = MPI_Isend (sendarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &srequest);
    ret = MPI_Wait (&rrequest, &status);
    ret = MPI_Wait (&srequest, &status);
  }
  ret = GPTLstop_handle (name, &handle);
  verify (other);

  if (iam < other) {
    ret = GPTLget_wallclock (name, 0, &bw);
    bw = 2. / bw;
    printf ("iam=%d host=%s %s BW to rank %d host=%s=%g GB/s\n", 
	    iam, host, name, other, otherhost, bw);
  }
}

void pingpong_Ssend (int iam, int other)
{
  int ret;
  int n;
  double bw;
  MPI_Status status;
  char name[32];
  int handle;       // for gptl

  // Determine timer name based on where iam and other live
  setname ("Ssend", name);

  // Initialize gptl handle
  ret = GPTLinit_handle (name, &handle);

  for (n = 0; n < onek; ++n) {
    if (iam < other) {
      // Only time the guy who recvs after sending
      ret = GPTLstart_handle (name, &handle);
      ret = MPI_Ssend (sendarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD);
      ret = MPI_Recv (recvarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &status);
      ret = GPTLstop_handle (name, &handle);
    } else {
      ret = MPI_Recv (recvarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD, &status);
      ret = MPI_Ssend (sendarr, onemb, MPI_INT, other, 999, MPI_COMM_WORLD);
    }
  }
  verify (other);

  if (iam < other) {
    ret = GPTLget_wallclock (name, 0, &bw);
    bw = 2. / bw;
    printf ("iam=%d host=%s %s BW to rank %d host=%s=%g GB/s\n", 
	    iam, host, name, other, otherhost, bw);
  }
}

void verify (int other)
{
  int n;
  int ret;

  for (n = 0; n < onemb; ++n) {
    if (recvarr[n] != n + other) {
      printf ("Bad recvarr[%d]=%d expected %d\n", n, recvarr[n], n+other);
      ret = MPI_Abort (MPI_COMM_WORLD, -1);
    }
  }
}

void setname (const char *base, char *name)
{
  strcpy (name, base);
  if (strcmp (host, otherhost) == 0) {
    // On-node
    if (strstr (host, "-mic") == NULL) {
      strcat (name, "_hostself");
    }  else {
      strcat (name, "_micself");
    }
  } else {
    // Off-node
    if (strstr (host, "-mic") == NULL) {
      // host is NOT a mic
      if (strstr (otherhost, "-mic") == NULL) {
	strcat (name, "_hosthost");
      } else {
	if (strncmp (host, otherhost, 8) == 0) {
	  strcat (name, "_hostmic_samenode");
	} else {
	  strcat (name, "_hostmic_diffnodes");
	}
      }
    }  else {
      // host IS a mic
      if (strstr (otherhost, "-mic") == NULL) {
	if (strncmp (host, otherhost, 8) == 0) {
	  strcat (name, "_hostmic_samenode");
	} else {
	  strcat (name, "_hostmic_diffnodes");
	}
      } else {
	// other host is a mic
	if (strncmp (host, otherhost, 8) == 0) {
	  strcat (name, "_micmic_samenode");
	} else {
	  strcat (name, "_micmic_diffnodes");
	}
      }
    }
  }
}
