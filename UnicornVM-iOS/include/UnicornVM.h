// UnicornVM Framework for ARM64 on iOS platform.
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
// 2020.03.03 {
//         * add vcop_ifetch for callback
// }
//
// 2020.02.15 {
//         * initialize UnicornVM SDK
// }

#ifndef __UNICORNVM_ARM64_H__
#define __UNICORNVM_ARM64_H__

#ifdef __cplusplus
extern "C" {
#endif

#define VCAPI __attribute__((visibility("default")))

// arm64 common register like x0 x1 ... lr sp pc
typedef union vc_cmmreg_t {
  unsigned int w;
  unsigned long long x;
  int sw;
  long long sx;
  const void *p;
  const char *s;
  unsigned int ws[2];
  int sws[2];
} vc_cmmreg_t;

// arm64 neon register like s0 d0 q0 ...
typedef union vc_neonreg_t {
  unsigned int i[4];
  unsigned long long l[2];
  int si[4];
  long long sl[2];
} vc_neonreg_t;

// special register wrapper
#define ARM64_FP(c) (c).r[29]
#define ARM64_LR(c) (c).r[30]
#define ARM64_SP(c) (c).r[31]
#define ARM64_PC(c) (c).pc

// arm64 execution context
typedef struct vc_arm64ctx_t {
  vc_cmmreg_t r[32];  // 0-28,29-fp,30-lr,31-sp
  vc_neonreg_t v[32];
  vc_cmmreg_t pc;
} vc_arm64ctx_t;

// opcode type for callback args
typedef enum vc_optype_t {
  vcop_read,    // memory read
  vcop_write,   // memory write
  vcop_call,    // function call
  vcop_return,  // function return
  vcop_svc,     // arm64 syscall
  vcop_ifetch,  // interpreter fetch instruction
} vc_optype_t;

// callback args
typedef struct vc_callback_args_t {
  // your own context passed for vc_run_interp/vc_make_callee
  const void *usrctx;
  // arm64 execution context
  vc_arm64ctx_t *arm64ctx;
  // current opcode
  vc_optype_t op;
  union {
    // for vcop_read/vcop_write/vcop_ifetch
    struct {
      const void *src;
      void *dst;
      int byte;
    } rw;
    // for vcop_call
    struct {
      const void *callee;
    } call;
    // for vcop_return
    struct {
      const void *hitaddr;  // which address hit return
    } ret;
    // for vcop_svc
    struct {
      int sysno;  // syscall number
    } svc;
  } info;
} vc_callback_args_t;

// callback return type
typedef enum vc_callback_return_t {
  cbret_continue,    // let interp continue
  cbret_processed,   // already processed by callback implementation
  cbret_recursive,   // interp this function recursively
  cbret_directcall,  // call this function directly
} vc_callback_return_t;

// callback prototype
typedef vc_callback_return_t (*fn_vc_callback_t)(vc_callback_args_t *args);

// interpreter context
typedef struct vc_context_t {
  void *usrctx;
  vc_arm64ctx_t regctx;
  fn_vc_callback_t callback;
} vc_context_t;

// this api is for VirtualCodeTool and manually call
//
// run function 'fn' on our VCPU with 'ctx'
// return value is x0
VCAPI long vc_run_interp(const void *fn, const vc_context_t *ctx);

// this api is used to make target's function pointer under your control
//
// make a wrapper for function 'fn' with 'usrctx','callback'
// return value is a new function pointer which will run under our VCPU
// you can replace this pointer to target's function pointer
// like C++-Vtable/Script-Native-Bridge
// if return null, you should check errno
VCAPI const void *vc_make_callee(const void *fn, void *usrctx,
                                 fn_vc_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif  // end of __UNICORNVM_ARM64_H__
