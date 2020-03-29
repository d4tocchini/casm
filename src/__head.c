/****************************************************************
*****************************************************************
THIS IS AN AUTO AMALGAMATED FILE
*****************************************************************
****************************************************************/

#define NAPI_EXPERIMENTAL

#include        <node_api.h>
#include        "napi.h"
#include        "types.h"
#include        <string.h>
#include        <stdio.h>
#include        <assert.h>
#include        <stdlib.h>
// #include     <stdlib.h>     // size_t, malloc, posix_memalign
// #include     <stdbool.h>    // bool

int             module_init_count = 0;
#define         MOD_DID_INIT (module_init_count > 0)

#define         MOD_SETUP() \
                    __malloc_setup(env, exports);\
                    __hash_setup(env, exports);\
                    __abi_setup(env, exports);\
                    __crypto_setup(env, exports);\
                    __job_setup(env, exports);\
                    __jit_setup(env, exports);\
                    __xm_setup(env, exports);\
                    __end_setup(env, exports);
                    // __xm_setup();
#define         MOD_TEARDOWN() \
                    __abi_teardown();


void            __end_setup(napi_env env, napi_value exports)
{
                    ++module_init_count;
                    JS_EXPORT_I32(module_init_count)
}