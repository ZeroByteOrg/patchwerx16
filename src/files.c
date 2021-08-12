#include "files.h"

// stub file for now as a place to copy/paste some interesting code examples
// from forum member rje.
//

#if(0)

// the following code posted to commanderx16.com forums by @rje:
//
// probably requires <cbm.h> - is that included automatically by cx16.h?
// I believe that it is....
//
unsigned char readFile(const char *filename)
{
    char s[200];
    unsigned char lfn = 1;
    unsigned char dev = 8;
    unsigned char sec_addr = 0;
    unsigned char res = cbm_open(lfn, dev, sec_addr, filename);
    if (res == 0)
    {
        cbm_read(lfn, s, 200);
        cbm_close(lfn);
        printf("data: %s\r\n", s);
    }

    return res;
}

unsigned char readDir(const char* path)
{
    struct cbm_dirent dir;
    unsigned char res = cbm_opendir(1, 8, path);
    printf("dir: [%s]\n", path);
    if (res == 0)
    {
        printf("listing for %s:\n", path);
        cbm_readdir(1, &dir); // disk header
        printf("file              blocks     type  access\n"
               "----              ------     ----  ------\n");
        while (cbm_readdir(1, &dir) == 0)
        {
            switch(dir.type)
            {
                case 2:  textcolor(8); break;
                case 17: textcolor(7); break;
            }
            cprintf("%-18s %5u     %3s   %u\r\n",
                dir.name,
                dir.size,
                dir.type == 2? "dir" : dir.type == 17? "prg" : "???",
                dir.access);
	            textcolor(1);
        }
        cbm_closedir(1);
    }
    return res;
}





#endif
