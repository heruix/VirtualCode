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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include <sys/mman.h>

#include "../../../Common/PLTHooker/src/elf_hooker.h"
#include "../../include/UnicornVM.h"

#define logger(...)                                                            \
    __android_log_print(ANDROID_LOG_DEBUG, "Fridobot", __VA_ARGS__)

// simple implementation for module filter
static bool plt_prehook(const char *module_name, const char *func_name) {
    logger("Hijacking %s plt %s.", module_name, func_name);
    return true;
}

// do nothing as we'll use recursive mode to interpret main function
static vc_callback_return_t interp_callback_nop(vc_callback_args_t *args) {
    return cbret_continue;
}

// hijack __libc_init in plt and replace main function to UnicornVM's implementation
static void *hijack_libc_init_plt = NULL;
static void hijack_libc_init(void *arg0, void *arg1, const void *fnmain) {
    const void *vmmain = vc_make_callee(fnmain, NULL, interp_callback_nop);
    logger(
        "Replace main function %p to UnicornVM's %p, running with arm interpreter...", 
        fnmain, vmmain);
    ((void (*)(void *, void *, const void *))hijack_libc_init_plt)(arg0, arg1, vmmain);
}

// invoke plt hooker
static void __attribute__((constructor)) __init__() {
    elf_hooker hooker;
    hooker.set_prehook_cb(plt_prehook);
    hooker.phrase_proc_maps();
    hooker.hook_all_modules("__libc_init", (void *)hijack_libc_init, &hijack_libc_init_plt);
}
