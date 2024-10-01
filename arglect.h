#ifndef _ARGLECT_H
#define _ARGLECT_H


#define ARGLECT_ARG(lngnm, shrtnm, descr, type) (arglect_Arg) {lngnm, shrtnm, descr, NULL, 0, type}
#define ARGLECT_GET_ARG_ARR(arr, name) arglect_get_arg(arr, sizeof(arr) / sizeof(arr[0]), name, 0)

typedef enum {
    ARGLECT_BOOL = 0,
    ARGLECT_INT,
    ARGLECT_STR
} arglect_ArgType;

typedef struct arglect_Arg {
    const char *longname;
    const char shortname;

    const char *descr;

    void *value;
    int __tmp; // THIS IS A HACK, PLEASE REMOVE AS QUICKLY AS POSSIBLE
    const arglect_ArgType type;
} arglect_Arg;

int arglect(arglect_Arg *args, unsigned int args_len, int argc, char **argv);
arglect_Arg *arglect_get_arg(arglect_Arg *const args, unsigned int args_len, const char *name, const int is_shortname);


#ifdef ARGLECT_UTILS
const char *arglect_get_err(int code, char **argv);
void arglect_error(int code, char **argv);
void arglect_help(const arglect_Arg *args, unsigned int args_len, const char *program);
#endif //ARGLECT_UTILS

#ifdef ARGLECT_IMPLEMENTATION
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef ARGLECT_UTILS
#include <stdio.h>
void arglect_error(int code, char **argv) {
    const char *msg = arglect_get_err(code, argv);
    printf("%s\n", msg);
    free((void*)msg);
    exit(code);
}
const char *arglect_get_err(int code, char **argv) {
    switch (code) {
        case 9:
            return "Unreachable";
        default:
            if (code >= 10) {
                char *ret = malloc(snprintf(NULL, 0, "Invalid argument \"%s\"", argv[code - 10]));
                sprintf(ret, "Invalid argument \"%s\"", argv[code - 10]);
                return ret;
            }
            break;
    }
    return "No Error";
}
void arglect_help(const arglect_Arg *args, unsigned int args_len, const char *program) {
    printf("Usage:\n\t%s\t<flags>\n", program);
    printf("Flags:\n");
    for (unsigned int i = 0; i < args_len; i++) {
        printf("\t--%s -%c\t%s\n", args[i].longname, args[i].shortname, args[i].descr);
    }
}
#endif //ARGLECT_UTILS

arglect_Arg *arglect_get_arg(arglect_Arg *const args, unsigned int args_len, const char *name, const int is_shortname) {
    if (is_shortname) {
        for (unsigned int i = 0; i < args_len; i++) {
            if (name[0] == args[i].shortname) return &(args[i]);
        }
    }
    else {
        for (unsigned int i = 0; i < args_len; i++) {
            if (strcmp(args[i].longname, name) == 0) return &(args[i]);
        }
    }
    return NULL;
}

int arglect(arglect_Arg *args, unsigned int args_len, int argc, char **argv) {
    for (unsigned int i = 0; i < (unsigned int)argc; i++) {
        const char *arg = argv[i];
        if (arg[0] == '-') {
            arglect_Arg *argnm = NULL;
            if (arg[1] == '-') argnm = arglect_get_arg((arglect_Arg *const)args, args_len, arg+2, false);
            else argnm = arglect_get_arg((arglect_Arg *const)args, args_len, arg+1, true);

            if (argnm == NULL) return 10 + i;

            arglect_ArgType type = argnm->type;
            switch (type) {
                case ARGLECT_BOOL:
                    argnm->value = (bool*)true;
                    return 0; // Success
                case ARGLECT_INT:
                    argnm->__tmp = atoi(argv[++i]);
                    argnm->value = (void*)&(argnm->__tmp);
                    return 0; // Success
                case ARGLECT_STR:
                    argnm->value = (void*)(argv[++i]);
                    return 0; // Success
                default: return 9; // Unreachable
            }
        }
    }
    return 0; // SUCCES
}
#endif //ARGLECT_IMPLEMENTATION
#endif //_ARGLECT_H