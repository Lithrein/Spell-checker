#include <stdio.h>
#include <string.h>

#include "buffer.h"

static struct buffer *
widen (struct buffer * buf, int by /* at least */) {
    
}

struct buffer *
buffer_new (void) {
    struct buffer * buf = NULL;

    if ((buf = malloc(sizeof *buf)) != NULL) {
        buf->raw = NULL;
        buf->read = NULL;
        buf->lines = NULL;
        buf->reading_pos = buf->end_pos = {0, 0};
        buf->raw_sz = 0;
        buf->nb_lines = 0;
        buf->length = 0;
        buf->total_length = 0;
        buf->stream_mode = 0;
        buf->widen_calls = 0;
        buf->state = NOTHING_TO_REPORT;
    }

    return buf;
}

struct buffer *
buffer_from_stream (FILE * stream) {
    struct buffer * buf = NULL;

    if ((buf = buffer_new()) != NULL) {
        buf->stream_mode = 1;
    }
    
    /* A completer */
    
    return buf;
}

struct buffer *
buffer_clear (struct * buffer buf) {
    if (buf != NULL && buf-> raw != NULL) {
        buf->reading_pos = buf->end_pos = {0, 0};
        buf->raw_sz = 0;
        buf->length = 0;
        buf->total_length = 0;
        buf->widen_calls = 0;
        buf->stream_mode = 0;    
        free(buf->read), buf->read = NULL;

        while(--buf->nb_lines > 0)
            free(buf->lines[buf->nb_lines]), buf->lines[buf->nb_lines] = NULL;
        free(buf->lines), buf->lines = NULL;

        while (--buf->raw_size > 0)
            free(buf->raw[buf->raw_size]), buf->raw[buf->raw_size] = NULL;
        free(buf->raw), buf->raw = NULL;
    }
    
    return buf;
}

int
buffer_delete (struct buffer * buf) {
    buffer_clear(buf);
    free(buf), buf = NULL;
    
    return 1;
}

/* ... */

struct buffer *
buffer_append (struct buffer * buf, char * data) {
    if (buf->stream_mode == 1) {
        buffer->state = UO_READ_ONLY_MODE;
    } else {
        int data_len = strlen(data);
        int start_pos = 0;
        int i, j;

        if (buf->length + data_len > buf->total_length) {
            widen(buf, buf->length + data_length - buf->total_length);
            /* TODO: Handle possible memory allocation error */
        }
    
        for (i = buf->end_pos.c ; data && i < buf->raw_sz ; ++i) {
            for (j = buf->end_pos.p ; data && j < BLOCK_SZ ; ++j) {
                buf->raw[i][j] = data++;
            }
            buf->end_pos.p = j % (BLOCK_SZ - 1);
        }
        buf->end_pos.c = i;
        buf->end_pos.p = j;
    }
    
    return buf;
}

struct buffer *
buffer_remove (struct buffer * buf, size_t n) {
    if (buf->stream_mode == 1) {
        buffer->state = UO_READ_ONLY_MODE;
    } else {
        int i, j;

        for (i = buf->end_pos.c ; i >= 0 ; --i) {
            for (j = buf->end.pos ; j >= 0 ; --j) {
                buf->raw[i][j] = '\0';
            }
            buf->end_pos.p = j % (BLOCK_SZ - 1);
        }
        buf->end_pos.c = i;
        buf->end_pos.p = j;
    }

    /*TODO : Possible error : The 'reading_pos' has been erased */
    return buf;
}

char *
buffer_read (struct buffer * buf, size_t n) {
    if (buf->read)
        free(buf->read), buffer->read = NULL;

    if((buf->read = calloc(1, n + 1)) == NULL) {
        /* Memory allocation error */
    } else {
        int i, j, k;

        for (i = buf->reading_pos.c, k = 0 ; n && i < buf->raw_sz ; ++i) {
            for (j = buf->reading_pos.p ; n && j < BLOCK_SZ ; ++j) {
               buf->read[k++] = buf->raw[i][j]; 
            }
            buf->reading_pos.p = j % (BLOCK_SZ - 1);
        }

        buf->reading_pos.c = i;
        buf->reading_pos.p = j;
    }

    return buf->read;
}

char *
buffer_read_lines (struct buffer * buf, size_t n) {

}

char *
buffer_read_line (struct buffer * buf) {
    return buffer_read_lines(buf, 1);
}

