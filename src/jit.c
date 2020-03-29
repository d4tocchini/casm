
#include "libtcc.h"

#include <stdlib.h> // getenv
#include <stdio.h>
#include <string.h>

typedef TCCState
                jit_ctx;

#define         JIT_MAX_N_CTX 1024
static jit_ctx*        jit_ctx_vals[JIT_MAX_N_CTX];
static i32             jit_ctx_keys[JIT_MAX_N_CTX];
static i32             jit_ctx_count = 0;

static void     _jit_set_environment(jit_ctx *s);

jit_ctx*         jit_new(void)
{
                    return tcc_new();
}
void            jit_store(i32 key, jit_ctx* ctx)
{
                    jit_ctx_keys[ jit_ctx_count ] = key;
                    jit_ctx_vals[ jit_ctx_count ] = ctx;
                    ++jit_ctx_count;
}
jit_ctx*        jit_load(i32 key)
{
                    i32 i = 0;
                    while (i < jit_ctx_count) {
                        const i32 _key = jit_ctx_keys[i];
                        if (key == _key)
                            return jit_ctx_vals[i];
                        ++i;
                    }
                    jit_ctx* val = jit_new();
                    jit_store(key, val);
                    return val;
}
void            jit_delete(jit_ctx* ctx)
{
                    tcc_delete(ctx);
}
void            jit_trap(jit_ctx* ctx, void *error_opaque,
                        void (*error_func)(void *opaque, const char *msg))
{                   /* set error/warning display callback */
                    tcc_set_error_func(ctx, error_opaque, error_func);
}
void            jit_setopts(jit_ctx* ctx, const char *str)
{                   /* set options as from command line (multiple supported) */
                    tcc_set_options(ctx, str);
}
void            jit_setdefaults(jit_ctx* ctx)
{                   /* set options as from command line (multiple supported) */
                    tcc_set_options(ctx, "-m64 -w  -nostdlib "); //-m64 -g -b -nostdinc
}

// void    jit_setlpath
//         /* set CONFIG_TCCDIR at runtime */
// return tcc_set_lib_path(TCCState *s, const char *path);

/*****************************/
/* preprocessor */

int             jit_add_include_path(jit_ctx*ctx, const char *p)
{               /* add include path */
                    return tcc_add_include_path(ctx, p);
}
int             jit_add_sysinclude_path(jit_ctx *ctx, const char *p)
{               /* add in system include path */
                    return tcc_add_sysinclude_path(ctx, p);
}
void            jit_define(jit_ctx *ctx, const char *sym, const char *val)
{               /* define preprocessor symbol 'sym'. Can put optional value */
                    tcc_define_symbol(ctx, sym, val);
}
void            jit_undef(jit_ctx *ctx, const char *sym)
{               /* undefine preprocess symbol 'sym' */
                    tcc_undefine_symbol(ctx, sym);
}

/*****************************/
/* compiling */

int             jit_file(jit_ctx *ctx, const char *f)
{               /* add a file (C file, dll, object, library, ld script). Return -1 if error. */
                    return tcc_add_file(ctx, f);
}
int             jit_string(jit_ctx* ctx, const char *buf)
{               /* compile a string containing a C source. Return -1 if error. */
                    return tcc_compile_string(ctx, buf);
}


/*****************************/
/* linking commands */

/* set output type. MUST BE CALLED before any compilation */
// LIBTCCAPI int tcc_set_output_type(TCCState *s, int output_type);
// #define TCC_OUTPUT_MEMORY   1 /* output will be run in memory (default) */
// #define TCC_OUTPUT_EXE      2 /* executable file */
// #define TCC_OUTPUT_DLL      3 /* dynamic library */
// #define TCC_OUTPUT_OBJ      4 /* object file */
// #define TCC_OUTPUT_PREPROCESS 5 /* only preprocess (used internally) */


