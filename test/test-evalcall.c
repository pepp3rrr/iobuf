#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int f1; // fichier pour lire
  int f2; // fichier pour ecrire
  char c;

  f1 = open("test_files/test_file1.txt", O_RDONLY);
  if (f1 == -1)
    exit(-1);

  f2 = open("test_files/test_file2.txt", O_WRONLY | O_CREAT, 0700);
  if (f2 == -1)
    exit(-1);

  while (read(f1, &c, 1) == 1) {
    write(f2, &c, 1);
  }
  close(f1);
  close(f2);

  return 0;
}
