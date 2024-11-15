#include "iobuf.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

FICHIER *ouvrir(const char *nom, char mode){
  if(!(mode == 'W' || mode == 'R')){
    //fprintf(stderr, "Unsupported mode");
    return NULL;
  }
  FICHIER* file = malloc(sizeof(FICHIER));
  if(file == NULL){
    //fprintf(stderr, "Internal alloc error");
    return NULL;
  }
  if((file->fd = open(nom, mode)) == -1){
    // if open fails
    //fprintf(stderr, "Error opening file");
    free(file);
    return NULL;
  }
  file->mode = mode;
  file->p = 0;
  file->buf_size = 0;

  return file;
}

int fermer(FICHIER* f){
  close(f->fd);
  free(f);
  return 0;
}

int lire(void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  if(f == NULL){
    //fprintf(stderr, "No rights to read this file");
    return 0;
  }

  if(f->mode != 'R'){
    //fprintf(stderr, "No rights to read this file");
    return 0;
  }
  // reading while possible
  size_t offset = 0;
  size_t size;
  size_t size_read = 0;
  while(nbelem > 0){
    if(f->buf_size == 0){
      // filling if buf is full/empty
      f->buf_size = read(f->fd, f->buf, MAX_SIZE);
      f->p = 0;
    }
    // the rest is too small, shifting + filling
    if(f->buf_size < taille){
      memcpy(f->buf, f->buf + f->p, (MAX_SIZE - f->p));
      f->p = MAX_SIZE - f->p;
      f->buf_size += read(f->fd, f->buf + f->p, (MAX_SIZE - f->buf_size));
    }
    if(f->buf_size < taille){
      // if still not enough - end
      return size_read;
    }
    // else copy current buffer
    size = (f->buf_size/taille)*taille;
    memcpy((p + offset),f->buf[f->p], size);
    // update buffer and offset the given pointer
    f->buf_size -= size;
    f->p += size;
    offset += size;
    // update return val
    size_read += (size/taille);
    nbelem -= size/taille;
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