#include <stdlib.h>
#include <string.h>

#include "iobuf.h"

int main()
{
  FICHIER* f;
  char* filename = "rand-file.txt";

  init_es_standard();

  f = ouvrir(filename, 'E');
  if (f == NULL)
    exit(-1);

  fecriref(f,
           "Testing format for decimal %d , string %s and char %c",
           123456789,
           "test",
           't');

  fermer(f);

  f = ouvrir(filename, 'L');
  if (f == NULL)
    exit(-1);
  {
    char buff[1024];
    char* expected =
        "Testing format for decimal 123456789 , string test and char t";
    lire(&buff, 1, 1024, f);

    size_t len = strlen(expected);
    int result = strncmp(buff, expected, len);
    if (result != 0)
      ecriref("Error: %d\n", result);
    else
      ecriref("OK\n");
  }
  fermer(f);
  vider(stdout);

  return 0;
}
