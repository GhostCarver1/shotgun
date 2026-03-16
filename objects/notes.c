#include "notes.h"
#include <string.h>

Notes create_notes()
{
    Notes notes;
    notes.note_count = 0;
    return notes;
}

Note create_note(char * text)
{
    Note note;
    strncpy(note.text, text, sizeof(note.text) - 1);
    note.text[sizeof(note.text) - 1] = '\0';
    return note;
}