int             jit_setsym(jit_ctx* ctx, const char *name, const void *val)
{               /* add a symbol to the compiled program */
                    return tcc_add_symbol(ctx, name, val);
}
void*           jit_sym(jit_ctx* ctx, const char *name)
{               /* add a symbol to the compiled program */
                    return tcc_get_symbol(ctx, name);
}
int             jit_outfile(jit_ctx* ctx, const char *filename)
{               /* output an executable, library or object file. DO NOT call
   tcc_relocate() before. */
                    return tcc_output_file(ctx, filename);
}

LIBTCCAPI int tcc_output_file(TCCState *s, const char *filename);

int             jit_run(jit_ctx* ctx, int argc, char **argv)
{               /* link and run main() function and return its value.
                DO NOT call `tcc_relocate()` before. */
                    return tcc_run(ctx, argc, argv);
}
                // #define TCC_RELOCATE_AUTO (void*)1
int             jit_compile(jit_ctx* ctx)
{
                    if (tcc_relocate(ctx, TCC_RELOCATE_AUTO) < 0)
                        return 1;
                    return 0;
}
int             jit_relocate(jit_ctx* ctx, void* ptr)
{               /* do all relocations (needed before using tcc_get_symbol()) */
                /* possible values for 'ptr':
                    - TCC_RELOCATE_AUTO : Allocate and manage memory internally
                    - NULL              : return required memory size for the step below
                    - memory address    : copy code to memory passed by the caller
                    returns -1 if error. */
                    return tcc_relocate(ctx, ptr);
}


static void     _jit_set_environment(jit_ctx *s)
{
    char *          path;
                    path = getenv("C_INCLUDE_PATH");
                    if (path != NULL)
                        tcc_add_sysinclude_path(s, path);
                    path = getenv("CPATH");
                    if (path != NULL)
                        tcc_add_include_path(s, path);
                    path = getenv("LIBRARY_PATH");
                    if (path != NULL)
                        tcc_add_library_path(s, path);
}
JS_METHOD(      js_jit_alloc)
{
                    jit_ctx* ptr = jit_new();
                    JS_RET_U64(ptr)
}
JS_METHOD(      js_jit_alloc_wdefaults)
{
                    jit_ctx* ptr = jit_new();
                    jit_setdefaults(ptr);
                    JS_RET_U64(ptr)
}
JS_METHOD(      js_jit_load)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I32(key, 0)
                    jit_ctx* ptr = jit_load(key);
                    JS_RET_U64(ptr)
}
JS_METHOD(      js_jit_load_wdefaults)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I32(key, 0)
                    jit_ctx* ptr = jit_load(key);
                    jit_setdefaults(ptr);
                    JS_RET_U64(ptr)
}
JS_METHOD(      js_jit_free)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I64(ptr, 0)

                    jit_delete((jit_ctx*)ptr);
                    return NULL;
}
JS_METHOD(      js_jit_sym)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I64(ptr, 0)
                    NAPI_ARGV_UTF8(name, DL_SYM_NAME_MAX_LEN, 1)

                    void * sym = jit_sym( (jit_ctx*)ptr, name );

                    JS_RET_U64(sym)
}
JS_METHOD(      js_jit_string)
{
                    NAPI_ARGV(2) // NAPI_ARGV(3)
                    JS_ARGV_I64(ptr, 0)
                    // NAPI_ARGV_UTF8(name, 2) TODO:
                    NAPI_ARGV_UTF8(src_str, 1024U<<2U, 1)
                    JS_RET_I32( jit_string((jit_ctx*)ptr, src_str) )
}
JS_METHOD(      js_jit_file)
{
                    NAPI_ARGV(2) // NAPI_ARGV(3)
                    JS_ARGV_I64(ptr, 0)
                    // NAPI_ARGV_UTF8(name, 2) TODO:
                    NAPI_ARGV_UTF8(filepath, 512, 1)
                    JS_RET_I32( jit_file((jit_ctx*)ptr, filepath) )
}
JS_METHOD(      js_jit_outfile)
{
                    NAPI_ARGV(2) // NAPI_ARGV(3)
                    JS_ARGV_I64(ptr, 0)
                    // NAPI_ARGV_UTF8(name, 2) TODO:
                    NAPI_ARGV_UTF8(filepath, 512, 1)
                    JS_RET_I32( jit_outfile((jit_ctx*)ptr, filepath) )
}
JS_METHOD(      js_jit_compile)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I64(ptr, 0)
                    JS_RET_I32( jit_compile((jit_ctx*)ptr) )
}
JS_METHOD(      js_jit_run)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I64(ptr, 0)
                    // TODO:
                    int _argc = 0;
                    void * _argv = NULL;
                    // JS_ARGV_I32(_argc, 1)
                    // JS_ARRBUF_CAST(char **, _argv, argv[2])

                    JS_RET_I32( jit_run((jit_ctx*)ptr, _argc, _argv) )
}


