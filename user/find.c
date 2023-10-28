#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


char*
getname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void 
find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){
        case T_DEVICE:
        case T_FILE:
            if (strcmp(getname(path), filename) == 0)
                printf("%s", path);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (strcmp(getname(buf), ".") == 0 || strcmp(getname(buf), "..") == 0) 
                    continue;
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                if (st.type == T_FILE && strcmp(getname(buf), filename) == 0) {
                    printf("%s\n", buf);
                } else if (st.type == T_DIR) {
                    find(buf, filename);
                }
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
  int i;
  if(argc < 2) {
    fprintf(2, "Usage: find path filename\n");
  } else if(argc == 2) {
    find(".", argv[2]);
    exit(0);
  }
  for(i=1; i<argc-1; i++)
    find(argv[i], argv[argc - 1]);
  exit(0);
}
