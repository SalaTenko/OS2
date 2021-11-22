#include "../include/foo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char **get_tokens(char *str, int *tokens_count)
{
    int number_of_malloc_tokens = 3;
    char *token = malloc(sizeof(*token) * 255);
    char **tmp = malloc(sizeof(*tmp) * number_of_malloc_tokens);

    for (int i = 0, token_len = 0; i < strlen(str); i++)
    {
        if (str[i] == '\n' || str[i] == '\t' || str[i] == ' ')
        {
            token[token_len] = '\0';
            if ((*tokens_count) == number_of_malloc_tokens)
            {
                number_of_malloc_tokens += 3;
                tmp = realloc(tmp, number_of_malloc_tokens * sizeof(*tmp));
            }
            tmp[(*tokens_count)] = malloc(sizeof(**tmp) * token_len);
            strcpy(tmp[(*tokens_count)], token);
            (*tokens_count) += 1, token_len = 0;
        }
        else
        {
            token[token_len++] = str[i];
        }
    }
    free(token);
    return tmp;
}
void execute_one_command(char **tokens_list)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(tokens_list[0], tokens_list);
        exit(EXIT_SUCCESS);
    }
    else if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        do
        {
            wait(&status);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

char **find_sign_of_pipe(char **tokens_list, int tokens_count)
{
    for (int i = 0; i < tokens_count; i++)
    {
        if (!strcmp(tokens_list[i], "|"))
        {
            char **tmp = malloc(sizeof(*tmp) * (tokens_count - i++));
            for (int j = 0; i < tokens_count; j++, i++)
            {
                tmp[j] = tokens_list[i];
            }
            return tmp;
        }
    }
    return NULL;
}
void execute_loop()
{
    char *str = malloc(sizeof(*str) * 255);

    printf("\nYou run my_bash and this is so good!");
    printf("\nIf you wanna go out just enter 'quit' or 'q'.\nPlease enter your commands:\n\n");

    do
    {
        printf("> ");
        int tokens_count = 0;
        fgets(str, 256, stdin);

        char **tokens_list = get_tokens(str, &tokens_count);
        char **second_list = find_sign_of_pipe(tokens_list, tokens_count);

        if (second_list == NULL)
            execute_one_command(tokens_list);
        else
        {
            printf("%s\n", second_list[0]);
            // for (int i = 0; i < sizeof(second_list) / sizeof(second_list[0]); i++) {
            //     printf("%s\n", second_list[i]);
            // }
        }

        free_tokens_list(tokens_list, tokens_count);
    } while (strcmp(str, "quit\n") && strcmp(str, "q\n"));
    printf("\n\nThank you for your time!\nSee you later!\n\n");
    free(str);
}

void free_tokens_list(char **tokens_list, int tokens_count)
{
    for (int i = 0; i < tokens_count; i++)
    {
        free(tokens_list[i]);
    }
    free(tokens_list);
}
int get_screen_size(int *rows, int *cols)
{
    struct winsize w;
    if (!ioctl(1, TIOCGWINSZ, &w))
    {
        *rows = w.ws_row;
        *cols = w.ws_col;
    }
    else
        return 1;
    return 0;
}
void my_help()
{
    char head[256] = {"Welcome in bash cell for execute linux commands "};
    int rows, cols;
    get_screen_size(&rows, &cols);
    int n_border = cols - strlen(head) + 1;
    int n_half = abs((n_border / 2));
    printf("\n");
    for (int i = 0; i < n_border; i++)
    {
        if (n_half == i)
        {
            printf("%s", head);
        }
        else
        {
            printf("=");
        }
    }
    printf("\n\nUSAGE EXAMPLES:\n");
    printf("\t./my_bash\n");
    printf("\t> ls\n\n");
    printf("\t./my_bash\n");
    printf("\t> ls | grep 'test'\n\n");
    printf("\t./my_bash help\n\n");
    for (int i = 0; i < cols; i++)
    {
        printf("=");
    }
    printf("\n\n");
}