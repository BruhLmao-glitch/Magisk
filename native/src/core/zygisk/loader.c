#include <dlfcn.h>
#include <android/dlext.h>

#if defined(__LP64__)
# define LP_SELECT(lp32, lp64) lp64
#else
# define LP_SELECT(lp32, lp64) lp32
#endif

__attribute__((constructor))
static void zygisk_loader(void) {
    void *handle = dlopen("/system/lib" LP_SELECT("", "64") "/libzygisk.so", RTLD_LAZY);
    if (handle) {
        void(*entry)(void*) = dlsym(handle, "zygisk_inject_entry");
        if (entry) {
            entry(handle);
        }
        void (*unload)(void) = dlsym(handle, "unload_loader");
        if (unload) {
            __attribute__((musttail)) return unload();
        }
    }
}
	