int             __jit_var_test = 1000;
int             __jit_var2_test = 2000;
void            __jit_fn_test(int var)
{
                    printf("%s : %i\n","hello from __jit_fn_test", var);
}

int            __jit_setup(napi_env env, napi_value exports)
{
                    JS_EXPORT_FN_AS(js_jit_alloc, jit_alloc)
                    JS_EXPORT_FN_AS(js_jit_alloc_wdefaults, jit_alloc_wdefaults)
                    JS_EXPORT_FN_AS(js_jit_load, jit_load)
                    JS_EXPORT_FN_AS(js_jit_load_wdefaults, jit_load_wdefaults)
                    JS_EXPORT_FN_AS(js_jit_free, jit_free)
                    JS_EXPORT_FN_AS(js_jit_sym, jit_sym)
                    JS_EXPORT_FN_AS(js_jit_string, jit_string)
                    JS_EXPORT_FN_AS(js_jit_file, jit_file)
                    JS_EXPORT_FN_AS(js_jit_outfile, jit_outfile)
                    JS_EXPORT_FN_AS(js_jit_compile, jit_compile)
                    JS_EXPORT_FN_AS(js_jit_run, jit_run)

    #ifdef SMOKE_TESTS
                    printf("%s\n","cc.");

                    int (*func)(int);
                    jit_ctx* ctx;

                    ctx = jit_new();
                    if (!ctx) {
                        fprintf(stderr, "Could not create tcc state\n");
                        exit(1);
                    }
                    printf("%s\n","cc..");
                    /* MUST BE CALLED before any compilation */
                    // tcc_set_output_type(ctx, TCC_OUTPUT_MEMORY);
                    // tcc_set_error_func(ctx, error_opaque, error_func);
                    // "#include <tcclib.h>\n" /* Simple libc header for TCC */
                    char src[] = ""
                        "extern int __jit_var_test; \n"
                        "int foo(int n)\n"
                        "{\n"
                        "    __jit_fn_test( __jit_var_test );\n"
                        "    printf(\"foo(%d) => %d\\n\", n, 2 * n);\n"
                        "    printf(\"&mmap => %u\\n\", dl_sym(\"mmap\"));\n"
                        "    return 2*n;\n"
                        "}\n";
                    "";
                    if (jit_string(ctx, src) == -1)
                        return 1;
                    printf("%s\n","cc...");
                    if (jit_compile(ctx))
                        return 1;
                    func = jit_sym(ctx, "foo");
                    if (!func)
                        return 1;

                    // jit_setsym(ctx,"__jit_var_test",__jit_var2_test);
                    jit_delete(ctx);

                    printf("cc foo(32) -->%u,\n",func(32));
                    printf("cc foo(32) -->%u,\n",func(32));

                    printf("%s\n","cc....!");
                    // jit_run(ctx,0,NULL);
    #endif
                    return 0;
}
// static void     __jit_teardown()
// {
//                     dlclose(dl_main);
// }

/*============================================================================*/