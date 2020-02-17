After building unicorn with ios_clang, you'll get a crash when executing uc_emu_start.
So, what the hell ?
 * 1.qemu uses malloc to allocate the dynamic executable page for 0x800000 bytes, but pages which are allocated by malloc can't be executable by default in iOS. You should patch qemu to use vm_allocate mach API;
 * 2.If the page is writable, then it can't be executable in iOS. You should mprotect it to PROT_READ|PROT_EXEC;

OK, if you notice these limits, the solution is simple, wish you to solve this crash issue by yourself. Have fun with unicorn.