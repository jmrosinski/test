#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static int cpn = 4;   /* cores per node (default 4) */
static int nwt = 0;   /* number of write tasks (default none) */
static int mwtpn = 4; /* max write tasks per node (default cpn) */
static int root_own_node = 1; /* whether root is on a node by itself (default false) */

int main (int argc, char **argv)
{
  char name[MPI_MAX_PROCESSOR_NAME];
  char **names;
  char *namebuf;
  int c;              /* walk thru arg list */
  int i, ii, n;       /* indices */
  int ret;            /* return code */
  int resultlen;      /* length of name (returned by MPI_GET_PROCESSOR_NAME */
  int coresleft;      /* number of cores remaining on node after write tasks assigned */
  int nproc;          /* number of MPI tasks in WORLD */
  int *proclist;      /* list of MPI ranks */
  int iam;            /* rank in MPI_COMM_WORLD */
  int iamnew;         /* rank in comm */

  MPI_Group MPI_GROUP_WORLD; // world
  MPI_Group group = -1;      // group changes size
  MPI_Comm comm = -1;        // communicator 

  extern char *optarg;

  setlinebuf (stdout);

  /* Parse argument list */

  while ((c = getopt (argc, argv, "c:m:w:z")) != -1) {
    switch (c) {
    case 'c':
      if ((cpn = atoi (optarg)) < 1) {
	printf ("-c (cores per node) must be > 0\n");
	return 1;
      }
      break;
    case 'm':
      if ((mwtpn = atoi (optarg)) < 0) {
	printf ("-m (max write tasks per node) must be non-negative\n");
	return 1;
      }
      break;
    case 'w':
      if ((nwt = atoi (optarg)) < 0) {
	printf ("-w (number of write tasks) must be non-negative\n");
	return 1;
      }
      break;
    case 'z':
      root_own_node = atoi (optarg);
      break;
    default:
      printf ("unknown option %c\n", c);
      return 1;
    }
  }

  if (mwtpn > cpn) {
    printf ("mwtpn = %d must not exceed cpn = %d\n", mwtpn, cpn);
    return 1;
  }
  
  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &nproc);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  ret = MPI_Get_processor_name (name, &resultlen);
  printf ("MPI_COMM_WORLD rank %d is running on %s\n", iam, name);

  /* This would be easier in Fortran */

  namebuf = (char *) malloc (nproc * MPI_MAX_PROCESSOR_NAME * sizeof (char));
  names = (char **) malloc (nproc * sizeof (char *));
  for (i = 0; i < nproc; ++i)
    names[i] = &namebuf[i*MPI_MAX_PROCESSOR_NAME];

  (void) strncpy (names[iam], name, MPI_MAX_PROCESSOR_NAME);
  ret = MPI_Gather (name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 
		    names, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 
		    0, MPI_COMM_WORLD);

  if (iam == 0) {
    printf ("Running %s -c%d -m%d -w%d -z%d\n", 
	    argv[0], cpn, mwtpn, nwt, root_own_node);
  }

  if ((proclist = (int *) malloc (nproc * sizeof (int))) == 0)
    MPI_Abort (MPI_COMM_WORLD, 1);
  
  /* Define proclist for new communicator, and set contents of 1st node */

  i = ii = 0;
  proclist[ii] = i; /* Root task always same in both communictors */
  if (root_own_node) {
    i = cpn;        // Move pointer to 1st core of next node
    ii = 1;
    if (iam == 0 && strcmp (names[0], names[i]) == 0) {
	printf ("WARNING: Node name[%d]=%s matches name[%d]=%s\n",
		0, names[0], i, names[i]);
	printf ("cpn specified improperly, or non-standar MPI task distribution?\n");
      }
  } else {          // Fill node 0 with compute tasks
    for (i = 1; i < cpn; (++i, ++ii)) {
      proclist[ii] = i;
      if (iam == 0 && strcmp (names[i-1], names[i]) != 0) {
	printf ("WARNING: Node name[%d]=%s does not match name[%d]=%s\n",
		i-1, names[i-1], i, names[i]);
	printf ("cpn specified improperly, or non-standar MPI task distribution?\n");
      }
    }
  }
  
  /* Add write tasks to proclist */

  for (n = 0; n < nwt; ++n) {
    if (i >= nproc) {
      if (iam == 0) {
	printf ("Ran out of MPI tasks at n=%d i=%d\n", n, i);
      }
      MPI_Finalize ();
      exit (1);
    }
    proclist[ii] = i;
    if ((n+1) % mwtpn == 0)  // Move pointer to start of next node
      i += cpn - mwtpn;
    else                     // Increment pointer
      ++i;

    ++ii;
  }

  /* Compute tasks start at next empty node */
  
  coresleft = i % cpn;
  i += coresleft;

  /* Remainder of proclist is compute tasks */

  while (i < nproc) {
    proclist[ii] = i;
    ++i; ++ii;
  }

  /* Define the new communicator */

  ret = MPI_Comm_group (MPI_COMM_WORLD, &MPI_GROUP_WORLD);      // create group=world
  ret = MPI_Group_incl (MPI_GROUP_WORLD, ii, proclist, &group); // set contents
  ret = MPI_Comm_create (MPI_COMM_WORLD, group, &comm);

  ret = MPI_Barrier (MPI_COMM_WORLD);

  if (comm != MPI_COMM_NULL) {
    ret = MPI_Comm_rank (comm, &iamnew);
    if (iamnew == 0) {
      printf ("Root task is running on %s\n", name);
    } else if (iamnew <= nwt) {
      printf ("Write task: WORLD rank %d = comm rank %d is running on %s\n", 
	      iam, iamnew, name);
    } else {
      printf ("Compute task: WORLD rank %d = comm rank %d is running on %s\n", 
	      iam, iamnew, name);
    }
  }
  MPI_Finalize ();
  return 0;
}
