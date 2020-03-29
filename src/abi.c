
/*              abi / symbol / dl
==============================================================================*/

#include        <sys/mman.h>
#include        <dlfcn.h>

#define         DL_SYM_NAME_MAX_LEN \
                    64
#define         SYM_EXPORT(name)\
                    (i64) &##name

s_void *        dl_main;

void*           dl_sym(const char* name)
{                   void* sym = dlsym(dl_main, name);
                    return sym;
}
void            call0(void* sym)
{
                    ((void (*)())sym)();
}
void            callr(void *sym, void *args, void *ret)
{
                    ((void (*)(void *, void *))sym)(args, ret);
}
JS_METHOD(      js_call0)
{
                    NAPI_ARGV(1)
                    JS_ARGV_U64(sym, 0)
                    ((void (*)())sym)();
                    return NULL;
}
JS_METHOD(      js_call1)
{
                    NAPI_ARGV(2)
                    JS_ARGV_U64(sym, 0)
                    JS_ARGV_U64(args, 1)
                    ((void (*)(void *))sym)(args);
                    return NULL;
}
JS_METHOD(      js_callr)
{
                    NAPI_ARGV(3)
                    JS_ARGV_U64(sym, 0)
                    JS_ARGV_U64(args, 1)
                    JS_ARGV_U64(ret, 2)
                    ((void (*)(void *, void *))sym)(args, ret);
                    return NULL;
}
JS_METHOD(      js_sym)
{
                    // static void *syms[64];
                    NAPI_ARGV(1)
                    NAPI_ARGV_UTF8(
                        name, DL_SYM_NAME_MAX_LEN, 0)
                    // u64_t hash = hash64(name);
                    void * sym = dl_sym(name);
                    // int sym_handle = 0;
                    // syms[sym_handle] = sym;
                    // JS_RET_U64(sym_handle)
                    // printf("\nc sym %"PRIu64"\n",sym);
                    JS_RET_U64(sym)
                    // NAPI_ARGV_BUFFER_CAST(struct things *, t, 0)
                    // t->operations+=2;
                    // return NULL;
}
JS_METHOD(      arrbuf_ref  )
{                   NAPI_ARGV(1)
                    JS_ARRBUF_CAST(char *, arrbuf_ptr, argv[0])
                    JS_RET_U64(arrbuf_ptr)
}
JS_METHOD(      buf_ref )
{                   NAPI_ARGV(1)
                    JS_BUF_CAST(char *, arrbuf_ptr, argv[0])
                    JS_RET_U64(arrbuf_ptr)
}


void            __abi_setup(napi_env env, napi_value exports)
{
                    if (!MOD_DID_INIT)
                        dl_main = dlopen(NULL, RTLD_LAZY);
                    JS_EXPORT_FN_AS(js_sym, sym)
                    JS_EXPORT_FN_AS(js_call0, call0)
                    JS_EXPORT_FN_AS(js_call1, call1)
                    JS_EXPORT_FN_AS(js_callr, callr)
                    JS_EXPORT_FN(buf_ref)
                    JS_EXPORT_FN(arrbuf_ref)
}
static void     __abi_teardown()
{
                    dlclose(dl_main);
}
/*============================================================================*/
