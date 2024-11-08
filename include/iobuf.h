#ifndef _IOBUF_H
#define _IOBUF_H

#define MAX_SIZE 128

typedef struct iobuf_s{
  char buf[MAX_SIZE]; // buffer
  int p; // current byte;
} iobuf;

struct _ES_FICHIER;
typedef struct _ES_FICHIER {
  int fd; // file descriptor
  char mode; // L/E
  iobuf* fbuf; // custom file buffer
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
