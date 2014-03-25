#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_LEVENSHTEIN
#include <stdio.h>
#endif

#include "levenshtein.h"

#define MAX_LEN 42
#define min3(a,b,c) ( (a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)) )

int
levenshtein (const char * src, const char * dst) {
    int dist[MAX_LEN + 1][MAX_LEN + 1] = {0};
    int i, j;
    int len_src, len_dst;

    len_src = strlen(src);
    len_dst = strlen(dst);

    for (i = 0 ; i < MAX_LEN ; ++i)
        dist[i][0] = i;
    for (j = 0 ; j < MAX_LEN ; ++j)
        dist[0][j] = j;

    for (i = 0 ; i < len_src ; ++i)
        for (j = 0 ; j < len_dst ; ++j) {

            dist[i + 1][j + 1] = min3(
                                    dist[i][j + 1] + 1,
                                    dist[i + 1][j] + 1,
                                    dist[i][j] + (src[i] != dst[j])
                                 );
        }

    return dist[len_src][len_dst];    
}

#ifdef DEBUG_LEVENSHTEIN
int
main (int argc, char ** av) {
    if (argc == 3) {
        printf("levenshtein(%s, %s) = %d\n", av[1], av[2], levenshtein(av[1], av[2]));
    }

    return EXIT_SUCCESS;
}    
#endif

