/**
 * @file  music.c
 * @author Profesores EDAT
 * @date February 2026
 * @mersion 1.0
 * @brief Library to manage ADT Music
 *
 * @details
 *
 * @see
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "music.h"

/* Music declaration goes here */

struct _Music
{
  long            id;
  char            title[STR_LENGTH];
  char            artist[STR_LENGTH];
  unsigned  short duration;
  State           status;

};

/*----------------------------------------------------------------------------------------*/
/*
Private function:
*/

Status music_setField (Music *m, char *key, char *value) {
  if (!key || !value) return ERROR;

  if (strcmp(key, "id") == 0) {
    return music_setId(m, atol(value));
  } else if (strcmp(key, "title") == 0) {
    return music_setTitle(m, value);
  } else if (strcmp(key, "artist") == 0) {
    return music_setArtist(m, value);
  } else if (strcmp(key, "duration") == 0) {
    return music_setDuration(m, (unsigned short)atoi(value));
  } else if (strcmp(key, "state") == 0) {
    return music_setState(m, (State)atoi(value));
  }

  return ERROR;
}

/*----------------------------------------------------------------------------------------*/
Music *music_initFromString(char *descr) {
    Music *m;
    char *p, *key_start, *value_start;
    char *buffer;

    if (!descr) return NULL;

    buffer = strdup(descr);
    if (!buffer) return NULL;

    m = music_init();
    if (!m) {
        free(buffer);
        return NULL;
    }

    p = buffer;
    while (*p) {
        /* Skip leading whitespace */
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;

        /* Parse key */
        key_start = p;
        while (*p && *p != ':') p++;
        if (!*p) break;
        *p++ = '\0'; /* Null-terminate key */

        /* Skip whitespace and the opening quote */
        while (isspace((unsigned char)*p)) p++;
        if (*p != '"') break;
        p++;

        /* Parse value */
        value_start = p;
        while (*p && *p != '"') p++;
        if (!*p) break;
        *p++ = '\0'; /* Null-terminate value */

        music_setField(m, key_start, value_start);
    }

    free(buffer);
    return m;
}


/**  Remaining functions of music.h to be implemented here **/

Music * music_init (){
  Music * newMusic = NULL;

  newMusic = (Music *) malloc(1 * sizeof(Music));

  if(!newMusic) return NULL;

  newMusic->id = 0;
  strcpy(newMusic->title, "");
  strcpy(newMusic->artist, "");
  newMusic->duration = 0;
  newMusic->status = NOT_LISTENED;

  return  newMusic;
}

void music_free (void * m){
  if(m) {free(m);}
}
const char* music_getTitle (const Music * m){

  if(!m) return (NULL);

  return m->title;
}
const char* music_getArtist (const Music * m){

  if(!m) return (NULL);

  return m->artist;
}
Status music_setId (Music * m, const long id){

  if(!m) return (ERROR);

    m->id = id;

  return OK;
}
Status music_setTitle (Music * m, const char * title){

  if(!m || !title)
    return (ERROR);

  strcpy(m->title, title);

  return OK;
}
Status music_setArtist (Music * m, const char * artist){

  if(!m || !artist) return (ERROR);

  strcpy(m->artist, artist);

  return OK;
}
Status music_setDuration (Music * m, const unsigned short duration){

  if(!m) return (ERROR);

  m->duration = duration;
  return OK;
}
Status music_setState (Music * m, const State state){

  if(!m) return (ERROR);

  m->status = state;
  return OK;
}
int music_cmp (const void * m1, const void * m2){

  Music* music1 = NULL;
  Music* music2 = NULL;
  int    result;

  if(!m1 || !m2) return (ERROR_MUSIC);

  music1= (Music *) m1;
  music2= (Music *) m2;

  if (music1->id < music2->id) return -1;
  if (music1->id > music2->id) return  1;

  /* If ids are equal, compare title */
  result = strcmp( music1->title ,  music2->title );
  if (result != 0) {
      return result;
  }

  /* If titles are equal, compare artist */
  return strcmp( music1->artist ,  music2->artist );
}
void * music_copy (const void * src){
  Music * source = NULL;
  Music * dest = NULL;
  
  dest = music_init();
  if(!src || !dest) return NULL; 

  source = (Music *)src;
  strcpy(dest->artist, source->artist);
  strcpy(dest->title, source->title);
  dest->id = source->id;
  dest->duration = source->duration;
  dest->status = source->status;

  return  dest;
}
unsigned short music_getDuration (const Music * m){

  if(!m) return 0; /* Return 0 on error to avoid -1 cast issues */

  return m->duration;
}
State music_getState (const Music * m){
  if(!m) return ERROR_MUSIC;

  return m->status;
}
int music_plain_print (FILE * pf, const void * m){

  Music * music = NULL;
  if(!pf  || !m) return (ERROR_I);


  music = (Music *)m;
  return (fprintf(pf,"[%ld, %s, %s, %hu, %d]", music->id, music->title, music->artist,  music->duration, music->status));

}
long music_getId (const Music * m){

  if(!m) return (ERROR_I);
  return m->id;
}

int music_formatted_print (FILE * pf, const void * m) {
        Music * aux;
        int counter = 0, minutes, sec;
        if (!pf || !m) return -1;

        aux = (Music*) m;

        if (!aux->duration || aux->duration <= 0) return -1;
        minutes = aux->duration / 60;
    sec = aux->duration % 60;

        counter = fprintf(pf, "\t ɴᴏᴡ ᴘʟᴀʏɪɴɢ: %s\n", aux->title);
        counter += fprintf(pf, "\t • Artist %s •\n", aux->artist);
        counter += fprintf(pf, "\t──────────⚪──────────\n");
        counter += fprintf(pf, "\t\t◄◄⠀▐▐ ⠀►►\n");
        counter += fprintf(pf, "\t 0:00 / %02d:%02d ───○ 🔊⠀\n\n", minutes, sec);

        return counter;
}


