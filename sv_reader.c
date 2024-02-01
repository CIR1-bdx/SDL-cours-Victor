#include <dirent.h>

int read_pages(Page *pages[]) {
    struct dirent *de; // Pointer for directory entry 
    
    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir("pages");

    // opendir returns NULL if couldn't open directory
    if (dr == NULL) { 
        printf("Could not open current directory" ); 
        return 0; 
    } 

    // skip . and ..
    readdir(dr);
    readdir(dr);

    // for readdir()
    for (nbPage = 1;(de = readdir(dr)) != NULL; ++nbPage) {
        char path[1024];
        strcpy(path, "pages/");
        strcat(path, de->d_name);

        FILE *file = fopen(path, "r");
        if (file == NULL) {
            perror(file);
            return 1;
        }

        char buffer[1024];
        fgets(pages[nbPage].title.title, 1024, file);
        fgets(buffer, 1024, file);
        while(strcmp(fgets(buffer, 1024, file), "░\n") != 0)
            strcat(pages[nbPage].contentText.contentText, buffer);
        fgets(pages[nbPage].pathImage.pathImage, 1024, file);
        fgets(buffer, 1024, file);
        for(int nbLink = 0;  fgets(buffer, 1024, file) != NULL; ++nbLink)
            strcpy(pages[nbPage].link.link[nbLink], buffer);

        fclose(file);
    }
    closedir(dr);

    return 0;
} 
