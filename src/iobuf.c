#include "iobuf.h"

#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

FICHIER* ouvrir(const char* nom, char mode)
{
  int file_fd;
  switch (mode) {
    case 'L':
      file_fd = open(nom, O_RDONLY);
      break;
    case 'E':
      file_fd = open(nom, O_WRONLY);
      break;
    case 'A':
      file_fd = open(nom, O_RDWR);
      break;
    default:
      // fecriref(stderr, "Unsupported mode");
      return NULL;
  }

  if (file_fd == -1) {
    // fecriref(stderr, "Error opening file");
    return NULL;
  }

  FICHIER* file = malloc(sizeof(FICHIER));
  if (file == NULL) {
    // fecriref(stderr, "Internal alloc error");
    return NULL;
  }

  char(*rbuf)[MAX_SIZE] = NULL;
  char(*wbuf)[MAX_SIZE] = NULL;
  if (mode == 'L' || mode == 'A') {
    rbuf = malloc(MAX_SIZE);
    if (rbuf == NULL) {
      // fecriref(stderr, "Internal alloc error");
      free(file);
      return NULL;
    }
  }
  if (mode == 'E' || mode == 'A') {
    wbuf = malloc(MAX_SIZE);
    if (wbuf == NULL) {
      // fecriref(stderr, "Internal alloc error");
      free(file);
      free(rbuf);
      return NULL;
    }
  }

  file->fd = file_fd;
  file->rbuf = rbuf;
  file->wbuf = wbuf;
  file->wbuf_p = 0;
  file->rbuf_p = 0;
  file->rbuf_s = 0;

  return file;
}

int fermer(FICHIER* f)
{
  close(f->fd);
  free(f->wbuf);
  free(f->rbuf);
  free(f);
  return 0;
}

int lire(void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  if (f->rbuf == NULL) {
    fecriref(stderr, "Called `lire` on a file opened in write-only mode");
    return 0;
  }

  char refill_rbuf = f->rbuf_p == 0 && nbelem && taille;
  unsigned int read_elems = 0;
  while (read_elems != nbelem) {
    /// Initialize the buffer the first time
    if (refill_rbuf) {
      ssize_t bytes_read =
          read(f->fd, f->rbuf + f->rbuf_p, MAX_SIZE - f->rbuf_p);
      if (bytes_read == -1) {
        fecriref(stderr, "Error reading from file");
        return read_elems;
      }
      if (bytes_read == 0) {
        return read_elems;
      }
      f->rbuf_s = f->rbuf_p + bytes_read;
      refill_rbuf = 0;
    }

    unsigned int nbytes =  // Number of bytes to read
        (nbelem - read_elems) * taille;
    int available_bytes = MIN(nbytes, f->rbuf_s - f->rbuf_p);
    int available_elems =
        available_bytes / taille;  // Integer division, remainder is discarded
    if (available_elems <= 0) {
      unsigned int remaining_bytes = f->rbuf_s - f->rbuf_p;
      // Copy the remaining bytes to the beginning of the buffer
      memmove(f->rbuf, f->rbuf + f->rbuf_p, remaining_bytes);
      f->rbuf_p = 0;
      f->rbuf_s = remaining_bytes;
      // Refill the buffer
      refill_rbuf = 1;
      continue;
    } else {
      mempcpy(p, f->rbuf + f->rbuf_p, available_elems * taille);
      p += available_elems * taille;
      read_elems += available_elems;
      f->rbuf_p += available_elems * taille;
    }
  }

  return read_elems;
}
int ecrire(const void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  if (f->wbuf == NULL) {
    fecriref(stderr, "Called `ecrire` on a file opened in read-only mode");
    return 0;
  }

  unsigned int written_elems = 0;
  while (written_elems != nbelem) {
    unsigned int nbytes =  // Number of bytes to write
        (nbelem - written_elems) * taille;
    int available_bytes = MIN(nbytes, MAX_SIZE - f->wbuf_p);
    int available_elems =
        available_bytes / taille;  // Integer division, remainder is discarded
    if (available_elems <= 0) {
      // Flush the buffer
      vider(f);
      continue;
    } else {
      mempcpy(f->wbuf + f->wbuf_p, p, available_elems * taille);
      p += available_elems * taille;
      written_elems += available_elems;
      f->wbuf_p += available_elems * taille;
    }
  }

  return written_elems;
}

int vider(FICHIER* f)
{
  return 0;
}

int fecriref(FICHIER* f, const char* format, ...)
{
  // Simple implementation for now
  write(f->fd, format, strlen(format));
  return 0;
}
/* directly in stdout */
int ecriref(const char* format, ...)
{
  // Simple implementation for now
  write(stdout->fd, format, strlen(format));
  return 0;
}
int fliref(FICHIER* f, const char* format, ...)
{
  // TODO: implement
  return 0;
}
