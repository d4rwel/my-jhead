#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define ITEM_COUNT 4

typedef struct pictureInfo {
    char description[20]; 
    off_t offset;
    size_t size;
} PictureInfo;

int main(int argc, char **argv) {

    if( argc != 2 ) {
        fprintf(stderr, "USAGE: myjhead jpg-file\n");
        exit(EXIT_FAILURE);
    }

    int fd;

    void *buf = (char *) malloc(sizeof(char) * 20);
    if( buf == NULL ) {
        fprintf(stderr, "EXIT ON ERROR: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    PictureInfo pictureInfo[] = {
        {"HERSTELLER", 0xC8, 4},
        {"MODELL", 0xD8, 9},
        {"FIRMWARE VERSION", 0xF2, 15},
        {"FOTO ERSTELLT", 0x102, 19}
    };

    if( (fd = open(argv[1], O_RDONLY)) == -1 ) {
        perror("EXIT ON ERROR"); 
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Informationen zu \"%s\":\n", argv[1]);
    for( int i = 0; i < ITEM_COUNT; i++ ) {
        if( lseek(fd, pictureInfo[i].offset, SEEK_SET) == -1 ) {
            perror("EXIT ON ERROR");
            exit(EXIT_FAILURE);
        }
        if( read(fd, buf, pictureInfo[i].size) == -1 ) {
            perror("EXIT ON ERROR");
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "\t%s : %s\n", pictureInfo[i].description, (char *)buf);
    }

    if( close(fd) == -1 ) {
        perror("EXIT ON ERROR");
        exit(EXIT_FAILURE);
    }

    free(buf);
            
    return EXIT_SUCCESS;
}
