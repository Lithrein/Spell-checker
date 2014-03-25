#ifndef H_BUFFER
#define H_BUFFER

#include <stddef.h> /* size_t */

#define BLOCK_SZ 256
#define WINDOW_SZ 1024

/** Buffer definition ** ******************************************************
 * 
 * Buffers  are  a way to map directly some text into the random access memory
 * (RAM) with virtually no limitation of length.
 *
 * There are two ways to create and use a buffer.
 *      - from a stream (ie. a file):
 *          In this case, the buffer is in read-only mode. And the buffer goes
 *      through  the  text with a "window" (the actually loaded text) in order
 *      not to saturate the memory.
 *
 *      - from stracth:
 *          In  this  case, you can add/erase content _only_ at the end of the
 *      buffer.
 *
 **************************************************/

struct buffer {
    enum buf_state state; /* State of the buffer. */

    char ** raw; /* The content of the buffer by chunks of 256 chars. */

    char * read; /* Stores the result of the last call of reading functions. */
    char * lines; /* Stores the result of the last call of reading functions. */
    struct {
        int c; /* The number of the chunk. */
        int p; /* The position in the chunk. */
    } reading_pos, end_pos; /* Position of the cursor. / End of the text */

    size_t length; /* The length of the text. */
    size_t total_length; /* The size of the buffer. */
    size_t widen_calls; /* Number of calls to the function 'widen' */
    size_t raw_sz; /* The number of chunks. */
    size_t nb_lines; /* The number of read lines. */

    int stream_mode; /* Stream mode activation. */
};

/* States definition */
enum buf_state {
    NOTHING_TO_REPORT,
    EMPTY_BUFFER,
    UO_READ_ONLY_MODE; /*! Unauthorized operation in stream_mode */
    END_OF_FILE    
};

/******************************************************************************
 ******* Creation & Deletion functions **************************************** 
 ******************************************************************************/

/**
    buffer_new : void -> buffer *

    Creates a new buffer from scratch.
*/
struct buffer * buffer_new (void);
/**
    buffer_from_stream : FILE * -> buffer *

    Creates a buffer from a stream
*/
struct buffer * buffer_from_stream (FILE * stream);
/**
    buffer_clear : buffer * -> buffer *

    Clears the content of the buffer.
*/
struct buffer * buffer_clear (struct buffer * buf);
/**
    buffer_delete : buffer * -> int

    Erase definitively the buffer 'bufÂ´.
    Returns 0 in case of fqilure and 1 in case of success.
*/
int buffer_delete (struct buffer * buf);

/******************************************************************************
 ******* Functions to move into the buffer ************************************
 ******************************************************************************/

/**
    buffer_find_next : (buffer *, char) -> int

    Returns the next position (from the current one) of the character 'c´. 
*/
int buffer_find_next (struct buffer * buf, char c);
/**
    buffer_go_to_next : (buffer *, char) -> void

    Jumps to the next occurrence of the character 'c´.
*/
void buffer_go_to_next (struct buffer * buf, char c);
/**
    buffer_forward : (buffer *, size_t) -> buffer *

    Jumps 'size´ characters forward.
*/
struct buffer * buffer_forward (struct buffer * buf, size_t size);
/**
    buffer_backward : (buffer *, size_t) -> buffer *

    Jumps 'size´ characters backward.
*/
struct buffer * buffer_backward (struct buffer * buf, size_t size);

/******************************************************************************
 ******* Writing & Reading functions ******************************************
 ******************************************************************************/

/**
    buffer_append : (buffer *, char *) -> buffer *

    Appends 'data´ at the end of the buffer 'buf´.
*/
struct buffer * buffer_append (struct buffer * buf, char * data);
/**
    buffer_remove : (buffer *, size_t) -> buffer *

    Removes 'n´ characters at the end of the buffer 'buf´.
*/
struct buffer * buffer_remove (struct buffer * buf, size_t n);

/**
    buffer_read : (buffer *, size_t) -> char *

    Returns the read charcters.
*/
char * buffer_read (struct buffer * buf, size_t n);
/**
    buffer_read_lines : (buffer *, size_t) -> char *

    Returns the 'n´ lines read.
*/
char * buffer_read_lines (struct buffer * buf, size_t n);
/**
    buffer_read_line : (buffer *, size_t) -> char *

    Returns the read line.
*/
char * buffer_read_line (struct buffer * buf);

/******************************************************************************
 ******* For Debugging Purposes ***********************************************
 ******************************************************************************/

/**
    buffer_state : buffer * -> char *
    
    Returns a message with the current state of the buffer.
*/
char * buffer_state (struct buffer * buf);

#endif /* H_BUFFER  */

