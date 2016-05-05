/* Grows two files in parallel and checks that their contents are
   correct. */

#include <random.h>
#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"

#define BLOCK_SIZE 512
static char buf_a[BLOCK_SIZE];

static void
write_some_bytes (const char *file_name, int fd, const char *buf)
{
  size_t ret_val;
  ret_val = write (fd, buf, BLOCK_SIZE);
  if (ret_val != BLOCK_SIZE){
    msg("WTF IT FAILS");
    fail ("write %zu bytes in \"%s\" returned %zu",
          BLOCK_SIZE, file_name, ret_val);
  }
}

void
test_main (void)
{
  int fd;
  int n_read1;
  int n_read2;

  random_init (0);
  random_bytes (buf_a, sizeof buf_a);

  CHECK (create ("a", 0), "create \"a\"");
  CHECK ((fd = open ("a")) > 1, "open \"a\"");

  n_read1 = buffer_stat (2);

  msg ("write 100 kB to \"a\"");
  int i = 0;
  for(;i < 200; i++)
  {
    write_some_bytes ("a", fd, buf_a);
  }
  n_read2 = buffer_stat (2);
  msg("called block_read %d times in 200 writes", n_read2-n_read1);
  msg ("close \"a\"");
  close (fd);
}
