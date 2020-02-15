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
// 2019.12.15 {
//         * initialize UnicornVM SDK
// }

#ifndef __UNICORNVM_ARMV7__
#define __UNICORNVM_ARMV7__

#ifdef __cplusplus
extern "C" {
#endif

#define VCAPI __attribute__((visibility("default")))

// arm common register like r0 r1 ... lr sp pc
typedef union vc_cmmreg_t {
    unsigned int w;
    int sw;
    const void *p;
    const char *s;
} vc_cmmreg_t;

// arm neon register like s0 d0 ...
typedef union vc_neonreg_t {
    unsigned int i[2];
    unsigned long long l;
    int si[2];
    long long sl;
    float f[2];
    double d;
} vc_neonreg_t;

// special register wrapper
#define ARM_FP(c) (c).r[12]
#define ARM_SP(c) (c).r[13]
#define ARM_LR(c) (c).r[14]
#define ARM_PC(c) (c).pc

// arm execution context
typedef struct vc_armctx_t {
    vc_cmmreg_t r[16]; // 0-12, 13-sp, 14-lr, 15-reserved
    vc_neonreg_t v[32];
    vc_cmmreg_t pc;
} vc_armctx_t;

// opcode type for callback args
typedef enum vc_optype_t {
    vcop_read,   // memory read
    vcop_write,  // memory write
    vcop_call,   // function call
    vcop_return, // function return
    vcop_svc,    // arm syscall
} vc_optype_t;

// callback args
typedef struct vc_callback_args_t {
    // your own context passed for vc_run_interp/vc_make_callee
    const void *usrctx;
    // arm execution context
    vc_armctx_t *armctx;
    // current opcode
    vc_optype_t op;
    union {
        // for vcop_read/vcop_write
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
            const void *hitaddr; // which address hit return
        } ret;
        // for vcop_svc
        struct {
            // parameters are in armctx->r[0...6]
            int sysno; // syscall number from armctx->r[7]
        } svc;
    } info;
} vc_callback_args_t;

// callback return type
typedef enum vc_callback_return_t {
    cbret_continue,   // let interp continue
    cbret_processed,  // already processed by callback implementation
    cbret_recursive,  // interp this function recursively
    cbret_directcall, // call this function directly
} vc_callback_return_t;

// callback prototype
typedef vc_callback_return_t (*fn_vc_callback_t)(vc_callback_args_t *args);

// interpreter context
typedef struct vc_context_t {
    void *usrctx;
    vc_armctx_t regctx;
    fn_vc_callback_t callback;
} vc_context_t;

// this api is for VirtualCodeTool and manually call
//
// run function 'fn' on our VCPU with 'ctx'
// return value is r0
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

#endif // end of __UNICORNVM_ARMV7__
