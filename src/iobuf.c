#include "iobuf.h"

#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

FICHIER* ouvrir(const char* nom, char mode)
{
  if (mode != 'L' || mode != 'E') {
    // fprintf(stderr, "Unsupported mode");
    return NULL;
  }
  FICHIER* file = malloc(sizeof(FICHIER));
  if (file == NULL) {
    // fprintf(stderr, "Internal alloc error");
    return NULL;
  }
  if ((file->fd = open(nom, mode)) == -1) {
    // if open fails
    // fprintf(stderr, "Error opening file");
    free(file);
    return NULL;
  }
  file->mode = mode;
  file->p = 0;
  file->buf_size = 0;

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
  if (f == NULL) {
    // fprintf(stderr, "Empty file pointer");
    return 0;
  }

  if (f->mode != 'L') {
    // fprintf(stderr, "No rights to read this file");
    return 0;
  }
  // reading while possible
  size_t offset = 0;
  size_t bytes;
  int el_read = 0;
  while (nbelem > 0) {
    if (f->buf_size == 0) {
      // filling if buf is full/empty
      if((f->buf_size = read(f->fd, f->buf, MAX_SIZE)) < 1){
        // read did not do anything - quitting
        return el_read;
      }
      f->p = 0;
    }

    // case buffer is not empty, but does not have a "taille" object to read
    if (f->buf_size < taille) {
      // copy the leftovers to the left
      memcpy(f->buf, (f->buf + f->p), (MAX_SIZE - f->p));
      f->p = 0;
      f->buf_size += read(f->fd, (void*)(f->buf) + f->p, (MAX_SIZE - f->buf_size));
    }
    if (f->buf_size < taille) {
      // if still not enough -- quit
      return el_read;
    }
    // else copy current buffer

    bytes = (f->buf_size / taille) * taille; // read only by objects
    memcpy((p + offset), (void*)(f->buf) + f->p, bytes);
    // update buffer and offset the given pointer
    f->buf_size -= bytes;
    f->p += bytes;
    offset += bytes;
    // update return val
    el_read += (bytes / taille);
    nbelem -= (bytes / taille);
  }
  return el_read;
}
int ecrire(const void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
if (f == NULL) {
    // fprintf(stderr, "Empty file pointer");
    return 0;
  }

  if (f->mode != 'E') {
    // fprintf(stderr, "No rights to write this file");
    return 0;
  }
  
  size_t offset = 0;
  size_t bytes;
  int el_read = 0;
  // normally, we should be always able to write all objects
  while (nbelem > 0) {
    memcpy(f->buf, p, (size_t)((MAX_SIZE/taille)*taille));
    
    write(f->fd, f->buf, MAX_SIZE);
  }
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
