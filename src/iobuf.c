#include "iobuf.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

FICHIER* ouvrir(const char* nom, char mode)
{
  if (!(mode == 'W' || mode == 'R')) {
    fprintf(stderr, "Unsupported mode");
    return NULL;
  }
  FICHIER* file = malloc(sizeof(FICHIER));
  if (file == NULL) {
    fprintf(stderr, "Internal alloc error");
    return NULL;
  }
  if (file) {
    if (file->fd = open(nom, mode) == NULL) {
      // if open fails
      fprintf(stderr, "Error opening file");
      free(file);
      return NULL;
    }
    file->mode = mode;
    file->p = 0;
    file->buf_size = 0;
  }
  return file;
}

int fermer(FICHIER* f)
{
  close(f->fd);
  free(f);
  return 0;
}

int lire(void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  if (f) {
    if (f->mode != 'R') {
      fprintf(stderr, "No rights to read this file");
      return 0;
    }
    if (f->buf_size == 0) {
      // read if buf is full/empty
      f->buf_size = read(f->fd, f->buf, MAX_SIZE);
    }
    //
  }
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