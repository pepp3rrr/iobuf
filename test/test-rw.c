#include "iobuf.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FICHIER *f1;
	FICHIER *f2;
	char c;

	f1 = ouvrir ("test_files/test_file1.txt", 'L');
	if (f1 == NULL)
		exit (-1);

	f2 = ouvrir ("test_files/test_file2.txt", 'E');
	if (f2 == NULL)
		exit (-1);

	while (lire (&c, 1, 1, f1) == 1) {
          ecrire (&c, 1, 1, stdout);
          ecrire (&c, 1, 1, f2);
	}
/*        vider (stdout);*/

        fermer (f1);
        fermer (f2);

	return 0;
}
