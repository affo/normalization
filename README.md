#Project for Middleware Course A.A. 2013/14 

##Abstract

Use MPI (+OpenMP) to implement a parallel version of the image normalization algorithm. In practice, this means calculating the min and max color values of the submitted image (in parallel) and then perform the normalization (again in parallel). Use the image format you prefer (e.g., "plain ppm").

##Compiling

Move to ```c``` folder:

```normalization $ cd c/```

Compile with ```mpicc```:

```normalization/c $ mpicc -fopenmp -std=c99 *.c```


##Running

Sample run with ```mpirun```:

```normalization/c $ mpirun -np 4 ./a.out ../sample/sample.pgm 10 20```

In this example we use 4 processes and an output range of [10, 20].

To get the log:

```sudo less /var/log/syslog | grep NORM >> norm.log; less norm.log```

##Creating Input Image

We use the _PGM P2_ format for the input image.

To convert an image from any format to _PGM P2_ we recommend to use:

```convert <input-image> -compress none -depth 8 <output-name>.pgm```

Please make sure that the obtained image doesn't contain any comment.

