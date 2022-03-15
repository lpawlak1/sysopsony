#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ftw.h>
#include <time.h>
#include <dirent.h>


// Powinny zostać zliczone:
// zwykłe pliki,
// katalogi,
// pliki specjalne znakowe,
// pliki specjalne blokowe,
// potoki/kolejki FIFO,
// linki symboliczne i sokety. 
int zwykle_pliki = 0, katalogi = 0, znakowe = 0, blokowe = 0, fifo = 0, linki = 0, soket = 0;

char* better_ctime(const __time_t* essa){ // without sigsegv i guess
    char *buff = calloc(sizeof(char), 20);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(essa));
    return buff;
}

#ifdef NFTW
char* what_is_this(const struct stat* sb){
    //zwykły plik - file
    if (S_ISREG(sb->st_mode)){
        zwykle_pliki++;
        return "file";}
    // katalog - dir
    if (S_ISDIR(sb->st_mode)){
        katalogi++;
        return "dir";}
    // urządzenie znakowe - char dev
    if (S_ISCHR(sb->st_mode)){
        znakowe++;
        return "char dev";}
    // urządzenie blokowe - block dev
    if (S_ISBLK(sb->st_mode)){
        blokowe++;
        return "block dev";}
    // potok nazwany - fifo
    if (S_ISFIFO(sb->st_mode)){
        fifo++;
        return "fifo";}
    // link symboliczny - slink
    if (S_ISLNK(sb->st_mode)){
        linki++;
        return "slink";}
    // soket - sock),
    if (S_ISSOCK(sb->st_mode)){
        soket++;
        return "sock";}
    return "who cares :(";
}

static int print_thingy(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf)
{
    char* result = realpath(fpath, NULL);

    //Ścieżka bezwzględna pliku,
    printf("At: %s", result);
    //Liczbę dowiązań,
    printf(",%lu ", sb->st_nlink);
    //Rodzaj pliku (zwykły plik - file, katalog - dir, urządzenie znakowe - char dev, urządzenie blokowe - block dev, potok nazwany - fifo, link symboliczny - slink, soket - sock),
    char* nazwa = what_is_this(sb);
    printf(",%s", nazwa);
    // Rozmiar w bajtach,
    printf(",%ld", sb->st_size);
    // Datę ostatniego dostępu, // tą część wziąłem (żeby brzydziej nie powiedzieć) z https://c-for-dummies.com/blog/?p=3004
    // niestety nie wiem o co chodzi ale nanosekund nie dostaane :(
    printf(", File access time %s", better_ctime(&sb->st_atime));
    // Datę ostatniej modyfikacji.
    printf(", File modify time %s\n",
           better_ctime(&sb->st_mtime)
    );
//    printf(", File changed time %s",
//           better_ctime(&sb->st_ctime)
//    );
    return 0;
}

#endif

#ifdef READDIR
char* what_is_this_mate(unsigned char sg){
    //zwykły plik - file
    //DT_REG This is a regular file.
    if (sg == DT_REG){
        zwykle_pliki++;
        return "file";}
    // katalog - dir
    // DT_DIR This is a directory.
    if (DT_DIR == sg){
        katalogi++;
        return "dir";}
    // urządzenie znakowe - char dev
    // DT_CHR This is a character device.
    if (DT_CHR == sg){
        znakowe++;
        return "char dev";}
    // urządzenie blokowe - block dev
    //DT_BLK This is a block device.
    if (DT_BLK == sg){
        blokowe++;
        return "block dev";}
    // potok nazwany - fifo
    //DT_FIFO
    //This is a named pipe (FIFO).
    if (DT_FIFO == sg){
        fifo++;
        return "fifo";}
    // link symboliczny - slink
    // DT_LNK This is a symbolic link.
    if (DT_LNK == sg){
        linki++;
        return "slink";}
    // soket - sock),
    //DT_SOCK
    //This is a UNIX domain socket.
    if (DT_SOCK == sg){
        soket++;
        return "sock";}
    return "who cares :(";


    // who cares xD
    // DT_UNKNOWN
    // The file type could not be determined.




}

struct stat sb;

void myfilerecursive(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            stat(dp->d_name, &sb);

            char* result = realpath(path, NULL);

            //Ścieżka bezwzględna pliku,
            printf("At: %s", result);
            //Liczbę dowiązań,
            printf(",%lu ", sb.st_nlink);
            //Rodzaj pliku (zwykły plik - file, katalog - dir, urządzenie znakowe - char dev, urządzenie blokowe - block dev, potok nazwany - fifo, link symboliczny - slink, soket - sock),
            char* nazwa = what_is_this_mate(dp->d_type);
            printf(",%s", nazwa);
            // Rozmiar w bajtach,
            printf(",%ld", sb.st_size);
            // Datę ostatniego dostępu, // tą część wziąłem (żeby brzydziej nie powiedzieć) z https://c-for-dummies.com/blog/?p=3004
            // niestety nie wiem o co chodzi ale nanosekund nie dostaane :(
            printf(", File access time %s", better_ctime(&sb.st_atime));
            // Datę ostatniej modyfikacji.
            printf(", File modify time %s\n",
                   better_ctime(&sb.st_mtime)
            );



            myfilerecursive(path);
        }
    }

    closedir(dir);
}

#endif

int main(int argc, char* argv[]){
    if (argc <= 1){
        return 1;
    }
    #ifdef NFTW
        nftw(argv[1], print_thingy, 50, FTW_PHYS);
        katalogi--; // bo nftw zaczyna of katalogu który mu sie poda XD
    #endif
    #ifdef READDIR
        myfilerecursive(argv[1]);
    #endif

    printf("zwykłe pliki %d,",zwykle_pliki);
    printf("katalogi %d,",katalogi );
    printf("pliki specjalne znakowe %d,",znakowe );
    printf("pliki specjalne blokowe %d,",blokowe );
    printf("potoki/kolejki FIFO %d,",fifo );
    printf("linki symboliczne %d i sokety %d.\n", linki, soket );
}
