#include "../include/foo.h"

int main(int argc, char* argv[])
{
    if (argc == 1) {
        execute_loop();
    } else {
        my_help();
    }
    return 0;
}