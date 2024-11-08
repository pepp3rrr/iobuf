#include "iobuf/iobuf.h"

FICHIER* ouvrir(const char* nom, char mode)
{
  // TODO: implement
  return (void*)0;
}

int fermer(FICHIER* f)
{
  // TODO: implement
  return 0;
}

int lire(void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  // TODO: implement
  return 0;
}
int ecrire(const void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  // TODO: implement
  return 0;
}
int vider(FICHIER* f)
{
  // TODO: implement
  return 0;
}

int fecriref(FICHIER* f, const char* format, ...)
{
  // TODO: implement
  return 0;
}
/* directly in stdout */
int ecriref(const char* format, ...)
{
  // TODO: implement
  return 0;
}
int fliref(FICHIER* f, const char* format, ...)
{
  // TODO: implement
  return 0;
}