#ifndef _IOBUF_H
#define _IOBUF_H

#include <stdlib.h>
#define MAX_SIZE 128

struct _ES_FICHIER;
typedef struct _ES_FICHIER {
  int fd; // file descriptor
  char mode; // L/E

  char buf[MAX_SIZE]; // buffer
  int p; // current byte;
  size_t buf_size;
  
} FICHIER;

extern FICHIER* stdout;
extern FICHIER* stderr;

/* mode: 'L' = lecture, 'E' = écriture */
FICHIER* ouvrir(const char* nom, char mode);
int fermer(FICHIER* f);
int lire(void* p, unsigned int taille, unsigned int nbelem, FICHIER* f);
int ecrire(const void* p, unsigned int taille, unsigned int nbelem, FICHIER* f);
int vider(FICHIER* f);

int fecriref(FICHIER* f, const char* format, ...);
/* directly in stdout */
int ecriref(const char* format, ...);
int fliref(FICHIER* f, const char* format, ...);

#endif
