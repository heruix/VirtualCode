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

#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../../../Common/ASMHooker/whale/whale/include/whale.h"
#include "../../../Common/PLTHooker/src/elf_hooker.h"
#include "../../include/UnicornVM.h"

#define logger(...) \
  __android_log_print(ANDROID_LOG_INFO, "Fridobot", __VA_ARGS__)

#define USE_ASMHOOKER 1
#define USE_PLTHOOKER !USE_ASMHOOKER

#if USE_PLTHOOKER
// simple implementation for module filter
static bool plt_prehook(const char *module_name, const char *func_name) {
  size_t mnlen = strlen(module_name);
  if (strncmp(module_name + mnlen - 3, ".so", 3) == 0) {
    return false;
  }

  logger("Hijacking %s plt %s.", module_name, func_name);
  return true;
}
#endif  // end of USE_PLTHOOKER

// do nothing as we'll use recursive mode to interpret main function
static vc_callback_return_t interp_callback_nop(vc_callback_args_t *args) {
  static bool hitted = false;
  if (!hitted) {
    hitted = true;
    logger("Running UnicormVM-V7 android arm interpreter ...");
  }
  return cbret_continue;
}

#if USE_ASMHOOKER
extern "C" void *__libc_init(void *arg0, void *arg1, const void *fnmain,
                             void *arg3, void *arg4, void *arg5, void *arg6,
                             void *arg7);
#endif  // end of USE_ASMHOOKER

// hijack __libc_init in plt and replace main function to UnicornVM's
// implementation
static void *hijack_libc_init_orig = NULL;
static void *hijack_libc_init(void *arg0, void *arg1, const void *fnmain,
                              void *arg3, void *arg4, void *arg5, void *arg6,
                              void *arg7) {
  const void *vmmain = vc_make_callee(fnmain, NULL, interp_callback_nop);
  logger("Will replace main function %p to UnicornVM's %p.", fnmain, vmmain);
  return ((void *(*)(void *, void *, const void *, void *, void *, void *,
                     void *, void *))hijack_libc_init_orig)(
      arg0, arg1, vmmain, arg3, arg4, arg5, arg6, arg7);
}

// invoke plt hooker
static void __attribute__((constructor)) __init__() {
  if (getenv("HIJACK_NOP")) {
    logger("Ignoring hijack main for normal running.");
    return;
  }
  if (getenv("SOFT_PAUSE")) {
    for (int i = 0; i < 10; i++) {
      logger("Waiting for debugger...");
      sleep(2);
    }
  }
  logger("Running hijacked program.");

#if USE_PLTHOOKER
  elf_hooker hooker;
  hooker.set_prehook_cb(plt_prehook);
  hooker.phrase_proc_maps();
  hooker.hook_all_modules("__libc_init", (void *)hijack_libc_init,
                          &hijack_libc_init_orig);
#else
  WInlineHookFunction((void *)__libc_init, (void *)hijack_libc_init,
                      &hijack_libc_init_orig);
#endif
}
