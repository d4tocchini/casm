
#include "sx/allocator.h"

sx_alloc* SX_ALLOCATOR = NULL;

/*

#define WASM_PAGE_SIZE 65536
    // 1<<16

#ifndef __wasi__

uint32_t __builtin_wasm_memory_size(int meminst)
{
    return 1<<16
}

// alloc_size / os_page_size
uint32_t __builtin_wasm_memory_grow(int meminst, int numpages)
{

}

#endif


https://github.com/gabrielschulhof/abi-stable-node-addon-examples/blob/tsfn_example/thread_safe_function/node-api/tsfn_example.c

https://nodejs.org/api/n-api.html#n_api_custom_asynchronous_operations

https://stackoverflow.com/questions/50815038/n-api-continuous-callbacks-for-c-media-stream/50828983

https://github.com/nodejs/node/blob/4e5bb250d8c256dd32af3b2a7fd99197a56c21d6/test/js-native-api/test_general/test_general.c

https://github.com/poemm/C_ewasm_contracts/blob/859c7904784331604d3bdc15db3bff59de587dd9/src/ewasm.h#L120
*/

JS_METHOD(      js_c_malloc)
{
                    JS_ARGV(1)
                    JS_ARGV_U32(sz, 0)
                    void * ptr = malloc(sz);
                    JS_RET_I64(ptr)
}
JS_METHOD(      js_c_malloc_align)
{
                    JS_ARGV(2)
                    JS_ARGV_U32(sz, 0)
                    JS_ARGV_U32(al, 1)
                    void * ptr = NULL;
                    posix_memalign(&ptr,sz,al);
                    JS_RET_I64(ptr)
}
// char str_ptr[1024];
JS_METHOD(      js_c_malloc_str)
{
                    JS_ARGV(2)
                    JS_ARGV_U32(sz, 1)
                    JS_ARGV_UTF8(
                        str, sz, 0)
                    char * ptr = malloc(sz + 1);
                    strcpy(ptr, str);
                    JS_RET_I64(ptr)
}
JS_METHOD(      js_c_deref_str)
{
                    JS_ARGV(1)
                    // JS_ARGV_U32(sz, 1)
                    JS_ARGV_I64(ptr, 0)
                    // strcpy(ptr, addr);
                    JS_RET_STR(ptr)
}
JS_METHOD(      js_c_deref_utf8)
{
                    JS_ARGV(2)
                    JS_ARGV_I64(ptr, 0)
                    JS_ARGV_U32(len, 1)
                    JS_RET_UTF8(ptr,len)
}
JS_METHOD(      js_c_deref_u32)
{
                    JS_ARGV(1)
                    JS_ARGV_I64(ptr, 0)
                    u32 val = *((u32 *) ptr);
                    JS_RET_U32(val)
}
JS_METHOD(      js_c_deref_i32)
{
                    JS_ARGV(1)
                    JS_ARGV_I64(ptr, 0)
                    i32 val = *((i32 *) ptr);
                    JS_RET_I32(val)
}
JS_METHOD(      js_c_deref_u64)
{
                    JS_ARGV(1)
                    JS_ARGV_I64(ptr, 0)
                    u64 val = *((u64 *) ptr);
                    JS_RET_U64(val)
}
JS_METHOD(      js_c_deref_i64)
{
                    JS_ARGV(1)
                    JS_ARGV_I64(ptr, 0)
                    i64 val = *((i64 *) ptr);
                    JS_RET_I64(val)
}
JS_METHOD(      js_c_free)
{
                    JS_ARGV(1)
                    JS_ARGV_I64(ptr, 0)
                    free(ptr);
                    return NULL;
}
JS_METHOD(      js_strcpy)
{
                    JS_ARGV(2)
                    JS_ARGV_I64(dst, 0)
                    JS_ARGV_I64(src, 1)
                    strcpy(dst, src);
                    JS_RET_I64(dst)
}
JS_METHOD(      js_strncpy)
{
                    JS_ARGV(3)
                    JS_ARGV_I64(dst, 0)
                    JS_ARGV_I64(src, 1)
                    JS_ARGV_I32(len, 2)
                    strncpy(dst, src, len);
                    JS_RET_I64(dst)
}
JS_METHOD(      js_memcpy)
{
                    JS_ARGV(3)
                    JS_ARGV_I64(dst, 0)
                    JS_ARGV_I64(src, 1)
                    JS_ARGV_I32(n, 2)
                    memcpy(dst, src, n);
                    JS_RET_I64(dst)
}
JS_METHOD(      js_xor256to64)
{
                    JS_ARGV(1)
                    JS_ARGV_I64(_ptr, 0)
                    u64 * ptr = _ptr;
                    // TODO: replace w/ SIMD
                    u64 ret = ptr[0] ^ ptr[1] ^ ptr[2] ^ ptr[3];
                    JS_RET_I64(ret)
}
void            __malloc_setup(napi_env env, napi_value exports)
{
                    if (SX_ALLOCATOR == NULL)
                        SX_ALLOCATOR = sx_alloc_malloc();
                    JS_EXPORT_FN_AS(js_c_malloc, c_malloc)
                    JS_EXPORT_FN_AS(js_c_malloc_align, c_malloc_align)
                    JS_EXPORT_FN_AS(js_c_malloc_str, c_malloc_str)
                    JS_EXPORT_FN_AS(js_c_deref_str, c_deref_str)
                    JS_EXPORT_FN_AS(js_c_deref_utf8, c_deref_utf8)
                    JS_EXPORT_FN_AS(js_c_deref_u32, c_deref_u32)
                    JS_EXPORT_FN_AS(js_c_deref_i32, c_deref_i32)
                    JS_EXPORT_FN_AS(js_c_deref_u64, c_deref_u64)
                    JS_EXPORT_FN_AS(js_c_deref_i64, c_deref_i64)
                    JS_EXPORT_FN_AS(js_c_free, c_free)
                    JS_EXPORT_FN_AS(js_strcpy, strcpy)
                    JS_EXPORT_FN_AS(js_strncpy, strncpy)
                    JS_EXPORT_FN_AS(js_memcpy, memcpy)
                    JS_EXPORT_FN_AS(js_xor256to64, xor256to64)
}
