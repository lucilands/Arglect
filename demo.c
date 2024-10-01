#include <stdio.h>

#define ARGLECT_UTILS
#define ARGLECT_IMPLEMENTATION
#include "arglect.h"


arglect_Arg args[] = {
    ARGLECT_ARG("help", 'h', "Output this menu and exit", ARGLECT_BOOL)
};

int main(int argc, char **argv) {
    int res = arglect(args, sizeof(args) / sizeof(arglect_Arg), argc, argv);
    if (res) arglect_error(res, argv);

    if (ARGLECT_GET_ARG_ARR(args, "help")->value) arglect_help(args, sizeof(args) / sizeof(arglect_Arg), argv[0]);
    return 0;
}