#ifndef NOTES_H
#define NOTES_H

#include "../logic_constants.h"
#include "../logic_constants.h"

typedef struct {
    char text[MAX_NOTE_TEXT_LENGTH];
} Note;

typedef struct {
    Note note_list[MAX_NOTE_COUNT];
    int note_count;
} Notes;

Notes create_notes();
Note create_note(char * text);



#endif