# 1 "test/t.c"
# 16 "test/t.c"
# 1 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/dlfcn.h" 1
# 37 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/dlfcn.h"
# 1 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/cdefs.h" 1
# 608 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/cdefs.h"
# 1 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_symbol_aliasing.h" 1
# 609 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/cdefs.h" 2
# 674 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/cdefs.h"
# 1 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/_posix_availability.h" 1
# 675 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/sys/cdefs.h" 2
# 38 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/dlfcn.h" 2


# 1 "/usr/local/Cellar/tcc/0.9.27_1/lib/tcc/include/stdbool.h" 1
# 41 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/dlfcn.h" 2
# 1 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/Availability.h" 1
# 242 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/Availability.h"
# 1 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/AvailabilityInternal.h" 1
# 243 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/Availability.h" 2
# 42 "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/dlfcn.h" 2



typedef struct dl_info {
        const char      *dli_fname;
        void            *dli_fbase;
        const char      *dli_sname;
        void            *dli_saddr;
} Dl_info;

extern int dladdr(const void *, Dl_info *);


extern int dlclose(void * __handle);
extern char * dlerror(void);
extern void * dlopen(const char * __path, int __mode);
extern void * dlsym(void * __handle, const char * __symbol);


extern _Bool dlopen_preflight(const char* __path) ;
# 17 "test/t.c" 2

static void * dl_main;
inline static void*  dl_sym(const char* name)
{
                    void* sym = dlsym(dl_main, name);
                    return sym;
}

void dfn(double d) {
}

int main() {


	union val { double d; int u; int u8; };
	union val v;

	v.d = 10.1;

	dfn(*(double *)(&v));

	void *vptr = &v.d;
	dfn(*(double *)vptr);

	char *chptr = &v.d;
	dfn(*(double *)chptr);

    printf("\nc sym %u\n",&printf);

	printf("\n%s\n","xxxxasdfasdfxx");




    dl_main = dlopen(0, 1);

    int (*logf)(char *, ...);

    logf = dl_sym("printf");
    (*logf)("\nc printf sym %u\n",logf);
    (*logf)("\nc tcc_run sym %u\n",dl_sym("tcc_run"));
    (*logf)("\nc tcc_run sym %u\n",dl_sym("mmap"));



	v.u = 123;






}
