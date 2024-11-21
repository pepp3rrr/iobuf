#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

#include "iobuf.h"

#define MEMORY_SIZE 1280000 /* 1,3 Mo */

char MEMORY[MEMORY_SIZE];

void init_mem(void)
{
  init_es_standard();
  ecriref("Initializing memory...\n");
  vider(stdout);

  int i;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);

  for (i = 0; i < MEMORY_SIZE;)
    MEMORY[i++] = (char)(rand() % 95 + 0x20); /* printable characters */
}

void mem_compare(char* ref, char* buff, int nbytes)
{
  int i;
  ecriref("Comparing memories...\n");
  for (i = 0; i < nbytes; i++) {
    if (ref[i] != buff[i]) {
      fecriref(stderr, "ERROR %c != %c at index %d\n", ref[i], buff[i], i);
    }
  }
  ecriref("Done\n");
  vider(stdout);
}

int main()
{
  FICHIER* f;
  char* filename = "rand-file.txt";

  unsigned int elem_size = MAX_SIZE + 69;
  unsigned int elem_count = 42;

  init_mem();

  f = ouvrir(filename, 'E');
  if (f == NULL)
    exit(-1);

  ecrire(MEMORY, elem_size, elem_count, f);
  fermer(f);

  {
    char* buffer = malloc(sizeof(char) * MEMORY_SIZE);
    f = ouvrir(filename, 'L');
    if (f == NULL)
      exit(-1);
    lire(buffer, elem_size, elem_count, f);
    mem_compare(MEMORY, buffer, elem_count * elem_size);
    free(buffer);
    fermer(f);
  }
}
