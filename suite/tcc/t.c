
#define _GNU_SOURCE 1

// http://www.musl-libc.org/how.html
// https://andrewkelley.me/post/why-donating-to-musl-libc-project.html

// time tcc -m64 -w -run /usr/local/d4/aether/i/app/sh/cc/t.c

// /usr/local/d4/aether/i/app/pkg/aot/tcc/tccios/tcc -w -I/usr/local/include -I`xcrun --sdk macosx --show-sdk-path` -nostdinc -run /usr/local/d4/aether/i/app/sh/cc/t.c

// tcc -m64  `pkg-config --cflags --libs glib-2.0` -I`gcc --print-file-name=include` -I`gcc --print-file-name=include-fixed` -I`gcc --print-file-name=install-tools/include`  -run   /tmp/t.c

// #include <stdio.h>
// #include <glib/gstdio.h>


// #include        <inttypes.h>

#include        <dlfcn.h>
#define NIL (void *)0
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
    // printf("\nc sym %u\n",&mmap);
	printf("\n%s\n","xxxxasdfasdfxx");
    // printf("\n%u\n",RTLD_LAZY);

    // (int (*fn))(const char *restrict, ...);
    #define __RTLD_LAZY 1
    dl_main = dlopen(0, __RTLD_LAZY);
    // dl_main = NIL;
    int (*logf)(char *, ...);

    logf = dl_sym("printf");
    (*logf)("\nc printf sym %u\n",logf);
    (*logf)("\nc tcc_run sym %u\n",dl_sym("tcc_run"));
    (*logf)("\nc tcc_run sym %u\n",dl_sym("mmap"));



	v.u = 123;






}

