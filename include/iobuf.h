#ifndef _IOBUF_H
#define _IOBUF_H

#include <stdlib.h>
#define MAX_SIZE 128

struct _ES_FICHIER;
typedef struct _ES_FICHIER {
  /// The UNIX file descriptor
  int fd;
  /// The file open mode, L | E
  char mode;
  /// The buffer for caching disk reads & writes
  char (*buf)[MAX_SIZE];
  /// The current index in the buffer
  size_t buf_i;
} FICHIER;

extern FICHIER* stdout;
extern FICHIER* stderr;

void init_es_standard();

/// @brief Opens a file from a given path, uses `open` internally
/// @param nom The path (relative and absolute supported) of the file to open
/// @param mode The file opening mode, 'L' for read-only, 'R' for write-only
/// @return A file descriptor containing metadata and read/write cache buffers
FICHIER* ouvrir(const char* nom, char mode);

/// @brief Closes a file descriptor, uses `close` internally
/// @param f The file descriptor to close
/// @return The number of bytes flushed to disk before closing, or -1 if failed
int fermer(FICHIER* f);

/// @brief Reads data from a file
/// @param p The buffer to write read data into
/// @param taille The size of the elements (`sizeof`) to read from the file
/// @param taille The number of the elements to read from the file
/// @param f The file to read the data from
/// @return The number of elements that have been read (cache or disk)
int lire(void* p, unsigned int taille, unsigned int nbelem, FICHIER* f);

/// @brief Writes data to a file
/// @param p The buffer to read write data from
/// @param taille The size of the elements (`sizeof`) to write into the file
/// @param taille The number of the elements to write into the file
/// @param f The file to write data to
/// @return The number of elements that have been written to the disk or cached
/// in the buffer. Call `vider` to ensure elements are written from the cache
/// buffer to the disk.
int ecrire(const void* p, unsigned int taille, unsigned int nbelem, FICHIER* f);

/// @brief Flushes data from the file cache buffer to the disk.
/// @return The number of bytes that has been written to the disk
int vider(FICHIER* f);

int fecriref(FICHIER* f, const char* format, ...);
/* directly in stdout */
int ecriref(const char* format, ...);
int fliref(FICHIER* f, const char* format, ...);

#endif
