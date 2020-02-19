Virtual CPU based on unicorn for Android armeabi-v7a.

#### Description

 * fridobot : test scripts;
 * include : c/c++ header for UVM API;
 * sample : demo for UVM API;
 * solib : UnicornVM's andriod armeabi-v7a runtime;

```
VCAPI long vc_run_interp(const void *fn, const vc_context_t *ctx);

Run function 'fn' on our VCPU with 'ctx', the return value is in r0 which is the fn's return value.
```

```
VCAPI const void *vc_make_callee(const void *fn, void *usrctx, fn_vc_callback_t callback);

Make a wrapper for function 'fn' with 'usrctx','callback', the return value is a new function pointer which will run under our VCPU;

You can replace this pointer to process's function pointer like C++-Vtable/Script-Native-Bridge.
```

Follow us for update or bug report:

|Platform|Account|QRCode|
|-|-|-|
|Email|971159199@qq.com|.|
|公众号|刘柏江|![gongzhonghao](https://gitee.com/geekneo/PantaDocumentRes/raw/master/wemedia/gongzhonghao.jpeg)|
|百家号|刘柏江VM|![baijiahao](https://gitee.com/geekneo/PantaDocumentRes/raw/master/wemedia/baijiahao.jpeg)|
|微博|刘柏江VM|![weibo](https://gitee.com/geekneo/PantaDocumentRes/raw/master/wemedia/weibo.jpeg)|
|头条|刘柏江|![toutiao](https://gitee.com/geekneo/PantaDocumentRes/raw/master/wemedia/toutiao.jpeg)|
|码云|https://gitee.com/geekneo/|.|
