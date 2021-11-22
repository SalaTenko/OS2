#pragma once

void my_help();
void execute_loop();
char** get_tokens(char* str, int* tokens_count);
void execute_one_command(char** tokens_list);
void free_tokens_list(char** tokens_list, int tokens_count);