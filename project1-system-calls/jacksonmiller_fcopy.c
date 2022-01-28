/* Jackson Miller
 * COSC3503 Assignment 1
 * 27/01/2022
 *
 * fcopy implementation reads an input file and writes all the data
 * into a specified output file (overwriting any existing data)
 * I have tried two different methods, one with reading one byte at a time
 * and one using a block data buffer. Rudimentary tests showed only a
 * slight performance difference however.
 * Specify at compile time with the BYTE_COPY preprocessor definition.
 *
 * The `strace` utility shows the system calls between the different versions.
 * Command used for testing:
 * $ dd if=/dev/zero of=large.bin bs=1024 count=0 seek=2048
 * $ strace -o trace.log ./fcopy large.bin out.bin
 * The block implementation uses less read() and write() system calls,
 * because of the larger block size
 *
 */
#include <stdio.h>  // for file functions
#include <stdlib.h> // for atoi (str -> integer conversion)

// default block size, overwritten by optional command line argument
#define B_SIZE 65536

int main(int argc, char const *argv[]) {

  int buf_size = B_SIZE;
  // Check for correct usage
  if (argc < 3) {
    printf("Usage: %s SOURCE DEST [BUFFER_SIZE (bytes)]", argv[0]);
    return -1;
  } else if (argc == 4) {
    // Read optional buffer size
    buf_size = atoi(argv[3]);
  }

  // Initialize file pointers and data buffer
  FILE *source, *dest;
  char buffer[B_SIZE];

  // Open input file and check for error
  source = fopen(argv[1], "rb");
  if (source == NULL) {
    printf("%s: cannot stat '%s': No such file", argv[0], argv[1]);
    return 1;
  }

  // Open (or create) destination file and check for errors
  dest = fopen(argv[2], "wb+");
  if (dest == NULL) {
    printf("%s: cannot create destination file %s", argv[0], argv[1]);
    return -1;
  }

// Default operation is to use blocks
#ifndef BYTE_COPY
  size_t in;
  // Read block of input into buffer and write to output
  while (1) {
    if ((in = fread(buffer, 1, B_SIZE, source)) == 0)
      break;
    if (fwrite(buffer, 1, in, dest) == 0)
      break;
  }
#else
  // Also included is a byte-by-byte implementation for testing
	// Enable with -DBYTE_COPY flag at compile time.
#warning byte option selected
  char ch;
  while ((ch = fgetc(source)) != EOF) {
    fputc(ch, dest);
  }
#endif

  return 0;
}
