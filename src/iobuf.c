#include "iobuf.h"

#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void init_es_standard()
{
  stdout = malloc(sizeof(FICHIER));
  if (stdout == NULL) {
    exit(10);
  }
  stdout->fd = 1;
  stdout->buf_i = 0;

  stderr = malloc(sizeof(FICHIER));
  if (stderr == NULL) {
    exit(11);
  }
  stderr->fd = 2;
  stderr->buf_i = 0;
}

FICHIER* ouvrir(const char* nom, char mode)
{
  int file_fd;
  switch (mode) {
    case 'L':
      file_fd = open(nom, O_RDONLY);
      break;
    case 'E':
      file_fd = open(nom, O_WRONLY | O_CREAT, 0700);
      break;
    default:
      fecriref(stderr, "Unsupported mode\n");
      return NULL;
  }

  if (file_fd == -1) {
    fecriref(stderr, "Error opening file\n");
    return NULL;
  }

  FICHIER* file = malloc(sizeof(FICHIER));
  if (file == NULL) {
    fecriref(stderr, "Internal alloc error\n");
    return NULL;
  }

  file->fd = file_fd;
  file->buf_i = 0;
  file->buf_s = 0;

  return file;
}

int fermer(FICHIER* f)
{
  vider(f);
  close(f->fd);
  free(f);
  return 0;
}

int lire(void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  if (f->mode == 'E') {
    fecriref(stderr, "Called `lire` on a file opened in write-only mode\n");
    return 0;
  }

  unsigned int read_elems = 0;
  while (read_elems < nbelem) {
    unsigned int available_bytes = f->buf_s - f->buf_i;
    if (available_bytes < taille) {
      if (available_bytes != 0) {
        // Move last bytes to the beginning of the buffer
        memmove(f->buf, (void*)f->buf + f->buf_i, available_bytes);
      }
      ssize_t bytes_read = read(
          f->fd, (void*)f->buf + available_bytes, MAX_SIZE - available_bytes);
      if (bytes_read == -1) {
        fecriref(stderr, "Error reading from file\n");
        return read_elems;
      }
      f->buf_s = bytes_read + available_bytes;
      f->buf_i = 0;
    }

    if (f->buf_s < taille) {
      // Can't read more elements because reached EOF
      // TODO: Handle when taille > MAX_SIZE
      break;
    }

    memcpy(p, (void*)f->buf + f->buf_i, taille);
    f->buf_i += taille;
    p += taille;
    read_elems++;
  }

  return read_elems;
}

int ecrire(const void* p, unsigned int taille, unsigned int nbelem, FICHIER* f)
{
  // Ecrire logic only uses buf_s and not buf_i

  if (f->mode == 'L') {
    fecriref(stderr, "Called `ecrire` on a file opened in read-only mode\n");
    return 0;
  }

  unsigned int written_elems = 0;
  while (written_elems < nbelem) {
    unsigned int available_bytes = MAX_SIZE - f->buf_s;
    if (available_bytes < taille) {
      // Buffer is full, write it to the file
      vider(f);
    }

    memcpy((void*)f->buf + f->buf_s, p, taille);
    f->buf_s += taille;
    p += taille;
    written_elems++;
  }

  return written_elems;
}

int vider(FICHIER* f)
{
  if (f->mode == 'L') {
    fecriref(stderr, "Called `vider` on a file opened in read-only mode\n");
    return 0;
  }

  ssize_t bytes_written = write(f->fd, f->buf, f->buf_s);
  if (bytes_written == -1) {
    fecriref(stderr, "Error writing to file\n");
    return 0;
  }
  f->buf_s = 0;
  return bytes_written;
}

int fecriref(FICHIER* f, const char* format, ...)
{
  // Simple implementation for now
  ecrire(format, sizeof(char), strlen(format), f);
  return 0;
}

/* directly in stdout */
int ecriref(const char* format, ...)
{
  fecriref(stdout, format);
  return 0;
}

int fliref(FICHIER* f, const char* format, ...)
{
  // TODO: implement
  return 0;
}
