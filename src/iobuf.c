#include "iobuf.h"

#include <fcntl.h>
#include <stdarg.h>
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
      // Read directly into the user buffer
      ssize_t bytes_read = read(f->fd, p, taille);
      if (bytes_read == -1) {
        fecriref(stderr, "Error reading from file\n");
        return read_elems;
      }
      return bytes_read / taille;  // Return number of full elements read
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

    if (taille > MAX_SIZE) {
      ssize_t bytes_written = write(f->fd, p, taille);
      if (bytes_written == -1) {
        fecriref(stderr, "Error writing to file\n");
        return written_elems;
      }
      return bytes_written / taille;
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

int vfecriref(FICHIER* f, const char* format, va_list args)
{
  char buffer[MAX_SIZE];
  char* buf_ptr = buffer;
  const char* fmt = format;
  int written = 0;

  while (*fmt && buf_ptr < buffer + MAX_SIZE - 1) {
    if (*fmt == '%') {
      fmt++;
      if (*fmt == 'd') {  // Integer
        int value = va_arg(args, int);
        char int_buf[20];  // Temporary buffer for integer, don't mind buffer
                           // overflows that's C anyway
        int len = 0;
        if (value < 0) {
          *buf_ptr++ = '-';
          value = -value;
          written++;
        }
        do {
          int_buf[len++] = '0' + (value % 10);
          value /= 10;
        } while (value > 0);

        for (int i = len - 1; i >= 0; i--) {
          *buf_ptr++ = int_buf[i];
          written++;
        }
      } else if (*fmt == 's') {  // String
        const char* str = va_arg(args, const char*);
        while (*str && buf_ptr < buffer + MAX_SIZE - 1) {
          *buf_ptr++ = *str++;
          written++;
        }
      } else if (*fmt == 'c') {  // Character
        char c = (char)va_arg(args, int);
        *buf_ptr++ = c;
        written++;
      }
      // TODO: Add support for other formats like %f, %x as needed
    } else {
      // Copy non-placeholder characters directly
      *buf_ptr++ = *fmt;
      written++;
    }
    fmt++;
  }

  va_end(args);

  // Write the buffer content to the file
  ecrire(buffer, sizeof(char), buf_ptr - buffer, f);
  return written;  // Return the number of characters written
}

int fecriref(FICHIER* f, const char* format, ...)
{
  va_list args;
  va_start(args, format);

  int written = vfecriref(f, format, args);

  va_end(args);
  return written;
}

/* directly in stdout */
int ecriref(const char* format, ...)
{
  va_list args;
  va_start(args, format);

  // Use the existing vfecriref implementation
  int written = vfecriref(stdout, format, args);

  va_end(args);
  return written;
}

int fliref(FICHIER* f, const char* format, ...)
{
  char input[MAX_SIZE];
  int read_bytes = lire(input, sizeof(char), MAX_SIZE - 1, f);
  if (read_bytes <= 0) {
    return -1;  // Error or EOF
  }
  input[read_bytes] = '\0';  // Null-terminate the input string

  va_list args;
  va_start(args, format);

  const char* fmt = format;
  const char* inp = input;
  int matches = 0;

  while (*fmt && *inp) {
    if (*fmt == '%') {
      fmt++;
      if (*fmt == 'd') {  // Integer
        int* target = va_arg(args, int*);
        char* endptr;
        *target = strtol(inp, &endptr, 10);
        if (endptr == inp)
          break;  // No valid integer found
        inp = endptr;
        matches++;
      } else if (*fmt == 's') {  // String
        char* target = va_arg(args, char*);
        while (*inp && *inp != ' ' && *inp != '\n') {
          *target++ = *inp++;
        }
        *target = '\0';
        matches++;
      }
      // TODO: Add other cases (e.g., %f for floats) as needed
    } else {
      // Skip over matching characters
      if (*fmt == *inp) {
        inp++;
      } else {
        break;  // Format and input mismatch
      }
    }
    fmt++;
  }

  va_end(args);
  return matches;  // Return number of successfully matched fields
}
