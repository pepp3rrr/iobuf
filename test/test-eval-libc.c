#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *f1; // fichier pour lire
  FILE *f2; // fichier pour ecrire
  char c;

  f1 = fopen("test_files/test_file1.txt", "r");
  if (f1 == -1)
    exit(-1);

  f2 = fopen("test_files/test_file2.txt", "w");
  if (f2 == -1)
    exit(-1);

  while (fread(&c, 1, 1, f1) == 1) {
    fwrite(&c, 1, 1, f2);
  }

  fclose(f1);
  fclose(f2);

  return 0;
}
