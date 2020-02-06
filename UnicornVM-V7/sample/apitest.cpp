// UnicornVM Framework for ARMV7-A on Android platform.
//
// If you have any advises, We are happy to hear from you.
// Follow us:
// ----------------------------------------------------------------------
//         Email                           971159199@qq.com
//         公众号                           刘柏江
//         百家号                           刘柏江VM
//
//         微博                             刘柏江VM
//         头条                             刘柏江
//         码云                             https://gitee.com/geekneo/
// ----------------------------------------------------------------------
//
//
// Version history:
//
// 2020.02.06 {
//         * initialize UnicornVM api test code
// }

#include <iostream>
#include <map>
#include <set>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "../include/UnicornVM.h"

// log runtime information
static vc_callback_return_t interp_callback_log(vc_callback_args_t *args) {
    switch (args->op) {
    case vcop_read:
    case vcop_write: {
        break;
    }
    case vcop_call: {
        printf("vcop call : func %p.\n", args->info.call.callee);
        break;
    }
    case vcop_return: {
        printf("vcop return : hit address %p.\n", args->info.ret.hitaddr);
        break;
    }
    case vcop_svc: {
        printf("vcop syscall : syscall number %d.\n", args->info.svc.sysno);
        break;
    }
    default: {
        printf("unknown vcop %d.\n", args->op);
        break;
    }
    }
    // return cbret_directcall;
    return cbret_recursive;
}

// do nothing
static vc_callback_return_t interp_callback_nop(vc_callback_args_t *args) {
    return cbret_continue;
}

static int numbers[] = {2, 0, 2, 0, 2, 2, 0, 20, 20, 1, 199, 100000000};

// interpretee
static int print_message(const char *reason, const char **argv,
                         fn_vc_callback_t cb) {
    std::vector<std::string> svec;
    std::set<std::string> sset;
    std::map<std::string, std::string> smap;
    std::vector<int> ivec;
    std::set<int> iset;
    std::map<int, int> imap;

    std::string hi("hello, ");
    hi += argv[0];
    std::cout << reason << " : " << hi << std::endl;
    printf("printf, %s %s %c %d %p %d\n", reason, argv[0], '$', strlen(argv[0]),
           print_message, hi.length());
    
    puts("numbers: ");
    for (size_t i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
        char si[16], sv[16];
        sprintf(si, "%d", i);
        sprintf(sv, "%d", numbers[i]);
        svec.push_back(si);
        sset.insert(si);
        smap.insert(std::make_pair(si, sv));
        ivec.push_back(i);
        iset.insert(i);
        imap.insert(std::make_pair(i, numbers[i]));
        switch (numbers[i]) {
        case 0: {
            printf(" %d", numbers[i] + 1);
            break;
        }
        case 1: {
            printf(" %d", numbers[i] + 2);
            break;
        }
        default: {
            if (numbers[i] > 10) {
                printf(" %d", numbers[i]);
            } else {
                printf(" %d", numbers[i] + 3);
            }
            break;
        }
        }
    }
    putchar('\n');

    std::cout << "string vector : ";
    for (size_t i = 0; i < svec.size(); i++) {
        std::cout << svec[i] << " ";
    }
    putchar('\n');

    std::cout << "integer vector : ";
    for (auto i : ivec) {
        std::cout << i << " ";
    }
    putchar('\n');

    std::cout << "string set : " << *sset.find("3") << " ";
    for (auto &s : sset) {
        std::cout << s << " ";
    }
    putchar('\n');

    std::cout << "integer set : " << *iset.find(3) << " ";
    for (auto i : sset) {
        std::cout << i << " ";
    }
    putchar('\n');

    std::cout << "string map : " << smap.find("3")->second << " ";
    for (auto &s : smap) {
        std::cout << "<" << s.first << ", " << s.second << "> ";
    }
    putchar('\n');

    std::cout << "integer map : " << imap.find(3)->second << " ";
    for (auto &i : imap) {
        std::cout << "<" << i.first << ", " << i.second << "> ";
    }
    putchar('\n');
    fflush(stdout);
    return strlen(argv[0]);
}

// run interpretee directly
int vrun_print_message(const char *reason, const char **argv,
                       fn_vc_callback_t cb) {
    vc_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.callback = cb;
    ctx.regctx.r[0].p = reason;
    ctx.regctx.r[1].p = (void *)argv;
    return (int)(long)vc_run_interp((void *)print_message, &ctx);
}

// run interpretee with a wrapper
int wrapper_print_message(const char *reason, const char **argv,
                          fn_vc_callback_t cb) {
    const void *fnptr = vc_make_callee((void *)print_message, nullptr, cb);
    return ((int (*)(const char *, const char **))fnptr)(reason, argv);
}

int main(int argc, const char *argv[]) {
    printf("UnicornVM test program, pid is %d.\n", getpid());
    struct {
        const char *reason;
        int (*func)(const char *, const char **, fn_vc_callback_t);
        fn_vc_callback_t interpcb;
    } testor[] = {
        {
            "direct",
            print_message,
            nullptr,
        },
        {
            "vrun0",
            vrun_print_message,
            nullptr,
        },
        {
            "vrun1",
            vrun_print_message,
            interp_callback_nop,
        },
        {
            "wrapper0",
            wrapper_print_message,
            interp_callback_nop,
        },
        {
            "wrapper1",
            wrapper_print_message,
            interp_callback_log,
        },
    };
    for (size_t i = 0; i < sizeof(testor) / sizeof(testor[0]); i++) {
        printf("////// index %d //////\n", i);
        fflush(stdout);
        printf("////// result %d //////\n\n",
               testor[i].func(testor[i].reason, argv, testor[i].interpcb));
        fflush(stdout);
    }
    return 0;
}
