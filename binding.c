
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

#include "sx/hash.h"

// xxhash32: suitable for smaller data, and it's faster in all situations
// xxhash64: suitable for bigger data, collision of this is almost zero
#define xxhash32 sx_hash_xxh32
#define xxhash64 sx_hash_xxh64
// uint32_t xxhash32(const void* data, size_t len, uint32_t seed) {
//     return sx_hash_xxh32(data, len, seed);
// }
// uint64_t xxhash64(const void* data, size_t len, uint64_t seed) {
//     return sx_hash_xxh64(data, len, seed);
// }

// Integer hash functions: useful for pointer/index hashing
// Reference: https://gist.github.com/badboy/6267743
#define u32hash sx_hash_u32
#define u64hash sx_hash_u64
#define u64hashu32 sx_hash_u64_to_u32
// uint32_t u32hash(uint32_t key) {
//     return sx_hash_u32(key);
// }
// uint64_t u64hash(uint64_t key) {
//     return sx_hash_u64(key);
// }
// uint32_t u64hashu32(uint64_t key) {
//     return sx_hash_u64_to_u32(key);
// }

JS_METHOD(js_xxhash64)
{   // js_xxhash64( src_ptr, len, seed )
    NAPI_ARGV(3)
    JS_ARGV_U64(src_addr, 0)
    JS_ARGV_U32(len, 1)
    JS_ARGV_U64(seed, 2)
    u64 hash = xxhash64(src_addr, len, seed);
    JS_RET_U64(hash);
}
JS_METHOD(js_xxhash32)
{   // js_xxhash64( dst_ptr, src_ptr, len, seed )
    NAPI_ARGV(3)
    JS_ARGV_U64(src_addr, 0)
    JS_ARGV_U32(len, 1)
    JS_ARGV_U32(seed, 2)
    u32 hash = xxhash32(src_addr, len, seed);
    JS_RET_U32(hash);
}
JS_METHOD(js_xxhash31)
{   // js_xxhash64( dst_ptr, src_ptr, len, seed )
    NAPI_ARGV(3)
    JS_ARGV_U64(src_addr, 0)
    JS_ARGV_U32(len, 1)
    JS_ARGV_U32(seed, 2)
    i32 hash = xxhash32(src_addr, len, seed);
    JS_RET_I31(hash);
}
JS_METHOD(js_str_hash64)
{   // js_xxhash64( src_ptr, len, seed )
    NAPI_ARGV(3)
    JS_ARGV_U32(len, 1)
    JS_ARGV_UTF8(str, len + 1, 0)
    JS_ARGV_U64(seed, 2)
    u64 hash = xxhash64(&str, len, seed);
    JS_RET_U64(hash);
}
JS_METHOD(js_str_hash32)
{   // js_xxhash64( dst_ptr, src_ptr, len, seed )
    NAPI_ARGV(3)
    JS_ARGV_U32(len, 1)
    JS_ARGV_UTF8(str, len + 1, 0)
    JS_ARGV_U32(seed, 2)
    i32 hash = xxhash32(&str, len, seed);
    JS_RET_I32(hash);
}
JS_METHOD(js_str_hash31)
{   // js_xxhash64( dst_ptr, src_ptr, len, seed )
    NAPI_ARGV(3)
    JS_ARGV_U32(len, 1)
    JS_ARGV_UTF8(str, len + 1, 0)
    JS_ARGV_U32(seed, 2)
    i32 hash = xxhash32(&str, len, seed);
    JS_RET_I31(hash);
}
JS_METHOD(js_u32_hash31)
{   // js_xxhash64( dst_ptr, src_ptr, len, seed )
    NAPI_ARGV(1)
    JS_ARGV_U32(n, 0)
    i32 hash = u32hash(n);
    JS_RET_I31(hash);
}
// JS_METHOD(js_strhash32)
// {   // js_xxhash64( dst_ptr, src_ptr, len, seed )
//     NAPI_ARGV(3)
//     JS_ARGV_U64(src_addr, 0)
//     JS_ARGV_U32(len, 1)
//     JS_ARGV_U32(seed, 2)
//     u32 hash = xxhash32(src_addr, len, seed);
//     JS_RET_U32(hash);
// }
JS_METHOD(js_u64_hashu32)
{
    NAPI_ARGV(1)
    JS_ARGV_U64(in, 0)
    u32 hash = u64hashu32(in);
    JS_RET_U32(hash);
}
JS_METHOD(js_u64_hashu31)
{
    NAPI_ARGV(1)
    JS_ARGV_U64(in, 0)
    u32 hash = u64hashu32(in);
    JS_RET_U31(hash);
}

// Streaming (state based) hash using xxhash32
// typedef struct sx_hash_xxh32 sx_hash_xxh32_t;

// SX_API sx_hash_xxh32_t* sx_hash_create_xxh32(const sx_alloc* alloc);
// SX_API void sx_hash_destroy_xxh32(sx_hash_xxh32_t* state, const sx_alloc* alloc);
// SX_API void sx_hash_xxh32_init(sx_hash_xxh32_t* state, uint32_t seed);
// SX_API void sx_hash_xxh32_update(sx_hash_xxh32_t* state, const void* data, size_t len);
// SX_API uint32_t sx_hash_xxh32_digest(sx_hash_xxh32_t* state);

// // Streaming (state based) hash using xxhash64
// typedef struct sx_hash_xxh64_s sx_hash_xxh64_t;

// SX_API sx_hash_xxh64_t* sx_hash_create_xxh64(const sx_alloc* alloc);
// SX_API void sx_hash_destroy_xxh64(sx_hash_xxh64_t* state, const sx_alloc* alloc);
// SX_API void sx_hash_xxh64_init(sx_hash_xxh64_t* state, uint64_t seed);
// SX_API void sx_hash_xxh64_update(sx_hash_xxh64_t* state, const void* data, size_t len);
// SX_API uint64_t sx_hash_xxh64_digest(sx_hash_xxh64_t* state);

// #include "stdlib.h"   /* abort() */
// #include "xxhash.h"
// unsigned long long calcul_hash_streaming(someCustomType handler)
// {
//     /* create a hash state */
//     XXH64_state_t* const state = XXH64_createState();
//     if (state==NULL) abort();

//     size_t const bufferSize = SOME_SIZE;
//     void* const buffer = malloc(bufferSize);
//     if (buffer==NULL) abort();

//     /* Initialize state with selected seed */
//     unsigned long long const seed = 0;   /* or any other value */
//     XXH_errorcode const resetResult = XXH64_reset(state, seed);
//     if (resetResult == XXH_ERROR) abort();

//     /* Feed the state with input data, any size, any number of times */
//     (...)
//     while ( /* any condition */ ) {
//         size_t const length = get_more_data(buffer, bufferSize, handler);   
//         XXH_errorcode const updateResult = XXH64_update(state, buffer, length);
//         if (updateResult == XXH_ERROR) abort();
//         (...)
//     }
//     (...)

//     /* Get the hash */
//     XXH64_hash_t const hash = XXH64_digest(state);

//     /* State can then be re-used; in this example, it is simply freed  */
//     free(buffer);
//     XXH64_freeState(state);

//     return (unsigned long long)hash;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////
// Hash table
// typedef struct sx_hashtbl {
//     uint32_t* keys;
//     int* values;
//     int _bitshift;
//     int count;
//     int capacity;
// #if SX_CONFIG_HASHTBL_DEBUG
//     int _miss_cnt;
//     int _probe_cnt;
// #endif
// } sx_hashtbl;
// SX_API sx_hashtbl* sx_hashtbl_create(const sx_alloc* alloc, int capacity);
// SX_API void sx_hashtbl_destroy(sx_hashtbl* tbl, const sx_alloc* alloc);
// SX_API bool sx_hashtbl_grow(sx_hashtbl** ptbl, const sx_alloc* alloc);

// SX_API void sx_hashtbl_init(sx_hashtbl* tbl, int capacity, uint32_t* keys_ptr, int* values_ptr);
// SX_API int sx_hashtbl_valid_capacity(int capacity);
// SX_API int sx_hashtbl_fixed_size(int capacity);

// SX_API int sx_hashtbl_add(sx_hashtbl* tbl, uint32_t key, int value);
// SX_API int sx_hashtbl_find(const sx_hashtbl* tbl, uint32_t key);
// SX_API void sx_hashtbl_clear(sx_hashtbl* tbl);

void            __hash_setup(napi_env env, napi_value exports)
{
                    JS_EXPORT_FN_AS(js_xxhash64, xxhash64)
                    JS_EXPORT_FN_AS(js_xxhash32, xxhash32)
                    JS_EXPORT_FN_AS(js_xxhash31, xxhash31)
                    JS_EXPORT_FN_AS(js_str_hash64, str_hash64)
                    JS_EXPORT_FN_AS(js_str_hash32, str_hash32)
                    JS_EXPORT_FN_AS(js_str_hash31, str_hash31)
                    JS_EXPORT_FN_AS(js_u32_hash31, u32_hash31)
                    JS_EXPORT_FN_AS(js_u64_hashu32, u64_hashu32)
                    JS_EXPORT_FN_AS(js_u64_hashu31, u64_hashu31)
}
typedef sx_hashtbl map32_t;

void map32_create(const void **map, int capacity)
{
    *map = sx_hashtbl_create(SX_ALLOCATOR, capacity);
}

void map32_destroy(map32_t *map)
{
    sx_hashtbl_destroy(map, SX_ALLOCATOR);
}

void map32_capacity(const map32_t *map, int *out)
{
    *out = map->capacity;
}

void map32_size(const map32_t *map, int *out)
{
    *out = map->count;
}

void map32_is_full(const map32_t *map, bool *out)
{
    *out = map->capacity == map->count;
}

void map32_set(map32_t *map, uint32_t key, int val, int *idx)
{
    *idx = sx_hashtbl_add(map, key, val);
}

void map32_get(map32_t *map, uint32_t key, int *out)
{
    *out = sx_hashtbl_find(map, key);
}

void map32_at(const map32_t *map, int idx, int *out)
{
    *out = sx_hashtbl_get(map, idx);
}

void map32_clear(map32_t *map)
{
    sx_hashtbl_clear(map);
}

// void map32(int64_t **p)
// {
//     static const in64_t SYM[] = {
//         &map32_create,
//         &map32_destroy,
//         &map32_capacity,
//         &map32_size,
//         &map32_is_full,
//         &map32_set,
//         &map32_get,
//         &map32_at,
//         &map32_clear
//     };
//     void (*) SYM[p[0]]
// }

// void gmap32_grow(map32_t *map)
// {
//     sx_hashtbl_destroy(map, SX_ALLOCATOR);
// }
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

/*              crypto
==============================================================================*/
#include        "hydrogen.h"

#define         CRYPTO_KEY_SIZE hydro_hash_KEYBYTES
                    // == hydro_secretbox_KEYBYTES == 32
sc_ch*          CRYPTO_CTX = "ÆXILE";

int             crypto_hash256(
    u8*             out,
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    return hydro_hash_hash(out, 32, bin, bin_sz, CRYPTO_CTX, _key);
}
int             crypto_hash128(
    u8*             out,
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    return hydro_hash_hash(out, 16, bin, bin_sz, CRYPTO_CTX, _key);
}
u64             crypto_hash64(
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    u64 h[2];
                    hydro_hash_hash(h, 16, bin, bin_sz, CRYPTO_CTX, _key);
                    return h[0]^h[1];
                    // return *((u64 *)h);
}
u32             crypto_hash32(
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    u32 h[4];
                    hydro_hash_hash(h, 16, bin, bin_sz, CRYPTO_CTX, _key);
                    return h[0]^h[1]^h[2]^h[3];
}
bool            crypto_eq(
    c_void*         b1,
    c_void*         b2,
    sz_t            sz)
{                   //  - returns 1 if the len bytes pointed to by b1_ match the len bytes pointed to by b2_. Otherwise, it returns 0.
                    //  - returns 0 if b1_ and b2_ are the exact same address, as this is likely to only happen on misuse.
                    return hydro_equal(b1, b2, sz);
}

int             crypto_box_enc(
    u8*             out,
    c_void*         msg,
    sz_t            msg_sz,
    c_u8            key[CRYPTO_KEY_SIZE],
    u64             _id) // 0
{                   //  * The hydro_secretbox_encrypt() function encrypts a message m_ of length mlen bytes using a context ctx, a secret key key and a message counter msg_id.
                    //  * It puts the ciphertext whose length is hydro_secretbox_HEADERBYTES + mlen into c.
                    //  * The header includes an automatically-generated 160-bit nonce as well as a 128-bit authentication tag.
                    //  * msg_id * is an optional message tag. For example, if 3 messages are sent to the same recipient using the same key, these messages can sequentially use 0, 1 and 2 as identifiers.
                    //      * If this mechanism is not required by an application, using a constant msg_id such as 0 is also totally fine. Message identifiers are optional and do not have to be unique.
                    return hydro_secretbox_encrypt(out, msg, msg_sz, _id, CRYPTO_CTX, key);
}
int             crypto_box_dec(
    u8*             out,
    c_void*         box,
    sz_t            box_sz,
    c_u8            key[CRYPTO_KEY_SIZE],
    u64             _id)
{                   //  --> yay -> 0, nay -> -1
                    return hydro_secretbox_decrypt(out, box, box_sz, _id, CRYPTO_CTX, key);
}
int             crypto_box_verify(
    c_u8*           box,
    sz_t            box_sz,
    c_u8            key[CRYPTO_KEY_SIZE])
{                   //  --> yay -> 0, nay -> -1
    u8              probe[hydro_secretbox_PROBEBYTES];
                    hydro_secretbox_probe_create(probe, box, box_sz, CRYPTO_CTX, key);
                    return hydro_secretbox_probe_verify(probe, box, box_sz, CRYPTO_CTX, key);
}

int             hex2bin_parse(
    u8*             bin,
    sz_t            bin_maxlen,
    c_ch*           hex,
    sz_t            hex_len,
    c_ch*           _ignore,    // NULL
    c_ch**          _hex_end_p) // NULL
{                   /*
                    - parses a hexadecimal string hex & converts it to a byte sequence.
                    - The parser stops when a non-hexadecimal, non-ignored character is found or when bin_maxlen bytes have been written.
                    -> -1
                        if more than bin_maxlen bytes would be required to store the parsed string.
                    -> number of written bytes
                        on success and sets hex_end, if it is not NULL, to a pointer to the character following the last parsed character.
                    :hex
                        - does not have to be nul terminated, as the number of characters to parse is supplied via the hex_len parameter.
                    :ignore
                        is a string of characters to skip. For example, the string ": " allows columns and spaces to be present at any locations in the hexadecimal string. These characters will just be ignored. As a result, "69:FC", "69 FC", "69 : FC" and "69FC" will be valid inputs, and will produce the same output.
                        - can be set to NULL in order to disallow any non-hexadecimal character.
                    :bin_maxlen
                        is the maximum number of bytes to put into bin.

                    hydro_bin2hex: hex_maxlen
                        - the maximum number of bytes that the function is allowed to write starting at hex. It should be at least bin_len * 2 + 1.
                    */
                    return hydro_hex2bin( bin, (hex_len << 1U) + 1U, hex, hex_len,
                        _ignore, _hex_end_p);

}
ch*             hex2bin(
    u8*             out,
    c_ch*           hex,
    sz_t            hex_sz)
{
                    return hex2bin_parse(out, (hex_sz >> 1U), hex, hex_sz,
                    NULL, NULL);
}
ch*             bin2hex(
    ch*             hex,
    c_u8*           bin,
    sz_t            bin_sz)
{                   //  - converts bin_len bytes stored at bin into a hexadecimal string w/ nul byte (\0) terminator.
                    //  - returns hex on success, or NULL on overflow. It evaluates in constant time for a given size.
                    //  : hex_maxlen
                    //      - the maximum number of bytes that the function is allowed to write starting at hex. It should be at least bin_len * 2 + 1.
                    return hydro_bin2hex(hex, (bin_sz << 1U) + 1U, bin, bin_sz);

}

/*============================================================================*/

// https://github.com/jedisct1/libhydrogen/wiki/Random-numbers

#define hydro_random_SEEDBYTES 32

#define CRYPTO_RANDOM_U32_MINMAX(min, max) \
    (min + hydro_random_uniform(max + 1 - min))

#define CRYPTO_RANDOM_U32_MAX(max) \
    (hydro_random_uniform(max + 1))

void crypto_u32(u32 *out)
{
    *out = hydro_random_u32();
}

void crypto_u32_max(u32 *out, const u32 max)
{
    *out = CRYPTO_RANDOM_U32_MAX(max);
}

void crypto_u32_minmax(u32 *out, const u32 min, const u32 max)
{
    *out = CRYPTO_RANDOM_U32_MINMAX(min, max);
}

// void crypto_sample_u32(u32 *out, u32 *buf, const u32 min, const u32 max)
// {
//     *out = buf[CRYPTO_RANDOM_U32_MINMAX(min, max)];
// }


void crypto_memset_seed256(void *buf, size_t len, const u8 seed[hydro_random_SEEDBYTES])
    // The hydro_random_buf_deterministic() function stores len bytes into buf indistinguishable from random bytes without knowing seed. For a given seed, this function will always output the same sequence. This function is mainly useful for writing tests.
{
    hydro_random_buf_deterministic(buf, len, seed);
}

#define _CRYPTO_SWAP(TYPE, a, b)\
    TYPE temp = buf[i];\
    buf[i] = buf[j];\
    buf[j] = temp;

void crypto_shuffle32(u32 *buf, int len)
    // Fisher–Yates shuffle Algorithm
    // Start from the last element and swap one by one. We don't need to run for the first element that's why i > 0
{
    for (int i = len-1; i > 0; i--)
    {
        int j = CRYPTO_RANDOM_U32_MAX(i);
        _CRYPTO_SWAP(u32, i, j)
    }
}

void crypto_shuffle8(u8 *buf, int len)
    // Fisher–Yates
{
    for (int i = len-1; i > 0; i--)
    {
        int j = CRYPTO_RANDOM_U32_MAX(i);
        _CRYPTO_SWAP(u8, i, j)
    }
}

void crypto_range32(i32 *buf, i32 min, i32 max)
    // Fisher–Yates shuffle Algorithm
{
    i32 i = 0;
    i32 len = max - min + 1;
    while (i < len) {
        buf[i] = min + i++;
    }
    for (i = len-1; i > 0; i--)
    {
        i32 j = CRYPTO_RANDOM_U32_MAX(i);
        // Swap arr[i] with the element at random index
        _CRYPTO_SWAP(u32, i, j)
    }
}


// https://github.com/jedisct1/libhydrogen/wiki/Password-hashing

sc_ch*  SPECIALS = "!@$*_+-?";
#define SPECIALS_MAX    7
#define SPECIALS_LEN    8
#define LOWERCASE_MIN   'a'
#define LOWERCASE_MAX   'z'
#define LOWERCASE_LEN   26
#define LOWERCASE_0     (SPECIALS_LEN)
#define UPPERCASE_MIN   'A'
#define UPPERCASE_MAX   'Z'
#define UPPERCASE_LEN   26
#define UPPERCASE_0     (SPECIALS_LEN + LOWERCASE_LEN)
#define DIGIT_MIN       '0'
#define DIGIT_MAX       '9'
#define DIGIT_LEN       10
#define DIGIT_0         (SPECIALS_LEN + LOWERCASE_LEN + UPPERCASE_LEN)
#define PASSCH_LEN      (SPECIALS_LEN + LOWERCASE_LEN + UPPERCASE_LEN + DIGIT_LEN)
#define PASSCH_MAX      (PASSCH_LEN - 1)

ch crypto_pass_gen_char()
{
    int idx = CRYPTO_RANDOM_U32_MAX(PASSCH_MAX);
    if (idx >= DIGIT_0)
        return DIGIT_MIN + (idx - DIGIT_0);
    if (idx >= UPPERCASE_0)
        return UPPERCASE_MIN + (idx - UPPERCASE_0);
    if (idx >= LOWERCASE_0)
        return LOWERCASE_MIN + (idx - LOWERCASE_0);
    return SPECIALS[idx];
}
void crypto_pass_gen(u8 *buf, int len)
{
    if (len < 8)
        len = 8;
    buf[0] = CRYPTO_RANDOM_U32_MINMAX(LOWERCASE_MIN, LOWERCASE_MAX);
    buf[1] = CRYPTO_RANDOM_U32_MINMAX(LOWERCASE_MIN, LOWERCASE_MAX);
    buf[2] = CRYPTO_RANDOM_U32_MINMAX(UPPERCASE_MIN, UPPERCASE_MAX);
    buf[3] = CRYPTO_RANDOM_U32_MINMAX(UPPERCASE_MIN, UPPERCASE_MAX);
    buf[4] = CRYPTO_RANDOM_U32_MINMAX(DIGIT_MIN, DIGIT_MAX);
    buf[5] = CRYPTO_RANDOM_U32_MINMAX(DIGIT_MIN, DIGIT_MAX);
    buf[6] = SPECIALS[CRYPTO_RANDOM_U32_MAX(SPECIALS_MAX)];
    buf[7] = SPECIALS[CRYPTO_RANDOM_U32_MAX(SPECIALS_MAX)];
    int i = 8;
    while (i < len)
        buf[i++] = crypto_pass_gen_char();
    crypto_shuffle8(buf, len);
}
// buf = new Uint8Array(10); casm.c.bindings.crypto_pass_gen(casm.c.bindings.arrbuf_ref(buf.buffer), 10)



JS_METHOD(      js_crypto_hash256)
{
                    NAPI_ARGV(4)
                    JS_ARGV_I64(out_ptr, 0)
                    JS_ARGV_I64(in_ptr, 1)
                    JS_ARGV_U32(in_sz, 2)
                    JS_ARGV_I64(key_ptr, 3)
                    i32 ret = crypto_hash256(out_ptr, in_ptr, in_sz, key_ptr);
                    JS_RET_I32(ret);
}
JS_METHOD(      js_crypto_str_hash256)
{
                    NAPI_ARGV(4)
                    JS_ARGV_I64(out_ptr, 0)

                    JS_ARGV_U32(in_sz, 2) // JS_ARGV_U32(in_sz, 2)
                    JS_ARGV_UTF8(in_str, in_sz + 1, 1) // JS_ARGV_I64(in_ptr, 1)

                    JS_ARGV_I64(key_ptr, 3)
                    i32 ret = crypto_hash256(out_ptr, &in_str, in_sz, key_ptr);
                    JS_RET_I32(ret);
}
JS_METHOD(      js_crypto_hash128)
{
                    NAPI_ARGV(4)
                    JS_ARGV_I64(out_ptr, 0)
                    JS_ARGV_I64(in_ptr, 1)
                    JS_ARGV_U32(in_sz, 2)
                    JS_ARGV_I64(key_ptr, 3)
                    i32 ret = crypto_hash128(out_ptr, in_ptr, in_sz, key_ptr);
                    JS_RET_I32(ret);
}
JS_METHOD(      js_crypto_hash64)
{
                    NAPI_ARGV(3)
                    JS_ARGV_I64(in_ptr, 0)
                    JS_ARGV_U32(in_sz, 1)
                    JS_ARGV_I64(key_ptr, 2)
                    u64 ret = crypto_hash64(in_ptr, in_sz, key_ptr);
                    JS_RET_I64(ret);
}
JS_METHOD(      js_crypto_hash32)
{
                    NAPI_ARGV(3)
                    JS_ARGV_I64(in_ptr, 0)
                    JS_ARGV_U32(in_sz, 1)
                    JS_ARGV_I64(key_ptr, 2)
                    u32 ret = crypto_hash32(in_ptr, in_sz, key_ptr);
                    JS_RET_U32(ret);
}
JS_METHOD(      js_crypto_box_enc)
{
                    NAPI_ARGV(5)
                    JS_ARGV_U64(out_ptr, 0)
                    JS_ARGV_U64(msg_ptr, 1)
                    JS_ARGV_U32(msg_sz, 2)
                    JS_ARGV_U64(key_ptr, 3)
                    JS_ARGV_U32(_id, 4)
                    // JS_ARGV_U64(_key, 3)
                    i32 ret = crypto_box_enc(out_ptr, msg_ptr, msg_sz, key_ptr, _id);
                    JS_RET_I32(ret);
}
// JS_METHOD(      js_crypto_box_enc_str)
// {
//                     NAPI_ARGV(5)
//                     JS_ARGV_U64(out_ptr, 0)
//                     JS_ARGV_U32(msg_sz, 2)
//                     JS_ARGV_UTF8(msg_str, msg_sz+1, 1)
//                     JS_ARGV_U64(key_ptr, 3)
//                     JS_ARGV_U32(_id, 4)
//                     // JS_ARGV_U64(_key, 3)
//                     i32 ret = crypto_box_enc(out_ptr, &msg_str, msg_sz, key_ptr, _id);
//                     JS_RET_I32(ret);
// }
JS_METHOD(      js_crypto_box_dec)
{
                    NAPI_ARGV(5)
                    JS_ARGV_U64(out_ptr, 0)
                    JS_ARGV_U64(box_ptr, 1)
                    JS_ARGV_U32(box_sz, 2)
                    JS_ARGV_U64(key_ptr, 3)
                    JS_ARGV_U32(_id, 4)
                    // JS_ARGV_U64(_key, 3)
                    i32 ret = crypto_box_dec(out_ptr, box_ptr, box_sz, key_ptr, _id);
                    JS_RET_I32(ret);
}
JS_METHOD(      js_crypto_box_verify)
{
                    NAPI_ARGV(3)
                    JS_ARGV_U64(box_ptr, 0)
                    JS_ARGV_U32(box_sz, 1)
                    JS_ARGV_U64(key_ptr, 2)
                    i32 ret = crypto_box_verify(box_ptr, box_sz, key_ptr);
                    JS_RET_I32(ret);
}


JS_METHOD(      js_crypto_u32)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I64(out, 0)
                    crypto_u32(out);
                    return NULL;
}
JS_METHOD(      js_crypto_u32_max)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I64(out, 0)
                    JS_ARGV_I32(max, 1)
                    crypto_u32_max(out, max);
                    return NULL;
}
JS_METHOD(      js_crypto_u32_minmax)
{
                    NAPI_ARGV(3)
                    JS_ARGV_I64(out, 0)
                    JS_ARGV_I32(min, 1)
                    JS_ARGV_I32(max, 2)
                    crypto_u32_minmax(out, min, max);
                    return NULL;
}
JS_METHOD(      js_crypto_memset)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I64(ptr, 0)
                    JS_ARGV_I32(len, 1)
                    crypto_memset(ptr, len);
                    return NULL;
}
JS_METHOD(      js_crypto_shuffle32)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I64(ptr, 0)
                    JS_ARGV_I32(len, 1)
                    crypto_shuffle32(ptr, len);
                    return NULL;
}
JS_METHOD(      js_crypto_shuffle8)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I64(ptr, 0)
                    JS_ARGV_I32(len, 1)
                    crypto_shuffle8(ptr, len);
                    return NULL;
}

JS_METHOD(      js_crypto_range32)
{
                    NAPI_ARGV(3)
                    JS_ARGV_I64(ptr, 0)
                    JS_ARGV_I32(min, 1)
                    JS_ARGV_I32(max, 2)
                    crypto_range32(ptr, min, max);
                    return NULL;
}


JS_METHOD(      js_crypto_pass_gen)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I64(ptr, 0)
                    JS_ARGV_I32(len, 1)
                    crypto_pass_gen(ptr, len);
                    return NULL;
}


void            __crypto_setup(napi_env env, napi_value exports)
{
                    if (!MOD_DID_INIT) {
                        if (hydro_init() != 0) {
                            printf("\n\n%s\n\n","CRYPTO FAILED SETUP");
                            abort();
                        }
                    }
                    JS_EXPORT_FN_AS(js_crypto_hash256,      crypto_hash256)
                    JS_EXPORT_FN_AS(js_crypto_str_hash256,  crypto_str_hash256)
                    JS_EXPORT_FN_AS(js_crypto_hash128,      crypto_hash128)
                    JS_EXPORT_FN_AS(js_crypto_hash64 ,      crypto_hash64 )
                    JS_EXPORT_FN_AS(js_crypto_hash32 ,      crypto_hash32 )
                    JS_EXPORT_FN_AS(js_crypto_box_enc,      crypto_box_enc)
                    // JS_EXPORT_FN_AS(js_crypto_box_enc_str,  crypto_box_enc_str)
                    JS_EXPORT_FN_AS(js_crypto_box_dec,      crypto_box_dec)
                    JS_EXPORT_FN_AS(js_crypto_box_verify,   crypto_box_verify)

                    JS_EXPORT_FN_AS(js_crypto_u32,          crypto_u32)
                    JS_EXPORT_FN_AS(js_crypto_u32_max,      crypto_u32_max)
                    JS_EXPORT_FN_AS(js_crypto_u32_minmax,   crypto_u32_minmax)
                    JS_EXPORT_FN_AS(js_crypto_memset,       crypto_memset)
                    JS_EXPORT_FN_AS(js_crypto_shuffle32,    crypto_shuffle32)
                    JS_EXPORT_FN_AS(js_crypto_shuffle8,     crypto_shuffle8)
                    JS_EXPORT_FN_AS(js_crypto_range32,      crypto_range32)
                    JS_EXPORT_FN_AS(js_crypto_pass_gen,     crypto_pass_gen)

    #ifdef SMOKE_TESTS
    u8*             key3 = malloc(32);
    u8              key[32];
    u8              key2[32];
                    hydro_secretbox_keygen(key3);
                    hydro_secretbox_keygen(&key2);
                    hydro_secretbox_keygen(&key);
                    printf("***%X %X***\n", key, key[1]);
                    printf("***%X %X***\n", key2, key2[1]);
                    printf("***%X %X***\n", &key3, key3[1]);
                    printf("%02X:%02X:%02X:%02X\n", key3[0], key3[1], key3[2], key3[3]);
                    printf("%02X:%02X:%02X:%02X\n", key[0], key[1], key[2], key[3]);
    #endif SMOKE_TESTS
                    // printf("    &hydro_hash_hash -> %"PRIu64"\n", &hydro_hash_hash);
}



#include        "sx/atomic.h"
#include        "sx/os.h"
#include        "sx/threads.h"

// Mutex, for long-time data locks, for short-time locks use sx_lock_t in atomics.h
typedef sx_mutex            mtx;

#define mtx_init(mtx)           sx_mutex_init((mtx)) // void sx_mutex_init(sx_mutex* mutex);
#define mtx_release(mtx)        sx_mutex_release((mtx)) // void sx_mutex_release(sx_mutex* mutex);
#define mtx_lock(mtx)           sx_mutex_lock((mtx)) // void sx_mutex_lock(sx_mutex* mutex);
#define mtx_unlock(mtx)         sx_mutex_unlock(mtx) // void sx_mutex_unlock(sx_mutex* mutex);
#define mtx_trylock(mtx)        sx_mutex_trylock((mtx)) //bool sx_mutex_trylock(sx_mutex* mutex);


// Semaphore
typedef sx_sem              mtsem;

#define mtsem_init(sem_p)               sx_semaphore_init((sem_p))    // -> void sx_semaphore_init(sx_sem* sem);
#define mtsem_release(sem_p)            sx_semaphore_release((sem_p))    // -> void sx_semaphore_release(sx_sem* sem);
#define mtsem_post(sem_p, int_count)    sx_semaphore_post((sem_p), int_count)    // -> void sx_semaphore_post(sx_sem* sem, int count sx_default(1));
#define mtsem_wait(sem_p, int_msecs)    sx_semaphore_wait((sem_p), int_msecs)    // -> bool sx_semaphore_wait(sx_sem* sem, int msecs sx_default(-1));


// Lock-Free single-producer/single-consumer self-contained-data queue
// Reference:
//      http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448?pgno=1

#define SPSC_Q_ALLOCATOR SX_ALLOCATOR

typedef sx_queue_spsc       spsc_q;

spsc_q* spsc_q_create (int item_sz, int capacity)             { return sx_queue_spsc_create(SPSC_Q_ALLOCATOR, item_sz, capacity); }
void    spsc_q_destroy    (spsc_q* q)                         { sx_queue_spsc_destroy(q, SPSC_Q_ALLOCATOR); }
bool    spsc_q_produce    (spsc_q* q, const void* data)       { return sx_queue_spsc_produce(q,data); }
bool    spsc_q_consume    (spsc_q* q, void* data)             { return sx_queue_spsc_consume(q,data); }
bool    spsc_q_grow       (spsc_q* q, const sx_alloc* alloc)  { return sx_queue_spsc_grow(q,SPSC_Q_ALLOCATOR); }
bool    spsc_q_full (const spsc_q* q)                   { return sx_queue_spsc_full(q); }


// jobs
#include        "sx/jobs.h"


#define JOB_ALLOCATOR SX_ALLOCATOR

// Job priority, higher priority jobs will run sooner
#define JOB_PRIO_HIGH               SX_JOB_PRIORITY_HIGH // = 0
#define JOB_PRIO_MED                SX_JOB_PRIORITY_NORMAL
#define JOB_PRIO_LOW                SX_JOB_PRIORITY_LOW
#define JOB_PRIO_COUNT              SX_JOB_PRIORITY_COUNT

typedef sx_job_t                    job_h;
typedef sx_job_cb                   job_cb_t;
typedef sx_job_thread_init_cb       job_ctor;
typedef sx_job_thread_shutdown_cb   job_dtor;
typedef sx_job_context              job_ctx_t;
typedef sx_job_context_desc         job_desc_t;
typedef sx_job_priority             job_prio_t;
                    // Job description, required for submitting jobs
                // int num_threads;
                    // number of worker threads to spawn (default: num_cpu_cores-1)
                // int max_fibers;
                    // maximum fibers that are allowed to be active at the same time (default: 64)
                // int fiber_stack_sz;
                    // fiber stack size (default: 1mb)
                // sx_job_thread_init_cb* thread_init_cb;
                    // callback function that will be called on initiaslization of each worker thread
                // sx_job_thread_shutdown_cb* thread_shutdown_cb;
                    // callback functions that will be called on the shutdown of each worker thread
                // void* thread_user_data;
                    // user-data to be passed to callback functions above

job_ctx_t *     job_ctx_create(const job_desc_t* desc) { return sx_job_create_context(JOB_ALLOCATOR, desc); }
void            job_ctx_destroy(job_ctx_t* ctx) { sx_job_destroy_context(ctx, JOB_ALLOCATOR); }
job_h           job_dispatch(
                    job_ctx_t* ctx,
                    i32 count,
                    job_cb_t* job_cb,
                    void* user,
                    job_prio_t prio, // sx_default(SX_JOB_PRIORITY_NORMAL),
                    u32 tags) //sx_default(0)
                {   return sx_job_dispatch(ctx,count,job_cb,user,prio,tags); }
void            job_await(job_ctx_t* ctx, job_h h) { sx_job_wait_and_del(ctx, h); }
                    // (Thread-Safe) Blocks, waits on dispatched job and then deletes
bool            job_check(job_ctx_t* ctx, job_h h) { return sx_job_test_and_del(ctx, h); }
                    // (Thread-Safe) non-blocking function, checks if job is finished
int             job_worker_count(job_ctx_t* ctx) { return sx_job_num_worker_threads(ctx); }
                    // number of worker threads running (does not include main thread)
                    //  If job is finished, returns `true` and deletes the job handle
                    //  Else, returns `false` immediately
int             job_this_idx(job_ctx_t* ctx) { return sx_job_thread_index(ctx); }
                    // Get current working thread's index (0..num_workers)
int             job_this_id(job_ctx_t* ctx) { return sx_job_thread_id(ctx); }
                    // Get current working thread's Os Id
void            job_this_tag(job_ctx_t* ctx, u32 tags) { sx_job_set_current_thread_tags(ctx, tags); }



#include        <signal.h>
#include        <unistd.h>

static char volatile * volatile   _js_eval_src[1];
static char volatile * volatile   _js_eval_ret;

void            js_eval(char * src, size_t len)
{
                    printf("++ %s\n",src);
                    _js_eval_src[0] = malloc(len);
                    memcpy(_js_eval_src[0], src, len);
                    kill(getpid(), SIGUSR2);
}

static js_val_t _js_eval_flush(napi_env env, napi_callback_info ctx)
{
                    // kill(getpid(), SIGUSR2);
                    char src[] = "console.log('!!!!!!!!')";
                    js_eval(src, strlen(src) + 1);
                    return NULL;
}

static js_val_t _js_eval_run(napi_env env, napi_callback_info ctx)
{
                    printf("** %s\n",_js_eval_src[0]);
                    if (_js_eval_src[0] != NULL) {
                        js_val_t result;
                        JS_VAL_UTF8(src, _js_eval_src[0], strlen(_js_eval_src[0]))
                        NAPI_CALL(env, napi_run_script(env, src, &result));
                        free(_js_eval_src[0]);
                        _js_eval_src[0] = NULL;
                    }
                    // _js_eval_src = NULL;

                    return NULL;
}

void            __job_setup(napi_env env, js_val_t exports)
{
                    JS_EXPORT_FN_AS(_js_eval_run,       _eval_run)
                    JS_EXPORT_FN_AS(_js_eval_flush,     _eval_flush)
}


/*-----------------------------------------------------------------------------+
 |  Extended Memory Semantics (EMS)                            Version 1.3.0   |
 |  Synthetic Semantics       http://www.synsem.com/       mogill@synsem.com   |
 +-----------------------------------------------------------------------------+
 |  Copyright (c) 2011-2014, Synthetic Semantics LLC.  All rights reserved.    |
 |  Copyright (c) 2015-2016, Jace A Mogill.  All rights reserved.              |
 |                                                                             |
 | Redistribution and use in source and binary forms, with or without          |
 | modification, are permitted provided that the following conditions are met: |
 |    * Redistributions of source code must retain the above copyright         |
 |      notice, this list of conditions and the following disclaimer.          |
 |    * Redistributions in binary form must reproduce the above copyright      |
 |      notice, this list of conditions and the following disclaimer in the    |
 |      documentation and/or other materials provided with the distribution.   |
 |    * Neither the name of the Synthetic Semantics nor the names of its       |
 |      contributors may be used to endorse or promote products derived        |
 |      from this software without specific prior written permission.          |
 |                                                                             |
 |    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS      |
 |    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT        |
 |    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    |
 |    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SYNTHETIC         |
 |    SEMANTICS LLC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,   |
 |    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,      |
 |    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR       |
 |    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF   |
 |    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     |
 |    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS       |
 |    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.             |
 |                                                                             |
 +-----------------------------------------------------------------------------*/
#include "ems.h"
// #include "ems_alloc.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define BUDDY_UNUSED 0
#define BUDDY_USED   1
#define BUDDY_SPLIT  2
#define BUDDY_FULL   3


//-----------------------------------------------------------------------------+
//  Allocate memory for testing -- 
//  Performed as part of new EMS object initialization
struct emsMem *emsMem_new(int level) {
    size_t size = 1UL << level;
    printf("emsMem_new: malloc sz=%ld\n", size);
    struct emsMem *self = (struct emsMem *) malloc(sizeof(struct emsMem) + sizeof(uint8_t) * ((size << 1) - 2));
    self->level = level;
    memset(self->tree, BUDDY_UNUSED, (size << 1) - 1);
    return self;
}

//-----------------------------------------------------------------------------+
//  De-allocate memory from testing, not part of EMS object
void emsMem_delete(struct emsMem *self) {
    free(self);
}


//-----------------------------------------------------------------------------+
//  Pow-2 utility functions for 64 bit
size_t emsNextPow2(int64_t x) {
    if (__builtin_popcountll(x) == 1) return x;
    // TODO: Yes, that will overflow on 63 bit numbers.  Hopefully someone will rewrite this by then.
    //  fprintf(stderr, ">>>lz=%d   shift=%d\n", __builtin_clzl(x), (64 - __builtin_clzl(x)));
    return (1UL << (64 - __builtin_clzll(x)));
}


static inline
int64_t _ems_index_offset(int64_t index, int32_t level, int64_t max_level) {
    return ((index + 1) - (1UL << level)) << (max_level - level);
}


//-----------------------------------------------------------------------------+
//  Mark the parent buddy of this buddy
static
void _ems_mark_parent(struct emsMem *self, int64_t index) {
    for (; ;) {
        int64_t buddy = index - 1 + ((index & 1) << 1);
        if (buddy > 0 && (self->tree[buddy] == BUDDY_USED || self->tree[buddy] == BUDDY_FULL)) {
            index = ((index + 1) >> 1) - 1;
            self->tree[index] = BUDDY_FULL;
        } else {
            return;
        }
    }
}


//-----------------------------------------------------------------------------+
//  Allocate new memory from the EMS heap
size_t emsMem_alloc(struct emsMem *self, size_t bytesRequested) {
    size_t size = emsNextPow2(
        (bytesRequested + (EMS_MEM_BLOCKSZ - 1)) >> EMS_MEM_BLOCKSZ_P2);
    if (size == 0)
        size++;

    // printf("emsMem_alloc level=%i, bytes-req=%i -> size=%i \n",self->level, bytesRequested, size);
    size_t length = 1UL << self->level;

    //  fprintf(stderr, "emsMem_alloc: self=%x   size=%ld   s=%ld    len=%ld\n", self, size, s, length);

    if (size > length)
        return -1;

    int64_t index = 0;
    int32_t level = 0;

    while (index >= 0)
    {
        if (size == length) {
            if (self->tree[index] == BUDDY_UNUSED) {
                self->tree[index] = BUDDY_USED;
                _ems_mark_parent(self, index);
                return (
                    (_ems_index_offset(index, level, self->level))
                    << EMS_MEM_BLOCKSZ_P2
                );
            }
        } else {
            // size < length
            switch (self->tree[index]) {
                case BUDDY_USED:
                case BUDDY_FULL:
                    break;
                case BUDDY_UNUSED:
                    // split first
                    self->tree[index] = BUDDY_SPLIT;
                    self->tree[(index << 1) + 1] = BUDDY_UNUSED;
                    self->tree[(index << 1) + 2] = BUDDY_UNUSED;
                default:
                    index = (index << 1) + 1;
                    length = (length >> 1);
                    level++;
                    continue;
            }
        }
        if (index & 1) {
            ++index;
            continue;
        }
        for (; ;) {
            level--;
            length = (length << 1);
            index = ((index + 1) >> 1) - 1;
            if (index < 0)
                return -1;
            if (index & 1) {
                ++index;
                break;
            }
        }
    }

    return -1;
}


//-----------------------------------------------------------------------------+
//  Combine two buddies into one node
static void _ems_free_combine(struct emsMem *self, int64_t index) {
    for (; ;) {
        int64_t buddy = index - 1 + ((index & 1) << 1);
        if (buddy < 0 || self->tree[buddy] != BUDDY_UNUSED) {
            self->tree[index] = BUDDY_UNUSED;
            while (
                ((index = ((index + 1) >> 1) - 1) >= 0)
                && self->tree[index] == BUDDY_FULL
            ) {
                self->tree[index] = BUDDY_SPLIT;
            }
            return;
        }
        index = ((index + 1) >> 1) - 1;
    }
}


//-----------------------------------------------------------------------------+
//  Release EMS memory back to the heap for reuse
void ems_free(struct emsMem *self, size_t offset) {
    offset /= EMS_MEM_BLOCKSZ;
    assert(offset < (1UL << self->level));
    size_t left = 0;
    size_t length = 1UL << self->level;
    int64_t index = 0;

    for (; ;) {
        switch (self->tree[index]) {
            case BUDDY_USED:
                assert(offset == left);
                _ems_free_combine(self, index);
                return;
            case BUDDY_UNUSED:
                assert(0);
                return;
            default:
                length = length >> 1;
                if (offset < left + length) {
                    index = (index << 1) + 1;
                } else {
                    left += length;
                    index = (index << 1) + 2;
                }
                break;
        }
    }
}


//-----------------------------------------------------------------------------+
//  Return the size of a block of memory
size_t emsMem_size(struct emsMem *self, size_t offset) {
    assert(offset < (1UL << self->level));
    size_t left = 0;
    size_t length = 1UL << self->level;
    size_t index = 0;

    for (; ;) {
        switch (self->tree[index]) {
            case BUDDY_USED:
                assert(offset == left);
                return length << EMS_MEM_BLOCKSZ_P2;
            case BUDDY_UNUSED:
                assert(0);
                return length << EMS_MEM_BLOCKSZ_P2;
            default:
                length = length >> 1;
                if (offset < left + length) {
                    index = (index << 1) + 1;
                } else {
                    left += length;
                    index = (index << 1) + 2;
                }
                break;
        }
    }
}


//-----------------------------------------------------------------------------+
//  Diagnostic state dump
static void EMS_dump(struct emsMem *self, size_t index, int32_t level) {
    switch (self->tree[index]) {
        case BUDDY_UNUSED:
            printf("(%lld:%ld)", (long long int) _ems_index_offset(index, level, self->level),
                   1UL << (self->level - level));
            break;
        case BUDDY_USED:
            printf("[%lld:%ld]", (long long int) _ems_index_offset(index, level, self->level),
                   1UL << (self->level - level));
            break;
        case BUDDY_FULL:
            printf("{");
            EMS_dump(self, index * 2 + 1, level + 1);
            EMS_dump(self, index * 2 + 2, level + 1);
            printf("}");
            break;
        default:
            printf("(");
            EMS_dump(self, index * 2 + 1, level + 1);
            EMS_dump(self, index * 2 + 2, level + 1);
            printf(")");
            break;
    }
}

void emsMem_dump(struct emsMem *self) {
    EMS_dump(self, 0, 0);
    printf("\n");
}
// #include <assert.h>
#include "ems.h"

//==================================================================
//  Resolve External Declarations
//
// int EMSmyID = -1;   // EMS Process ID
char   *emsBufs[EMS_MAX_N_BUFS] = { NULL };
size_t  emsBufLengths[EMS_MAX_N_BUFS] = { 0 };
char    emsBufFilenames[EMS_MAX_N_BUFS][MAX_FNAME_LEN] = { { 0 } };

//==================================================================
//  Wrappers around memory allocator to ensure mutual exclusion
//  The buddy memory allocator is not thread safe, so this is necessary for now.
//  It can be used as the hook to wrap the non-threaded allocator with a
//  multiplexor of several independent regions.
//
//  Returns the byte offset in the EMS data space of the space allocated
//

// #define         EMS_ALLOC(addr, len, bufChar, errmsg, retval) \
//                     addr = emsMutexMem_alloc(\
//                         EMS_MEM_MALLOCBOT(bufChar),\
//                         len,\
//                         (char*) &bufInt64[EMScbData(EMS_ARR_MEM_MUTEX)]);\

// ((struct emsMem *) &bufChar[ bufInt64[ EMScbData(EMS_ARR_MALLOCBOT)] ])


#define _ems_malloc(len) \
    emsMutexMem_alloc(\
        &bufChar[bufInt64[EMScbData(EMS_ARR_MALLOCBOT)]],\
        len,\
        &bufInt64[EMScbData(EMS_ARR_MEM_MUTEX)])

#define EMS_MALLOC_ERR(len, errmsg) \
    fprintf(stderr,\
        "%s:%d (%s) ERROR: EMS memory allocation of len(%zx) failed: %s\n",\
        __FILE__, __LINE__, __FUNCTION__, len, errmsg);


size_t ems_malloc(
    const int id,
    size_t len)
{
    volatile char *bufChar = emsBufs[id];
    volatile int64_t *bufInt64 = bufChar;
    return _ems_malloc(len);
}

size_t emsMutexMem_alloc(
    struct emsMem *heap,   // Base of EMS malloc structs
    size_t len,            // Number of bytes to allocate
    volatile char *mutex)  // Pointer to the mem allocator's mutex
{
    RESET_NAP_TIME()

    // Wait until we acquire the allocator's mutex
    while (!__sync_bool_compare_and_swap(mutex, EMS_TAG_EMPTY, EMS_TAG_FULL))
        NANOSLEEP;

    size_t retval = emsMem_alloc(heap, len);
    *mutex = EMS_TAG_EMPTY; // Unlock the allocator's mutex
    return (retval);
}

void emsMutexMem_free(struct emsMem *heap,  // Base of EMS malloc structs
                      size_t addr,          // Offset of alloc'd block in EMS memory
                      volatile char *mutex) // Pointer to the mem allocator's mutex
{
    RESET_NAP_TIME()
    // Wait until we acquire the allocator's mutex
    while (!__sync_bool_compare_and_swap(mutex, EMS_TAG_EMPTY, EMS_TAG_FULL))
        NANOSLEEP;

    ems_free(heap, addr);
    *mutex = EMS_TAG_EMPTY;   // Unlock the allocator's mutex
}





//==================================================================
//  Convert any type of key to an index
//
int64_t ems_key2hash(EMSvalueType *key)
{
    int64_t idx = 0;
    switch (key->type)
    {
        case EMS_TYPE_BOOLEAN:
            if ((bool) key->value)  idx = 1;
            else                    idx = 0;
            break;
        case EMS_TYPE_INTEGER:
            idx = llabs((int64_t) key->value);
            break;
        case EMS_TYPE_FLOAT:
            idx = llabs((int64_t) key->value);
            break;
        case EMS_TYPE_STRING:
        case EMS_TYPE_BUFFER:
        case EMS_TYPE_JSON:
            idx = EMShashString(key->value, key->length);
            break;
        case EMS_TYPE_UNDEFINED:
            fprintf(stderr, "EMS ERROR: ems_key2hash keyType is defined as Undefined\n");
            return -1;
        default:
            fprintf(stderr, "EMS ERROR: ems_key2hash keyType(%d) is unknown\n", key->type);
            return -1;
    }
    return idx;
}

inline int64_t ems_arr_hash2index(int64_t *bufInt64, int64_t hash) {
    return hash % bufInt64[EMScbData(EMS_ARR_NELEM)];
}

int64_t ems_map_hash2index(void *emsBuf, EMSvalueType *key, int64_t idx, int64_t * timer)
{
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    volatile double *bufDouble = (double *) emsBuf;
    const char *bufChar = (char *) emsBuf;

    int nTries = 0;
    bool matched = false;
    bool notPresent = false;
    const int64_t capacity = bufInt64[EMScbData(EMS_ARR_NELEM)];
    EMStag_t mapTags;

    idx = idx % capacity;

    while (!matched && !notPresent)
    {
        // Wait until the map key is FULL, mark it busy while map lookup is performed
        mapTags.byte = EMStransitionFEtag(
            &bufTags[EMSmapTag(idx)], NULL,
            EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
            timer);
        if (NANO_DID_TIMEOUT(timer)) {
            return -1;
        }
        if (mapTags.tags.type == key->type)
        {
            switch (key->type)
            {
                case EMS_TYPE_BOOLEAN:
                case EMS_TYPE_INTEGER:
                    if ((int64_t) key->value == bufInt64[EMSmapData(idx)])
                        matched = true;
                    break;
                case EMS_TYPE_FLOAT: {
                    EMSulong_double alias;
                    alias.u64 = (uint64_t) key->value;
                    if (alias.d == bufDouble[EMSmapData(idx)])
                        matched = true;
                }
                    break;
                case EMS_TYPE_STRING:
                case EMS_TYPE_BUFFER:
                case EMS_TYPE_JSON: {
                    int64_t offset = bufInt64[EMSmapData(idx)];
                    uint32_t * ptr = EMSheapPtr(offset);
                    size_t length = ptr[0];
                    if ((length == key->length) &&
                        memcmp(key->value, (ptr+1), length) == 0)
                    {
                        matched = true;
                    }
                    break;
                }
                case EMS_TYPE_UNDEFINED:
                    // Nothing hashed to this map index yet, so the key does not exist
                    notPresent = true;
                    break;
                default:
                    fprintf(stderr, "EMS ERROR: EMSreadIndexMap: Unknown mem type\n");
                    matched = true;
            }
        }
        if (mapTags.tags.type == EMS_TYPE_UNDEFINED)
            notPresent = true;
        bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
        if (!matched)
        {
            if (++nTries < MAX_OPEN_HASH_STEPS)
                return -1;

            //  No match, set this Map entry back to full and try again
            idx++;
            // Instead of idx = idx % capacity;
            idx *= (idx != capacity);

        }
    }
    if (!matched)
        return -1;

    return idx;
    // return idx % capacity;
}

// TODO: handle timer in EMSkey2index
#define EMSkey2index(emsBuf, key, is_mapped) ((is_mapped)\
    ? ems_map_key2index((emsBuf), (key), NULL)\
    : ems_arr_key2index((key)))

#define ems_arr_key2index(key)\
    ems_key2hash(key)

int64_t ems_map_key2index(void *emsBuf, EMSvalueType *key, int64_t * timer)
{
    return ems_map_hash2index(emsBuf, key, ems_key2hash(key), timer);
}



#if 0
//==================================================================
//  Callback for destruction of an EMS array
//
static void EMSarrFinalize(char *data, void *hint) {
    //fprintf(stderr, "%d: EMSarrFinalize  data=%lx  hint=%lx %" PRIu64 "\n", EMSmyID, data, hint, hint);
    munmap(data, (size_t) hint);

    {
    v8::HandleScope scope;
    EMS_DECL(args);
    size_t length = buffer->handle_->GetIndexedPropertiesExternalArrayDataLength();

    if(-1 == munmap(emsBuf, length)) return v8::False();
    
    // JQM TODO  shoud unlink here?
    fprintf(stderr, "Unmap -- should also unlink\n");
    buffer->handle_->SetIndexedPropertiesToExternalArrayData(NULL, v8::kExternalUnsignedByteArray, 0);
    buffer->handle_->Set(length_symbol, v8::Integer::NewFromUnsigned(0));
    buffer->handle_.Dispose();
    args.This()->Set(length_symbol, v8::Integer::NewFromUnsigned(0));
    
    return v8::True();
  }

}
#endif




//==================================================================
//  Wait until the FE tag is a particular state, then transition it to the new state
//  Return new tag state, old tag state if timed out
//
unsigned char EMStransitionFEtag(
    EMStag_t volatile *tag,
    EMStag_t volatile *mapTag,
    unsigned char oldFE,
    unsigned char newFE,
    unsigned char oldType,
    int64_t *timer)
{
    NANO_SET_TIMEOUT(timer)
    EMStag_t oldTag;           //  Desired tag value to start of the transition
    EMStag_t newTag;           //  Tag value at the end of the transition
    EMStag_t volatile memTag;  //  Tag value actually stored in memory
    memTag.byte = tag->byte;
    while (oldType == EMS_TAG_ANY || memTag.tags.type == oldType)
    {
        // Copy current type and RW count information
        oldTag.byte = memTag.byte;
        // Set the desired start tag state
        oldTag.tags.fe = oldFE;
        // Copy current type and RW count information
        newTag.byte = memTag.byte;
        // Set the final tag state
        newTag.tags.fe = newFE;
        //  Attempt to transition the state from old to new
        memTag.byte =  __sync_val_compare_and_swap(&(tag->byte), oldTag.byte, newTag.byte);

        if (memTag.byte == oldTag.byte)
            return (newTag.byte);

        if (mapTag) {
            // else Allow preemptive map acquisition while waiting for data
            mapTag->tags.fe = EMS_TAG_FULL;

            NANO_TIMEOUT_SLEEP_CATCH        { return tag->byte; }

            EMStransitionFEtag(
                mapTag, NULL,
                EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
                timer);
            if (NANO_DID_TIMEOUT(timer))    { return tag->byte; }
        }
        else {
            NANO_TIMEOUT_SLEEP_CATCH        { return tag->byte; }
        }

        // Re-load tag in case was transitioned by another thread
        memTag.byte = tag->byte;

    }
    return (memTag.byte);
}


//==================================================================
//  Hash a string into an integer
//
int64_t EMShashString(const void *data, int32_t len) {
    return sx_hash_xxh64(data, len, 0);
    // TODO BUG MAGIC Max key length
    // int charN = 0;
    // uint64_t hash = 0;
    // while (charN < len) {
    //     hash = key[charN] + (hash << 6) + (hash << 16) - hash;
    //     charN++;
    // }
    // hash *= 1191613;  // Further scramble to prevent close strings from having close indexes
    // return (llabs((int64_t) hash));
}


//==================================================================
//  Find the matching map key, if not present, find the
//  next available open address.
//  Reads map key when full and marks Busy to perform comparisons,
//  if it is not a match the data is marked full again, but if it does
//  match, the map key is left empty and this function
//  returns the index of an existing or available array element.
//
int64_t EMSwriteIndexMap(const int mmapID, EMSvalueType *key, int64_t * timer)
{
    char *emsBuf = emsBufs[mmapID];
    volatile int64_t  *bufInt64  = (int64_t *) emsBuf;

    int64_t hash = ems_key2hash(key);
    int64_t idx;
    if (!EMSisMapped) {
        idx = ems_arr_hash2index(bufInt64, hash);
        if (idx < 0 || idx >= bufInt64[EMScbData(EMS_ARR_NELEM)]) {
            fprintf(stderr, "Wasn't mapped do bounds check\n");
            return -1;
        }
        return idx;
    }
    else {
        idx = ems_map_hash2index(emsBuf, key, hash, timer);
         //  If the key already exists, use it
        if (idx > 0)
            return idx;
        if (NANO_DID_TIMEOUT(timer)) {
            return -1;
        }

        idx = ems_arr_hash2index(bufInt64, hash); // ...

        volatile char     *bufChar   = emsBuf;
        volatile EMStag_t *bufTags   = (EMStag_t *) emsBuf;
        volatile double   *bufDouble = (double *) emsBuf;
        EMStag_t mapTags;
        int nTries = 0;
        int matched = false;
        while (nTries < MAX_OPEN_HASH_STEPS && !matched) {
            // Wait until the map key is FULL, mark it busy while map lookup is performed
            mapTags.byte = EMStransitionFEtag(
                &bufTags[EMSmapTag(idx)], NULL,
                EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
                timer);
            if (NANO_DID_TIMEOUT(timer)) {
                return -1;
            }

            mapTags.tags.fe = EMS_TAG_FULL;  // When written back, mark FULL

            if (
                mapTags.tags.type == key->type ||
                mapTags.tags.type == EMS_TYPE_UNDEFINED)
            {
                switch (mapTags.tags.type)
                {
                    case EMS_TYPE_BOOLEAN:
                        if ((int64_t) key->value == (bufInt64[EMSmapData(idx)] != 0)) {
                            matched = true;
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        }
                        break;
                    case EMS_TYPE_INTEGER:
                        if ((int64_t) key->value == bufInt64[EMSmapData(idx)]) {
                            matched = true;
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        }
                        break;
                    case EMS_TYPE_FLOAT: {
                        EMSulong_double alias;
                        alias.u64 = (uint64_t) key->value;
                        if (alias.d == bufDouble[EMSmapData(idx)]) {
                            matched = true;
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        }
                    }
                        break;
                    case EMS_TYPE_STRING:
                    case EMS_TYPE_BUFFER:
                    case EMS_TYPE_JSON: {
                        int64_t offset = bufInt64[EMSmapData(idx)];
                        uint32_t * ptr = EMSheapPtr(offset);
                        size_t length = ptr[0];
                        if ((length == key->length) &&
                            memcmp(key->value, (ptr+1), length) == 0)
                        {
                            matched = true;
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        }
                        // int64_t keyStrOffset = bufInt64[EMSmapData(idx)];
                        // if (strcmp((const char *) key->value, (const char *) EMSheapPtr(keyStrOffset)) == 0) {
                        //     matched = true;
                        //     bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        // }
                    }
                        break;
                    case EMS_TYPE_UNDEFINED:
                        // This map key index is still unused, so there was no match and a new
                        // mapped element must be allocated to perform the tag bit transitions upon
                        bufTags[EMSmapTag(idx)].tags.type = key->type;
                        switch (key->type) {
                            case EMS_TYPE_BOOLEAN:
                                bufInt64[EMSmapData(idx)] = ((int64_t) key->value !=  0);
                                break;
                            case EMS_TYPE_INTEGER:
                                bufInt64[EMSmapData(idx)] = (int64_t) key->value;
                                break;
                            case EMS_TYPE_FLOAT: {
                                EMSulong_double alias;
                                alias.u64 = (uint64_t) key->value;
                                bufDouble[EMSmapData(idx)] = alias.d;
                            }
                                break;
                            case EMS_TYPE_STRING:
                            case EMS_TYPE_BUFFER:
                            case EMS_TYPE_JSON: {
                                int64_t offset = _ems_malloc(key->length + 8);
                                if (offset < 0) {
                                    EMS_MALLOC_ERR(key->length + 8, "EMSwriteIndexMap(string): out of memory to store string")
                                    return -1;
                                }
                                bufInt64[EMSmapData(idx)] = offset;
                                uint32_t * heap_ptr = EMSheapPtr(offset);
                                heap_ptr[0] = key->length;
                                memcpy((heap_ptr+1), key->value, key->length);
                                // int64_t textOffset = _ems_malloc(key->length + 1);
                                // if (textOffset < 0) {
                                //     EMS_MALLOC_ERR(key->length + 1, "EMSwriteIndexMap(string): out of memory to store string")
                                //     return -1;
                                // }
                                // bufInt64[EMSmapData(idx)] = textOffset;
                                // strcpy((char *) EMSheapPtr(textOffset), (const char *) key->value);
                            }
                                break;
                            case EMS_TYPE_UNDEFINED:
                                bufInt64[EMSmapData(idx)] = 0xdeadbeef;
                                break;
                            default:
                                fprintf(stderr, "EMS ERROR: EMSwriteIndexMap: unknown arg type\n");
                        }
                        matched = true;
                        break;
                    default:
                        fprintf(stderr, "EMS ERROR: EMSwriteIndexMap: Unknown tag type (%d) on map key\n", mapTags.tags.type);
                        matched = true;
                }
            }

            if (!matched) {
                // No match so set this key map back to full and try the next entry
                bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;

                ++nTries;
                // TODO:  MAX_OPEN_HASH_STEPS should be computed?

                idx = ems_arr_hash2index(bufInt64, idx + 1);
            }
        }
        if (nTries >= MAX_OPEN_HASH_STEPS) {
            idx = -1;
            fprintf(stderr, "EMSwriteIndexMap ran out of key mappings (ntries=%d)\n", nTries);
        }
    }

    return idx;
}



//==================================================================
//  Read EMS memory, enforcing Full/Empty tag transitions
bool EMSreadUsingTags(const int mmapID,
                      EMSvalueType *key, // Index to read from
                      EMSvalueType *returnValue,
                      unsigned char initialFE, // Block until F/E tags are this value
                      unsigned char finalFE, // Set the tag to this value when done
                      int64_t* timer) // default = 0 = forever
{
    NANO_SET_TIMEOUT(timer)

    void *emsBuf = emsBufs[mmapID];
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;

    returnValue->length = 0;
    returnValue->type  = EMS_TYPE_UNDEFINED;
    // returnValue->value = (void *) 0xdeafbeef;  // TODO: Should return default value even when not doing write allocate

    int64_t idx;
    bool is_mapped = EMSisMapped;

    if (is_mapped)
    {
        //  Allocate on Write, writes include modification of the tag:
        //  If the EMS object being read is undefined and we're changing the f/e state
        //  then allocate the undefined object and set the state.  If the state is
        //  not changing, do not allocate the undefined element.
        if (finalFE != EMS_TAG_ANY){
            if ((idx = EMSwriteIndexMap(mmapID, key, timer)) < 0) {
                if (NANO_DID_TIMEOUT(timer)) {
                    // printf("******** EMSwriteIndexMap NANO_DID_TIMEOUT %lli \n", timer ? *timer : 0ll);
                     return false;
                }
                fprintf(stderr, "EMSreadUsingTags: Unable to allocate on read for new map index\n");
                return false;
            }
        }
        else {
            if ((idx = ems_map_key2index(emsBuf, key, timer)) < 0) {
                if (NANO_DID_TIMEOUT(timer)) {
                    // printf("******** ems_map_key2index NANO_DID_TIMEOUT %lli \n", timer ? *timer : 0ll);
                    return false;
                }
                return true;
            }
        }
    }
    else {
        idx = ems_arr_key2index(key);
        if (idx < 0 || idx >= bufInt64[EMScbData(EMS_ARR_NELEM)]) {
            fprintf(stderr, "EMSreadUsingTags: index out of bounds\n");
            return false;
        }
    }

    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    volatile double *bufDouble = (double *) emsBuf;
    const char *bufChar = (const char *) emsBuf;
    EMStag_t newTag, oldTag, memTag;
    const int64_t data_tag_idx = EMSdataTag(idx);
    while(true)
    {
        memTag.byte = bufTags[data_tag_idx].byte;
        //  Wait until FE tag is not FULL
        if (
                initialFE == EMS_TAG_ANY ||
            (
                initialFE != EMS_TAG_RW_LOCK &&
                memTag.tags.fe == initialFE
            ) ||
            (
                initialFE == EMS_TAG_RW_LOCK &&
                (  (
                        memTag.tags.fe == EMS_TAG_RW_LOCK &&
                        newTag.tags.rw < EMS_RW_NREADERS_MAX
                    ) ||
                    memTag.tags.fe == EMS_TAG_FULL
                ) &&
                (
                    memTag.tags.rw < ((1 << EMS_TYPE_NBITS_RW) - 1) // EMS_RW_NREADERS_MAX /???
                )  // Counter is already saturated
            )
        )
        {
            newTag.byte = memTag.byte;
            oldTag.byte = memTag.byte;
            newTag.tags.fe = EMS_TAG_BUSY;

            if (initialFE == EMS_TAG_RW_LOCK)
                newTag.tags.rw++; // TODO: ??? increment in loop?
            else
                oldTag.tags.fe = initialFE;

            //  Transition FE from FULL to BUSY
            if (initialFE == EMS_TAG_ANY ||
                __sync_bool_compare_and_swap(
                    &(bufTags[data_tag_idx].byte),
                    oldTag.byte, newTag.byte
                )
            ){
                // Under BUSY lock:
                //   Read the data, then reset the FE tag, then return the original value in memory
                newTag.tags.fe = finalFE;
                returnValue->type  = newTag.tags.type;
                switch (newTag.tags.type)
                {
                    case EMS_TYPE_UNDEFINED: {
                        // returnValue->value = (void *) 0xcafebeef;
                        if (finalFE != EMS_TAG_ANY)
                            bufTags[data_tag_idx].byte = newTag.byte;
                        if (EMSisMapped)
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        return true;
                    }
                    case EMS_TYPE_BOOLEAN: {
                        *((bool *)returnValue->value) = bufInt64[EMSdataData(idx)];
                        // returnValue->value = (void *) (bufInt64[EMSdataData(idx)] != 0);
                        if (finalFE != EMS_TAG_ANY)
                            bufTags[data_tag_idx].byte = newTag.byte;
                        if (EMSisMapped)
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        return true;
                    }
                    case EMS_TYPE_INTEGER: {
                        *((int64_t *)returnValue->value) = bufInt64[EMSdataData(idx)];
                        // returnValue->value = (void *) bufInt64[EMSdataData(idx)];
                        if (finalFE != EMS_TAG_ANY)
                            bufTags[data_tag_idx].byte = newTag.byte;
                        if (EMSisMapped)
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        return true;
                    }
                    case EMS_TYPE_FLOAT: {
                        EMSulong_double alias;
                        alias.d = bufDouble[EMSdataData(idx)];
                        *((uint64_t *)returnValue->value) = alias.u64;
                        // returnValue->value = (void *) alias.u64;
                        if (finalFE != EMS_TAG_ANY)
                            bufTags[data_tag_idx].byte = newTag.byte;
                        if (EMSisMapped)
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                        return true;
                    }
                    case EMS_TYPE_STRING:
                    case EMS_TYPE_BUFFER:
                    case EMS_TYPE_JSON: {
                        // printf("read idx=%i datad=%i bi64=%i\n",idx,EMSdataData(idx), bufInt64[EMSdataData(idx)]);

                        // uint32_t * heap_ptr = EMSheapPtr(bufInt64[EMSdataData(idx)]);
                        // returnValue->length = heap_ptr[0];
                        // returnValue->value = (void *)(heap_ptr + 1);
                        int64_t offset = bufInt64[EMSdataData(idx)];
                        const uint32_t * ptr = EMSheapPtr(offset);
                        int32_t len = ptr[0];
                        returnValue->length = len;
                        memcpy(returnValue->value, (ptr+1), len);

                        if (finalFE != EMS_TAG_ANY)
                            bufTags[data_tag_idx].byte = newTag.byte;
                        if (EMSisMapped)
                            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;

                        return true;
                    }
                    // case EMS_TYPE_JSON:
                    // case EMS_TYPE_STRING: {
                    //     returnValue->value = (void *) EMSheapPtr(bufInt64[EMSdataData(idx)]);
                    //     returnValue->length = strlen((const char *)returnValue->value); // TODO:
                    //     if (finalFE != EMS_TAG_ANY)
                    //         bufTags[EMSdataTag(idx)].byte = newTag.byte;
                    //     if (EMSisMapped)
                    //         bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                    //     return true;
                    // }
                    default:
                        fprintf(stderr, "EMSreadUsingTags: unknown type (%d) read from memory\n", newTag.tags.type);
                        return false;
                }
            } //else {
                // Tag was marked BUSY between test read and CAS, must retry
            //}
        } //else {
            // Tag was already marked BUSY, must retry
        //}
        // CAS failed or memory wasn't in initial state, wait and retry.
        // Permit preemptive map acquisition while waiting for data.
        if (EMSisMapped) {
            bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
        }
        NANO_TIMEOUT_SLEEP_CATCH {
            // printf("******** NANO_TIMEOUT_SLEEP_CATCH NANO_DID_TIMEOUT %lli \n", timer ? *timer : 0ll);
            return false;
        }
        if (EMSisMapped) {
            EMStransitionFEtag(
                &bufTags[EMSmapTag(idx)], NULL,
                EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
                timer);
            if (NANO_DID_TIMEOUT(timer)) {
                // printf("******** END NANO_DID_TIMEOUT %lli \n", timer ? *timer : 0ll);
                return false;
            }
        }
    }
}

//==================================================================
//  Read under multiple readers-single writer lock
bool EMSreadRW(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue) {
    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_RW_LOCK, EMS_TAG_RW_LOCK, 0);
}


//==================================================================
//  Read when full and leave empty
bool EMSreadFE(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue) {
    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_FULL, EMS_TAG_EMPTY, 0);
}


//==================================================================
//  Read when full and leave Full
bool EMSreadFF(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue) {
    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_FULL, EMS_TAG_FULL, 0);
}


//==================================================================
//   Wrapper around read
bool EMSread(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue) {
    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_ANY, EMS_TAG_ANY, 0);
}


//==================================================================
//  Decrement the reference count of the multiple readers-single writer lock

int EMSreleaseRW(const int mmapID, EMSvalueType *key) {
    // TODO: add timeout support
    RESET_NAP_TIME()
    void *emsBuf = emsBufs[mmapID];
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    EMStag_t newTag, oldTag;
    int64_t idx = EMSkey2index(emsBuf, key, EMSisMapped);
    if (idx < 0 || idx >= bufInt64[EMScbData(EMS_ARR_NELEM)]) {
        fprintf(stderr, "EMSreleaseRW: invalid index (%" PRIi64 ")\n", idx);
        return -1;
    }
    const int64_t data_tag_idx = EMSdataTag(idx);
    while (true) {
        oldTag.byte = bufTags[data_tag_idx].byte;
        newTag.byte = oldTag.byte;
        if (oldTag.tags.fe == EMS_TAG_RW_LOCK) {
            //  Already under a RW lock
            if (oldTag.tags.rw == 0) {
                //  Assert the RW count is consistent with the lock state
                fprintf(stderr, "EMSreleaseRW: locked but Count already 0\n");
                return -1;
            } else {
                //  Decrement the RW reference count
                newTag.tags.rw--;
                //  If this is the last reader, set the FE tag back to full
                if (newTag.tags.rw == 0) { newTag.tags.fe = EMS_TAG_FULL; }
                //  Attempt to commit the RW reference count & FE tag
                if (__sync_bool_compare_and_swap(&(bufTags[data_tag_idx].byte), oldTag.byte, newTag.byte)) {
                    return (int) newTag.tags.rw;
                } else {
                    // Another thread decremented the RW count while we also tried
                }
            }
        } else {
            if (oldTag.tags.fe != EMS_TAG_BUSY) {
                // Assert the RW lock being release is not in some other state then RW_LOCK or BUSY
                fprintf(stderr, "EMSreleaseRW: The RW lock being released is in some other state then RW_LOCK or BUSY\n");
                return -1;
            }
        }
        // Failed to update the RW count, sleep and retry
        NANOSLEEP;
    }
}


//==================================================================
//  Write EMS honoring the F/E tags
bool EMSwriteUsingTags(int mmapID,
                       EMSvalueType *key,
                       EMSvalueType *value,
                       unsigned char initialFE, // Block until F/E tags are this value
                       unsigned char finalFE,// Set the tag to this value when done
                       int64_t* timer)
{
    NANO_SET_TIMEOUT(timer)

    char *emsBuf = emsBufs[mmapID];
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    volatile double *bufDouble = (double *) emsBuf;
    char *bufChar = emsBuf;
    EMStag_t newTag, oldTag, memTag;
    int64_t idx = EMSwriteIndexMap(mmapID, key,
        timer);
    if (NANO_DID_TIMEOUT(timer))
            return false;
    if (idx < 0) {
        fprintf(stderr, "EMSwriteUsingTags: index out of bounds\n");
        return false;
    }

    const int64_t data_tag_idx = EMSdataTag(idx);
    // Wait for the memory to be in the initial F/E state and transition to Busy
    if (initialFE != EMS_TAG_ANY) {
        volatile EMStag_t *maptag;
        if (EMSisMapped)
            maptag = &bufTags[EMSmapTag(idx)];
        else
            maptag = NULL;
        EMStransitionFEtag(
            &bufTags[data_tag_idx], maptag,
            initialFE, EMS_TAG_BUSY, EMS_TAG_ANY,
            timer);
        if (NANO_DID_TIMEOUT(timer))
            return false;
    }
    const int64_t capacity = bufInt64[EMScbData(EMS_ARR_NELEM)];
    idx = idx % capacity;
    while (true)
    {
        memTag.byte = bufTags[data_tag_idx].byte;
        //  Wait until FE tag is not BUSY
        if (
            initialFE != EMS_TAG_ANY ||
            finalFE == EMS_TAG_ANY ||
            memTag.tags.fe != EMS_TAG_BUSY
        ) {
            oldTag.byte = memTag.byte;
            newTag.byte = memTag.byte;
            if (finalFE != EMS_TAG_ANY)
                newTag.tags.fe = EMS_TAG_BUSY;
            //  Transition FE from !BUSY to BUSY
            if (
                // TODO: remove this if???  at least min in-loop branches
                initialFE != EMS_TAG_ANY ||
                finalFE == EMS_TAG_ANY ||
                __sync_bool_compare_and_swap(
                    &(bufTags[data_tag_idx].byte), oldTag.byte, newTag.byte
                )
            ) {
                //  If the old data was a string, free it because it will be overwritten
                // TODO: maybe make better?   maybe check if room to write next val???????
                if (
                    oldTag.tags.type == EMS_TYPE_STRING ||
                    oldTag.tags.type == EMS_TYPE_JSON ||
                    oldTag.tags.type == EMS_TYPE_BUFFER
                ) {
                    EMS_FREE(bufInt64[EMSdataData(idx)]);
                }

                // Store argument value into EMS memory
                switch (value->type)
                {
                    case EMS_TYPE_UNDEFINED:
                        bufInt64[EMSdataData(idx)] = 0xdeadbeef;
                        break;
                    case EMS_TYPE_INTEGER:
                        bufInt64[EMSdataData(idx)] = (int64_t) value->value;
                        break;
                    case EMS_TYPE_BOOLEAN:
                        bufInt64[EMSdataData(idx)] = (int64_t) value->value;
                        break;
                    case EMS_TYPE_FLOAT: {
                        EMSulong_double alias;
                        alias.u64 = (uint64_t) value->value;
                        bufDouble[EMSdataData(idx)] = alias.d;
                    }
                        break;
                    case EMS_TYPE_STRING:
                    case EMS_TYPE_BUFFER:
                    case EMS_TYPE_JSON: {
                        // LENGTH (32bit) PREFIX ENCODE
                        int64_t offset = _ems_malloc(value->length + 8);
                        if (offset < 0) {
                            EMS_MALLOC_ERR(value->length + 8, "EMSwriteUsingTags: out of memory to store buffer")
                            goto WRITE_FAILED;
                        }
                        bufInt64[EMSdataData(idx)] = offset;
                        uint32_t * heap_ptr = EMSheapPtr(offset);
                        heap_ptr[0] = value->length;
                        memcpy((heap_ptr+1), value->value, value->length);
                        break;
                    }
                    // case EMS_TYPE_JSON:
                    // case EMS_TYPE_STRING: {
                    //     int64_t textOffset = _ems_malloc(value->length + 1);
                    //     if (textOffset < 0) {
                    //         EMS_MALLOC_ERR(value->length + 8, "EMSwriteUsingTags: out of memory to store string")
                    //         return false;
                    //     }
                    //     bufInt64[EMSdataData(idx)] = textOffset;
                    //     strcpy(EMSheapPtr(textOffset), (const char *) value->value);
                    // }
                        break;
                    default:
                        fprintf(stderr, "EMSwriteUsingTags: Unknown arg type\n");
                        goto WRITE_FAILED;
                }

                oldTag.byte = newTag.byte;
                if (finalFE != EMS_TAG_ANY) {
                    newTag.tags.fe = finalFE;
                    newTag.tags.rw = 0;
                }
                newTag.tags.type = value->type;
                if (
                    finalFE != EMS_TAG_ANY
                    && bufTags[data_tag_idx].byte != oldTag.byte
                ) {
                    fprintf(stderr, "EMSwriteUsingTags: Lost tag lock while BUSY\n");
                    return false;
                }
                //  Set the tags for the data (and map, if used) back to full to finish the operation
                bufTags[data_tag_idx].byte = newTag.byte;
                if (EMSisMapped)
                    bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                return true;

WRITE_FAILED:
                bufTags[data_tag_idx].byte = memTag.byte;
                if (EMSisMapped)
                    bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
                return false;
            }
            else {
                // Tag was marked BUSY between test read and CAS, must retry
            }
        }
        else {
            // Tag was already marked BUSY, must retry
        }
        //  Failed to set the tags, sleep and retry
        NANO_TIMEOUT_SLEEP_CATCH {
            return false;
        }
    }
}


//==================================================================
//  WriteXF
bool EMSwriteXF(int mmapID, EMSvalueType *key, EMSvalueType *value) {
    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_FULL,0);
}

//==================================================================
//  WriteXE
bool EMSwriteXE(int mmapID, EMSvalueType *key, EMSvalueType *value) {
    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_EMPTY,0);
}

//==================================================================
//  WriteEF
bool EMSwriteEF(int mmapID, EMSvalueType *key, EMSvalueType *value) {
    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_EMPTY, EMS_TAG_FULL,0);
}

//==================================================================
//  Write
bool EMSwrite(int mmapID, EMSvalueType *key, EMSvalueType *value) {
    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_ANY,0);
}


//==================================================================
//  Set only the Full/Empty tag  from JavaScript 
//  without inspecting or modifying the data.
bool EMSsetTag(int mmapID, EMSvalueType *key, bool is_full) {
    void *emsBuf = emsBufs[mmapID];
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    EMStag_t tag;

    int64_t idx = EMSkey2index(emsBuf, key, EMSisMapped);
    if (idx<0 || idx >= bufInt64[EMScbData(EMS_ARR_NELEM)]) {
        return false;
    }
    const int64_t data_tag_idx = EMSdataTag(idx);
    tag.byte = bufTags[data_tag_idx].byte;
    tag.tags.fe = (is_full) ? EMS_TAG_FULL : EMS_TAG_EMPTY;
    bufTags[data_tag_idx].byte = tag.byte;
    return true;
}


//==================================================================
//  Release all the resources associated with an EMS array
bool ems_destroy(int mmapID, bool do_unlink) {
    void *emsBuf = emsBufs[mmapID];
    if(munmap(emsBuf, emsBufLengths[mmapID]) != 0) {
        fprintf(stderr, "ems_destroy: Unable to unmap memory\n");
        return false;
    }

    if (do_unlink) {
        if (unlink(emsBufFilenames[mmapID]) != 0) {
            fprintf(stderr, "ems_destroy: Unable to unlink file\n");
            return false;
        }
    }

    emsBufFilenames[mmapID][0] = 0;
    emsBufLengths[mmapID] = 0;
    emsBufs[mmapID] = NULL;
    return true;
}



//==================================================================
//  Return the key of a mapped object given the EMS index
// TODO:  BEWARE ... ->value = ....
bool EMSindex2key(int mmapID, int64_t idx, EMSvalueType *key) {
    void *emsBuf = emsBufs[mmapID];
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    char *bufChar = (char *) emsBuf;
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    volatile double *bufDouble = (double *) emsBuf;

    if(!EMSisMapped) {
        fprintf(stderr, "EMSindex2key: Unmapping an index but Array is not mapped\n");
        return false;
    }

    if (idx < 0 || idx >= bufInt64[EMScbData(EMS_ARR_NELEM)]) {
        fprintf(stderr, "EMSindex2key: index out of bounds\n");
        return false;
    }

    key->type = bufTags[EMSmapTag(idx)].tags.type;
    switch (key->type) {
        case EMS_TYPE_BOOLEAN:
        case EMS_TYPE_INTEGER: {
            key->value = (void *) bufInt64[EMSmapData(idx)];
            return true;
        }
        case EMS_TYPE_FLOAT: {
            EMSulong_double alias;
            alias.d = bufDouble[EMSmapData(idx)];
            key->value = (void *) alias.u64;
            return true;
        }
        case EMS_TYPE_BUFFER:
        case EMS_TYPE_STRING:
        case EMS_TYPE_JSON: {
            uint32_t * heap_ptr = EMSheapPtr(bufInt64[EMSmapData(idx)]);
            key->length = heap_ptr[0];
            key->value = heap_ptr + 1;
            // key->value = (void *)(EMSheapPtr(bufInt64[EMSmapData(idx)]));
            return true;
        }
        case EMS_TYPE_UNDEFINED: {
            key->value = NULL;
            return true;
        }
        default:
            fprintf(stderr, "EMSindex2key unknown type\n");
            return false;
    }
}


//==================================================================
//  Synchronize the EMS memory to persistent storage
//
bool EMSsync(int mmapID) {
    // resultIdx = msync((void*) emsBuf, pgsize, flags);
    printf("EMSsync() was called but stubbed out\n");
    return false;
}


//==================================================================
//  EMS Entry Point:   Allocate and initialize the EMS domain memory
//

// legacy api support
int EMSinitialize(int64_t nElements,     // 0
                  size_t heapSize,      // 1
                  bool useMap,           // 2
                  const char *filename,  // 3
                  bool persist,          // 4
                  bool useExisting,      // 5
                  bool doDataFill,       // 6
                  bool fillIsJSON,       // 7
                  EMSvalueType *fillValue,// 8
                  bool doSetFEtags,      // 9
                  bool setFEtagsFull,    // 10
                  int EMSmyIDarg,        // 11
                  bool pinThreads,       // 12
                  int32_t nThreads,      // 13
                  int32_t pctMLock ) {   // 14
    int32_t flags = (pctMLock & EMS_MLOCK_PCT_MASK)
        | (((int)useMap)*EMS_USE_MAP)
        | (((int)persist)*EMS_PERSIST)
        | (((int)doDataFill)*EMS_FILL)
        | (((int)doSetFEtags)*EMS_SET_E)
        | (((int)setFEtagsFull)*EMS_FILL_JSON)
        | (((int)pinThreads)*EMS_PIN_THREADS)
    ;
    // | (((int)fillIsJSON)*EMS_FILL_JSON)
    return EMSinit(
        nElements,
        heapSize,
        filename,
        EMSmyIDarg,
        nThreads,
        flags,
        fillValue
    );
}



/*
    ems_fill()

    EMStag_t tag;
    tag.tags.rw = 0;
    for (int64_t idx = 0; idx < nElements; idx++)
    {
        tag.tags.rw = 0;
        tag.tags.type = EMS_TYPE_UNDEFINED;

        bool doDataFill = _EMS_doDataFill != 0 ;
        size_t fillStrLen = 0;
        if (
            doDataFill  &&  (fillValue->type == EMS_TYPE_JSON || fillValue->type == EMS_TYPE_STRING || fillValue->type == EMS_TYPE_BUFFER)
            )
        {
            fillStrLen = fillValue->length;
        }

            tag.tags.type = fillValue->type;
            switch (tag.tags.type)
            {
                case EMS_TYPE_BOOLEAN:
                case EMS_TYPE_INTEGER:
                    bufInt64[EMSdataData(idx)] = (int64_t) fillValue->value;
                    break;
                case EMS_TYPE_FLOAT: {
                    EMSulong_double alias;
                    alias.u64 = (uint64_t) fillValue->value;
                    bufDouble[EMSdataData(idx)] = alias.d;
                }
                    break;
                case EMS_TYPE_UNDEFINED:
                    bufInt64[EMSdataData(idx)] = 0xdeadbeef;
                    break;
                case EMS_TYPE_BUFFER: {
                    int64_t byteOffset;
                    EMS_ALLOC(byteOffset, fillStrLen, bufChar, // TODO: + 1 NULL padding
                              "EMSinitialize: out of memory to store buffer", false);
                    bufInt64[EMSdataData(idx)] = byteOffset;
                    memcpy(EMSheapPtr(byteOffset), (const char *) fillValue->value, fillStrLen);
                    // strcpy(EMSheapPtr(byteOffset), (const char *) fillValue->value);
                }
                case EMS_TYPE_JSON:
                case EMS_TYPE_STRING: {
                    int64_t textOffset;
                    EMS_ALLOC(textOffset, fillStrLen + 1, bufChar,
                              "EMSinitialize: out of memory to store string", false);
                    bufInt64[EMSdataData(idx)] = textOffset;
                    strcpy(EMSheapPtr(textOffset), (const char *) fillValue->value);
                }
                    break;
                default:
                    fprintf(stderr, "EMSinitialize: fill type is unknown\n");
                    return -1;
            }
        }



        if (_EMS_doSetFEtags) {
            if (_EMS_setFEtagsFull)
                tag.tags.fe = EMS_TAG_FULL;
            else
                tag.tags.fe = EMS_TAG_EMPTY;
        }

        if (_EMS_doSetFEtags || doDataFill) {
            bufTags[EMSdataTag(idx)].byte = tag.byte;
        }



    */


size_t            ems_await_owner(
    const char *    path)
{
    static const struct
                    timespec sleep_time =
                    {
                        .tv_sec = 0,
                        .tv_nsec = 200000000
                    };
    struct timespec rem;
    struct stat     st;
                    // TODO: timeout?
                    // TODO: if ems_id == 0 dont wait
                    while (stat(path, &st) != 0)
                        nanosleep(&sleep_time, &rem);
                        // nanosleep(&sleep_time, NULL);
                    return st.st_size;
}


void ems_reset_map_tags(ems_id)
{
    volatile int64_t *bufInt64 = (int64_t *) emsBufs[ems_id];
    volatile EMStag_t *bufTags = (EMStag_t *) emsBufs[ems_id];
    int64_t nElements = bufInt64[EMScbData(EMS_ARR_NELEM)];
    for (int64_t idx = 0; idx < nElements; idx++)
    {
        // EMS_TAG_FULL == 0
        bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_EMPTY;

        // D4: not needed w/ EMS_TYPE_UNDEFINED is TYPE_0
        //if (!useExisting)
        //   bufTags[EMSmapTag(idx)].tags.type = EMS_TYPE_UNDEFINED;
    }
}

static inline
char * _ems_mmap(const char *filename, size_t filesize, int flags) {
     // TODO:
    // if (_EMS_persist)
    //     fd = open(filename, O_APPEND | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // else
    //     fd = shm_open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    int fd = open(filename,
        O_APPEND | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Error opening ems memory file -- Possibly need to be root?");
        return -1;
    }
    if (flags & 1) {
        if (ftruncate(fd, (off_t) filesize) != 0) {
            if (errno != EINVAL) {
                fprintf(stderr, "EMSinitialize: Error during initialization, unable to set memory size to %" PRIu64 " bytes\n",
                        (uint64_t) filesize);
                return -1;
            }
        }
    }
    char *emsBuf = (char *) mmap(0, filesize,
        PROT_READ | PROT_WRITE, MAP_SHARED,
        fd, (off_t) 0);
    if (emsBuf == MAP_FAILED) {
        fprintf(stderr, "EMSinitialize: Unable to map domain memory\n");
        return -1;
    }
    close(fd);
    return emsBuf;
}

int ems_index_of_file(
    const char *filename)
{
    int idx = 0;
    while(idx < EMS_MAX_N_BUFS  &&  emsBufs[idx] != NULL) {
        if (0==strncmp(emsBufFilenames[idx], filename, MAX_FNAME_LEN))
            return idx;
        idx++;
    }
    return -1;
}

static inline
int _ems_register(const char *filename, size_t filesize, char *buf) {
    int ems_id = 0;
    while(ems_id < EMS_MAX_N_BUFS  &&  emsBufs[ems_id] != NULL)
        ems_id++;
    if(ems_id >= EMS_MAX_N_BUFS) {
        fprintf(stderr, "EMSinitialize: ERROR - Unable to allocate a buffer ID/index\n");
        return -1;
    }
    // register
    emsBufs[ems_id] = buf;
    emsBufLengths[ems_id] = filesize;
    strncpy(emsBufFilenames[ems_id], filename, MAX_FNAME_LEN);
    return ems_id;
}


// ems_destroy
// static inline
// int _ems_unregister(const int ems_id) {
//     emsBufs[ems_id] = NULL;
//     emsBufLengths[ems_id] = 0;

// }

 #define _EMS_useMap ((bool)(flags & EMS_USE_MAP))
#define _EMS_persist (flags & EMS_PERSIST)
#define _EMS_doDataFill (flags & EMS_FILL)
#define _EMS_fillIsJSON (flags & EMS_FILL_JSON)
#define _EMS_doSetFEtags (flags & EMS_SET_E)
#define _EMS_setFEtagsFull (flags & EMS_SET_F)
#define _EMS_pinThreads (flags & EMS_PIN_THREADS)
#define _EMS_pctMLock (flags & EMS_MLOCK_PCT_MASK)


int ems_open(
        const char *filename)
{
    int idx = ems_index_of_file(filename);
    // printf("idx=%i\n",idx);
    // TODO:
    if (idx > -1)
        return idx;
    size_t filesize = ems_await_owner(filename);
    char *buf = _ems_mmap(filename, filesize, 0);
    idx = _ems_register(filename, filesize, buf);
    return idx;
}

int ems_create(
    int64_t nElements,
    size_t heapSize,
    const char *filename,
    int32_t flags
) {

    bool file_exists = sx_os_path_exists(filename);
    bool should_clear = flags & EMS_CLEAR;

    printf("\n!!!! filename=%s file_exists=%i should_clear=%i\n",filename, file_exists, should_clear);

    if (file_exists) {

        // patch useful for chromium process reuse...
        int idx = ems_index_of_file(filename);
        printf("\n!!!!??????? filename=%s idx=%i\n",filename, idx);
        if (idx > -1) {
            if (should_clear) {
                file_exists = 0;
                ems_destroy(idx, should_clear);
            }
            else
                return idx;
        }
        else {
            // if (should_clear) {
                //  Node 0 is first and always has mutual exclusion during initialization perform once-only here
                // TODO: barrier incase opened...
                // if (!useExisting) {
                    file_exists = 0;
                    unlink(filename);
                    // shm_unlink(filename);
                // }
            // }
            // else
                // ems_reset_map_tags(idx);
        }

        // if (!should_clear && idx > -1) {
        //     printf("\n!!!!??????? filename=%s idx=%i\n",filename, file_exists, should_clear);
        //     return idx;
        // }
        // if (should_clear) {

        //     ems_destroy(idx, should_clear);



        // }
    }

    bool useExisting = file_exists; //!(should_clear || !file_exists);

    // //  Node 0 is first and always has mutual exclusion during initialization perform once-only here
    // // TODO: barrier incase opened...
    // if (!useExisting) {
    //     unlink(filename);
    //     // shm_unlink(filename);
    // }

    size_t nMemBlocks = (heapSize >> EMS_MEM_BLOCKSZ_P2) + 1;
    size_t nMemBlocksPow2 = emsNextPow2((int64_t) nMemBlocks);
    int32_t nMemLevels = __builtin_ctzl(nMemBlocksPow2);
    size_t bottomOfMap = (size_t)EMSdataTagWord(nElements)
        + (size_t)EMSwordSize;
        // Map begins 1 word AFTER the last tag word of data

    size_t bottomOfMalloc = bottomOfMap + (bottomOfMap*(_EMS_useMap|0));

    size_t bottomOfHeap = bottomOfMalloc
        + sizeof(struct emsMem)
        + ((nMemBlocksPow2 << 1) - 2);

    size_t filesize = bottomOfHeap + (nMemBlocksPow2 << EMS_MEM_BLOCKSZ_P2);

    char *emsBuf = _ems_mmap(filename, filesize, 1);

    // lock RAM if master control block
    int pctMLock = (nElements <= 0) ? 100 : _EMS_pctMLock;
    if (mlock((void *) emsBuf, (size_t) (filesize * (pctMLock / 100))) != 0) {
        fprintf(stderr, "EMSinitialize NOTICE: EMS was not able to lock EMS memory to RAM for %s\n", filename);
    }

    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    volatile double *bufDouble = (double *) emsBuf;
    char *bufChar = emsBuf;
    volatile int *bufInt32 = (int32_t *) emsBuf;
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;

    if (!useExisting) {
        EMStag_t tag;
        tag.tags.rw = 0;
        tag.tags.type = EMS_TYPE_INTEGER;
        tag.tags.fe = EMS_TAG_FULL;
        bufInt64[EMScbData(EMS_ARR_NELEM)] = nElements;
        bufInt64[EMScbData(EMS_ARR_HEAPSZ)] = heapSize;     // Unused?

        // TODO:  EMS_ARR_Q_BOTTOM is currently unused, repurpose...
        bufInt64[EMScbData(EMS_ARR_Q_BOTTOM)] = 0;
            bufTags[EMScbTag (EMS_ARR_Q_BOTTOM)].byte = tag.byte;

        bufInt64[EMScbData(EMS_ARR_STACKTOP)] = 0;
            bufTags[EMScbTag (EMS_ARR_STACKTOP)].byte = tag.byte;
        bufInt64[EMScbData(EMS_ARR_MAPBOT)] = bottomOfMap >> EMSwordSize_pow2; // / EMSwordSize;
        bufInt64[EMScbData(EMS_ARR_MALLOCBOT)] = bottomOfMalloc;
        bufInt64[EMScbData(EMS_ARR_HEAPBOT)] = bottomOfHeap;
        bufInt64[EMScbData(EMS_ARR_MEM_MUTEX)] = EMS_TAG_EMPTY;
        bufInt64[EMScbData(EMS_ARR_FILESZ)] = filesize;

        struct emsMem *emsMemBuffer = (struct emsMem *)
            &bufChar[bufInt64[EMScbData(EMS_ARR_MALLOCBOT)]];
        emsMemBuffer->level = nMemLevels;
    }

    int ems_id = _ems_register(filename, filesize, emsBuf);
    if (ems_id < 0)
        return -1;

    // if (EMSmyID == 0 && _EMS_useMap && useExisting)
    //     ems_reset_map_tags(ems_id);

    // printf("filename=%s EMSmyID=%i nElements=%i useExisting=%i\n",emsBufFilenames[ems_id],EMSmyID,nElements,useExisting);
    return ems_id;
}

/* TODO: thread manager...
int ems_man_create(
    int64_t nElements,     // 0
    size_t heapSize,      // 1
    const char *filename,  // 2
    // int thread_id,        // 3
    // int32_t nThreads,      // 4
    int32_t flags,          // 5
    EMSvalueType *fillValue// 6
) {
    // int EMSmyID = thread_id;

    if (nElements <= 0)
        filesize = (EMS_CB_LOCKS + nThreads) // EMS Control Block
            * sizeof(int);

    // lock RAM if master control block
    int pctMLock = (nElements <= 0) ? 100 : _EMS_pctMLock;
    if (mlock((void *) emsBuf, (size_t) (filesize * (pctMLock / 100))) != 0) {
        fprintf(stderr, "EMSinitialize NOTICE: EMS was not able to lock EMS memory to RAM for %s\n", filename);
    }
    if (EMSmyID == 0) {
        if (nElements <= 0) {   // This is the EMS CB
            bufInt32[EMS_CB_NTHREADS] = nThreads;
            bufInt32[EMS_CB_NBAR0] = nThreads;
            bufInt32[EMS_CB_NBAR1] = nThreads;
            bufInt32[EMS_CB_BARPHASE] = 0;
            bufInt32[EMS_CB_CRITICAL] = 0;
            bufInt32[EMS_CB_SINGLE] = 0;
            for (int i = EMS_CB_LOCKS; i < EMS_CB_LOCKS + nThreads; i++) {
                bufInt32[i] = EMS_TAG_BUSY;  //  Reset all locks
            }
        } else {   //  This is a user data domain

        }
    }

    #if defined(__linux)
    if (_EMS_pinThreads) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET((EMSmyID % nThreads), &cpuset);  // Round-robin over-subscribed systems
        sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    }
    #endif
}
*/

#undef _EMS_useMap
#undef _EMS_persist
#undef _EMS_doDataFill
#undef _EMS_fillIsJSON
#undef _EMS_doSetFEtags
#undef _EMS_setFEtagsFull
#undef _EMS_pinThreads
#undef _EMS_pctMLock

// !
#define ARR_DATATAG_EMPTY EMS_TAG_FULL

int ems_stack_get_top(
    int mmapID,
    int64_t* timer)
{
    void *emsBuf = emsBufs[mmapID];
    int64_t *bufInt64 = (int64_t *) emsBuf;
    EMStag_t *bufTags = (EMStag_t *) emsBuf;
    // char *bufChar = (char *) emsBuf;

    // Wait until the stack top is FULL,
    // mark BUSY while reading
    EMStransitionFEtag(
        &bufTags[EMScbTag(EMS_ARR_STACKTOP)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer))
        return -1;

    int32_t top = bufInt64[EMScbData(EMS_ARR_STACKTOP)];

    // done, mark FULL while reading
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
    return top;
}


int ems_queue_get_length(
    int mmapID,
    int64_t* timer)
{
    void *emsBuf = emsBufs[mmapID];
    int64_t *bufInt64 = (int64_t *) emsBuf;
    EMStag_t *bufTags = (EMStag_t *) emsBuf;
    // char *bufChar = (char *) emsBuf;

    // Wait until the stack top is FULL,
    // mark BUSY while reading
    EMStransitionFEtag(
        &bufTags[EMScbTag(EMS_ARR_STACKTOP)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer))
        return -1;

    int64_t bottop = bufInt64[EMScbData(EMS_ARR_STACKTOP)];
    int32_t top = bottop;
    int32_t bot = bottop >> 32;

    // done, mark FULL while reading
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
    return top - bot;
}

//==================================================================
//  Push onto stack
// TODO: Eventually promote return value to 64bit
// retruns
//      +0  :  len (top || pushed idx + 1)
//      -1  :  timeout
//      -2  :  err unknown type
//      -3  :  err entry overflow
int EMSpush(
    int mmapID,
    EMSvalueType *value,
    int64_t* timer)
{
    void *emsBuf = emsBufs[mmapID];
    int64_t *bufInt64 = (int64_t *) emsBuf;
    EMStag_t *bufTags = (EMStag_t *) emsBuf;
    char *bufChar = (char *) emsBuf;

    // Wait until the stack top is FULL,
    // then mark it BUSY while updating the stack
    EMStransitionFEtag(
        &bufTags[EMScbTag(EMS_ARR_STACKTOP)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer))
        return -1;


    // TODO: BUG: Truncating the full 64b range
    int64_t bottop = bufInt64[EMScbData(EMS_ARR_STACKTOP)];
    int32_t idx = bottop;
    int32_t top = idx + 1;

    if (top == bufInt64[EMScbData(EMS_ARR_NELEM)]) {
        fprintf(stderr, "EMSpush: Ran out of stack entries\n");
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        return -2;
    }

    //  Wait until the target memory at the top of the stack is EMPTY
    EMStag_t newTag;
    newTag.byte = EMStransitionFEtag(
        &bufTags[EMSdataTag(idx)], NULL,
        ARR_DATATAG_EMPTY, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer)) {
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        return -1;
    }
    // assert(bufTags[EMSdataTag(idx)].tags.fe == EMS_TAG_BUSY);

    newTag.tags.rw = 0;
    newTag.tags.type = value->type;
    newTag.tags.fe = ARR_DATATAG_EMPTY;
    //  Write the value onto the stack
    switch (newTag.tags.type)
    {
        case EMS_TYPE_UNDEFINED:
            bufInt64[EMSdataData(idx)] = 0xdeadbeef;
            break;
        case EMS_TYPE_INTEGER:
        case EMS_TYPE_BOOLEAN:
        case EMS_TYPE_FLOAT:
            bufInt64[EMSdataData(idx)] = (int64_t) value->value;
            break;
        case EMS_TYPE_STRING:
        case EMS_TYPE_BUFFER:
        case EMS_TYPE_JSON: {
                int64_t offset = _ems_malloc(value->length + 8);
                if (offset < 0) {
                    EMS_MALLOC_ERR(value->length + 8, "EMSpush: out of memory to store buffer\n")
                    top = -2; // TODO:
                    newTag.tags.type = 0;
                    goto NOCOMMIT;
                }
                bufInt64[EMSdataData(idx)] = offset;
                uint32_t * ptr = EMSheapPtr(offset);
                ptr[0] = value->length;
                memcpy((ptr+1), value->value, value->length);
                // EMS_ALLOC(byteOffset, strlen((const char *) value->value), bufChar, "EMSpush: out of memory to store buffer\n", -1); // + 1 NULL padding
                // memcpy(EMSheapPtr(byteOffset), (const char *) value->value, value->length);
                break;
            }
        // case EMS_TYPE_JSON:
        // case EMS_TYPE_STRING:
        //     {
        //         int64_t textOffset;
        //         size_t len = strlen((const char *) value->value);
        //         EMS_ALLOC(textOffset, len + 1, bufChar, "EMSpush: out of memory to store string\n", -1);
        //         bufInt64[EMSdataData(idx)] = textOffset;
        //         memcpy(EMSheapPtr(textOffset), (const char *) value->value, len + 1);
        //         break;
        //     }
        default:
            fprintf(stderr, "EMSpush: Unknown value type\n");
            top = -3;
            newTag.tags.type = 0;
            goto NOCOMMIT;
    }

    //  commit new top
    bufInt64[EMScbData(EMS_ARR_STACKTOP)] = bottop + 1;
    //  commit & mark data FULL
NOCOMMIT:
    bufTags[EMSdataTag(idx)].byte = newTag.byte;
    //  mark stack pointer basck to FULL
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;

    return top;
}


//==================================================================
//  Pop data from stack
//
int EMSpop(
    int mmapID,
    EMSvalueType *returnValue,
    int64_t* timer)
{
    void *emsBuf = emsBufs[mmapID];
    int64_t *bufInt64 = (int64_t *) emsBuf;
    EMStag_t *bufTags = (EMStag_t *) emsBuf;
    char *bufChar = (char *) emsBuf;
    EMStag_t dataTag;

    returnValue->length = 0;
    returnValue->type = EMS_TYPE_UNDEFINED;

    //  Wait until the stack pointer is full and mark it empty while pop is performed
    EMStransitionFEtag(
        &bufTags[EMScbTag(EMS_ARR_STACKTOP)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer)) {
        // returnValue->value = (void *) 0xf00dd00f;
        return -1;
    }

    // bufInt64[EMScbData(EMS_ARR_STACKTOP)]--;
    // int64_t idx = bufInt64[EMScbData(EMS_ARR_STACKTOP)];
    int64_t bottop = bufInt64[EMScbData(EMS_ARR_STACKTOP)];
    int32_t top = bottop;
    int32_t idx = top - 1;

    //  Stack is empty, return undefined
    if (idx < 0) {
        // bufInt64[EMScbData(EMS_ARR_STACKTOP)] = 0;
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        // returnValue->value = (void *) 0xf00dd00f;
        return 0;
    }

    //  Wait until the data pointed to by the stack pointer is FULL,
    //  then mark it BUSY while it is copied,
    //  and set it to EMPTY when finished
    dataTag.byte = EMStransitionFEtag(
        &bufTags[EMSdataTag(idx)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer)) {
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        // returnValue->value = (void *) 0xf00dd00f;
        return -1;
    }

    returnValue->type = dataTag.tags.type;
    switch (dataTag.tags.type)
    {
        case EMS_TYPE_UNDEFINED: {
            // returnValue->value = (void *) 0xdeadbeef;
            break;
        }
        case EMS_TYPE_BOOLEAN:
        case EMS_TYPE_INTEGER:
        case EMS_TYPE_FLOAT: {
            returnValue->value = (void *) bufInt64[EMSdataData(idx)];
            break;
        }
        case EMS_TYPE_STRING:
        case EMS_TYPE_BUFFER:
        case EMS_TYPE_JSON:  {
            // LENGTH (32bit) PREFIX ENCODE
            int64_t offset = bufInt64[EMSdataData(idx)];
            const uint32_t * ptr = EMSheapPtr(offset);
            int32_t len = ptr[0];
            returnValue->length = len;
            // TODO:  ... requires large preallocated returnValue
            // returnValue->value = malloc(len + 1);  // FREE
            // if(returnValue->value == NULL) {
            //     fprintf(stderr, "EMSpop: Unable to allocate space to return stack top string\n");
            //     idx = -4;
            //     goto NAY;
            // }
            memcpy(returnValue->value, (ptr+1), len);
            EMS_FREE(offset);
            break;
        }
        // case EMS_TYPE_JSON:
        // case EMS_TYPE_STRING: {
        //     int64_t offset = bufInt64[EMSdataData(idx)];
        //     const char * ptr = EMSheapPtr(offset);
        //     size_t len = strlen(ptr); // TODO:
        //     returnValue->length = len;
        //     // TODO: ... requires large preallocated buffer on ->value
        //     //
        //     // returnValue->value = malloc(len + 1); // FREE
        //     // if(returnValue->value == NULL) {
        //     //     fprintf(stderr, "EMSpop: Unable to allocate space to return stack top string\n");
        //     //     idx = -4;
        //     //     goto NAY;
        //     // }
        //     puts("------\n");
        //     memcpy(returnValue->value, ptr, len);
        //     EMS_FREE(offset);
        //     break;
        // }
        default:
            fprintf(stderr, "EMSpop: ERROR - unknown top of stack data type\n");
            idx = -3;
            goto NAY;
    }
    bufTags[EMSdataTag(idx)].tags.fe = ARR_DATATAG_EMPTY;
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
    bufInt64[EMScbData(EMS_ARR_STACKTOP)] = bottop - 1;
    return idx;

NAY:
    bufTags[EMSdataTag(idx)].tags.fe = ARR_DATATAG_EMPTY;
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
    returnValue->type = EMS_TYPE_UNDEFINED;
    // returnValue->value = (void *) 0xf00dd00f;
    return idx;

}


//==================================================================
//  Enqueue data
//  Heap top and bottom are monotonically increasing, but the index
//  returned is a circular buffer.
// TODO: Eventually promote return value to 64bit
int EMSenqueue(
    int mmapID,
    EMSvalueType *value,
    int64_t* timer)
{

    void *emsBuf = emsBufs[mmapID];
    int64_t *bufInt64 = (int64_t *) emsBuf;
    EMStag_t *bufTags = (EMStag_t *) emsBuf;
    char *bufChar = (char *) emsBuf;

    //  Wait until heap top is FULL
    //  mark BUSY while data is enqueued
    EMStransitionFEtag(
        &bufTags[EMScbTag(EMS_ARR_STACKTOP)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer)) {
        return -1;
    }

    // TODO: EMStransitionFEtag EMS_ARR_Q_BOTTOM
    // ...


    // TODO: BUG  This could be truncated
    int32_t cap = bufInt64[EMScbData(EMS_ARR_NELEM)];
    int64_t bottop = bufInt64[EMScbData(EMS_ARR_STACKTOP)];
    int32_t top = bottop;
    int32_t bot = bottop >> 32;
    // int32_t bot = bufInt64[EMScbData(EMS_ARR_Q_BOTTOM)];
    int32_t idx = top % cap;
    ++top;
    int32_t len = top - bot;
    if ( len > cap ) {
        fprintf(stderr, "EMSenqueue: Ran out of stack entries\n");
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        return -2;
    }

    //  Wait for data pointed to by heap top to be empty,
    //  then set to Full while it is filled
    EMStag_t newTag;
    newTag.byte = EMStransitionFEtag(
        &bufTags[EMSdataTag(idx)], NULL,
        ARR_DATATAG_EMPTY, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer)) {
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        return -1;
    }

    newTag.tags.rw = 0;
    newTag.tags.type = value->type;
    newTag.tags.fe = ARR_DATATAG_EMPTY;
    // bufTags[EMSdataTag(idx)].tags.rw = 0;
    // bufTags[EMSdataTag(idx)].tags.type = value->type;
    switch (newTag.tags.type) {
        case EMS_TYPE_UNDEFINED:
            bufInt64[EMSdataData(idx)] = 0xdeadbeef;
            break;
        case EMS_TYPE_BOOLEAN:
        case EMS_TYPE_INTEGER:
        case EMS_TYPE_FLOAT:
            bufInt64[EMSdataData(idx)] = (int64_t) value->value;
            break;
        case EMS_TYPE_STRING:
        case EMS_TYPE_BUFFER:
        case EMS_TYPE_JSON: {
            int64_t offset = _ems_malloc(value->length + 8);
            if (offset < 0) {
                EMS_MALLOC_ERR(value->length + 8, "EMSenqueue: out of memory to store buffer\n")
                len = -2;
                newTag.tags.type = 0;
                goto NOCOMMIT;
            }
            bufInt64[EMSdataData(idx)] = offset;
            uint32_t * ptr = EMSheapPtr(offset);
            ptr[0] = value->length;
            memcpy((ptr+1), (const void *) value->value, value->length);
            // EMS_ALLOC(byteOffset, strlen((const char *) value->value), bufChar, "EMSpush: out of memory to store buffer\n", -1); // + 1 NULL padding
            // memcpy(EMSheapPtr(byteOffset), (const char *) value->value, value->length);
            break;
        }
        // case EMS_TYPE_JSON:
        // case EMS_TYPE_STRING: {
        //     int64_t textOffset;
        //     size_t len = value->length;
        //     EMS_ALLOC(textOffset, len + 1, bufChar, "EMSpush: out of memory to store string\n", -1);
        //     bufInt64[EMSdataData(idx)] = textOffset;
        //     memcpy(EMSheapPtr(textOffset), (const char *) value->value, len + 1);
        //     break;
        // }
        default:
            fprintf(stderr, "EMSenqueue: Unknown value type\n");
            len = -3;
            newTag.tags.type = 0;
            goto NOCOMMIT;
    }

    // commit new top
    bufInt64[EMScbData(EMS_ARR_STACKTOP)] = ++bottop;
    //  mark stack data FULL
NOCOMMIT:
    bufTags[EMSdataTag(idx)].byte = newTag.byte;
    //  Enqueue is complete, set the tag on the heap to to FULL
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
    return len;
}


//==================================================================
//  Dequeue
int EMSdequeue(
    int mmapID,
    EMSvalueType *returnValue,
    int64_t* timer)
{
    void *emsBuf = emsBufs[mmapID];
    int64_t *bufInt64 = (int64_t *) emsBuf;
    EMStag_t *bufTags = (EMStag_t *) emsBuf;
    char *bufChar = (char *) emsBuf;
    EMStag_t dataTag;

    returnValue->length = 0;
    returnValue->type = EMS_TYPE_UNDEFINED;

    //  wait for bottom of heap pointer FULL
    //  mark it BUSY while data is dequeued
    EMStransitionFEtag(
        &bufTags[EMScbTag(EMS_ARR_STACKTOP)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer)) {
        // returnValue->value = (void *) 0xf00dd00f;
        return -1;
    }

    int64_t bottop = bufInt64[EMScbData(EMS_ARR_STACKTOP)];
    int32_t top = bottop;
    int32_t bot = bottop >> 32;
    int32_t idx = bot % bufInt64[EMScbData(EMS_ARR_NELEM)];


    //  If Queue is empty, return undefined
    if (bot >= top) {
        // bufInt64[EMScbData(EMS_ARR_STACKTOP)] = bot;
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        // returnValue->value = (void *) 0xf00dd00f;
        return 0;
    }

    ++bot;
    int32_t len = top - bot;

    //  Wait for data pointed to heap bot FULL
    //  mark BUSY while copying it
    //  mark EMPTY when done
    dataTag.byte = EMStransitionFEtag(
        &bufTags[EMSdataTag(idx)], NULL,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        timer);
    if (NANO_DID_TIMEOUT(timer)) {
        bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
        // returnValue->value = (void *) 0xf00dd00f;
        return -1;
    }

    dataTag.tags.fe = EMS_TAG_FULL;
    returnValue->type = dataTag.tags.type;
    switch (dataTag.tags.type)
    {
        case EMS_TYPE_UNDEFINED: {
            // returnValue->value = (void *) 0xdeadbeef;
            break;
        }
        case EMS_TYPE_BOOLEAN:
        case EMS_TYPE_INTEGER:
        case EMS_TYPE_FLOAT: {
            returnValue->value = (void *) bufInt64[EMSdataData(idx)];
            break;
        }
        case EMS_TYPE_STRING:
        case EMS_TYPE_BUFFER:
        case EMS_TYPE_JSON: {
            // LENGTH (32bit) PREFIX ENCODE
            int64_t offset = bufInt64[EMSdataData(idx)];
            const uint32_t * ptr = EMSheapPtr(offset);
            int32_t len = ptr[0];
            returnValue->length = len;
            // TODO:  ... requires large preallocated returnValue
            // returnValue->value = malloc(len + 1);  // FREE
            // if(returnValue->value == NULL) {
            //     fprintf(stderr, "EMSdequeue: Unable to allocate space to return stack top string\n");
            //     idx = -4;
            //     goto NAY;
            // }
            memcpy(returnValue->value, (ptr+1), len);
            EMS_FREE(offset);
            break;
        }
        // case EMS_TYPE_JSON:
        // case EMS_TYPE_STRING: {
        //     int64_t offset = bufInt64[EMSdataData(idx)];
        //     const char * ptr = EMSheapPtr(offset);
        //     size_t len = strlen(ptr); // TODO:
        //     returnValue->length = len;
        //     returnValue->value = malloc(len + 1); // FREE
        //     if(returnValue->value == NULL) {
        //         fprintf(stderr, "EMSpop: Unable to allocate space to return stack top string\n");
        //         idx = -4;
        //         goto NAY;
        //     }
        //     memcpy(returnValue->value, ptr, len + 1);
        //     EMS_FREE(offset);
        //     break;
        // }
        default:
            fprintf(stderr, "EMSdequeue: ERROR - unknown type at head of queue\n");
            len = -3;
            goto NAY;
    }

    bufTags[EMSdataTag(idx)].byte = dataTag.byte;
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
    bufInt64[EMScbData(EMS_ARR_STACKTOP)] = ((int64_t)top)|((int64_t)bot<<32);
    return len;

NAY:
    bufTags[EMSdataTag(idx)].byte = dataTag.byte;
    bufTags[EMScbTag(EMS_ARR_STACKTOP)].tags.fe = EMS_TAG_FULL;
    returnValue->type = EMS_TYPE_UNDEFINED;
    // returnValue->value = (void *) 0xf00dd00f;
    return len;
}

/*-----------------------------------------------------------------------------+
 |  Extended Memory Semantics (EMS)                            Version 1.3.0   |
 |  Synthetic Semantics       http://www.synsem.com/       mogill@synsem.com   |
 +-----------------------------------------------------------------------------+
 |  Copyright (c) 2011-2014, Synthetic Semantics LLC.  All rights reserved.    |
 |  Copyright (c) 2015-2016, Jace A Mogill.  All rights reserved.              |
 |                                                                             |
 | Redistribution and use in source and binary forms, with or without          |
 | modification, are permitted provided that the following conditions are met: |
 |    * Redistributions of source code must retain the above copyright         |
 |      notice, this list of conditions and the following disclaimer.          |
 |    * Redistributions in binary form must reproduce the above copyright      |
 |      notice, this list of conditions and the following disclaimer in the    |
 |      documentation and/or other materials provided with the distribution.   |
 |    * Neither the name of the Synthetic Semantics nor the names of its       |
 |      contributors may be used to endorse or promote products derived        |
 |      from this software without specific prior written permission.          |
 |                                                                             |
 |    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS      |
 |    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT        |
 |    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    |
 |    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SYNTHETIC         |
 |    SEMANTICS LLC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,   |
 |    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,      |
 |    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR       |
 |    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF   |
 |    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     |
 |    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS       |
 |    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.             |
 |                                                                             |
 +-----------------------------------------------------------------------------*/
#include "ems.h"

//==================================================================
//  Fetch and Add Atomic Memory Operation
//  Returns a+b where a is data in EMS memory and b is an argument
bool EMSfaa(
    int mmapID,
    EMSvalueType *key,
    EMSvalueType *value,
    EMSvalueType *returnValue)
{
    // TODO:
    int64_t * timer = NULL;

    void *emsBuf = emsBufs[mmapID];
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    int64_t idx = EMSwriteIndexMap(mmapID, key,
        timer);
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    volatile double *bufDouble = (double *) emsBuf;
    char *bufChar = (char *) emsBuf;
    EMStag_t oldTag;

    if (idx < 0 || idx >= bufInt64[EMScbData(EMS_ARR_NELEM)]) {
        fprintf(stderr, "EMSfaa: index out of bounds\n");
        return false;
    }

    volatile EMStag_t *maptag;
    if (EMSisMapped) { maptag = &bufTags[EMSmapTag(idx)]; }
    else             { maptag = NULL; }
    // Wait until the data is FULL, mark it busy while FAA is performed
    oldTag.byte = EMStransitionFEtag(
        &bufTags[EMSdataTag(idx)], maptag,
        EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
        NULL); // TODO:

    oldTag.tags.fe = EMS_TAG_FULL;  // When written back, mark FULL
    switch (oldTag.tags.type) {
        case EMS_TYPE_BOOLEAN: {    //  Bool + _______
            bool retBool = bufInt64[EMSdataData(idx)];  // Read original value in memory
            returnValue->value = (void *) retBool;
            returnValue->type  = EMS_TYPE_BOOLEAN;
            switch (value->type) {
                case EMS_TYPE_INTEGER:   //  Bool + Int
                    bufInt64[EMSdataData(idx)] += (int64_t) value->value;
                    oldTag.tags.type = EMS_TYPE_INTEGER;
                    break;
                case EMS_TYPE_FLOAT: {    //  Bool + Float
                    EMSulong_double alias;
                    alias.u64 = (uint64_t) value->value;
                    bufDouble[EMSdataData(idx)] =
                            (double) bufInt64[EMSdataData(idx)] + alias.d;
                    oldTag.tags.type = EMS_TYPE_FLOAT;
                }
                    break;
                case EMS_TYPE_UNDEFINED: //  Bool + undefined
                    bufDouble[EMSdataData(idx)] = NAN;
                    oldTag.tags.type = EMS_TYPE_FLOAT;
                    break;
                case EMS_TYPE_BOOLEAN:   //  Bool + Bool
                    bufInt64[EMSdataData(idx)] += (int64_t) value->value;
                    oldTag.tags.type = EMS_TYPE_INTEGER;
                    break;
                case EMS_TYPE_STRING: {   //  Bool + string
                    int64_t textOffset;
                    EMS_ALLOC(textOffset, value->length + 1 + 5, //  String length + Terminating null + 'false'
                              bufChar, "EMSfaa(bool+string): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%s%s",
                            bufInt64[EMSdataData(idx)] ? "true" : "false", (const char *) value->value);
                    bufInt64[EMSdataData(idx)] = textOffset;
                    oldTag.tags.type = EMS_TYPE_STRING;
                }
                    break;
                default:
                    fprintf(stderr, "EMSfaa: Data is BOOL, but FAA arg type is unknown\n");
                    return false;

            }
            //  Write the new type and set the tag to Full, then return the original value
            bufTags[EMSdataTag(idx)].byte = oldTag.byte;
            if (EMSisMapped) bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
            return true;
        }  // End of:  Bool + ___

        case EMS_TYPE_INTEGER: {
            int64_t retInt = bufInt64[EMSdataData(idx)];  // Read original value in memory
            returnValue->type = EMS_TYPE_INTEGER;
            returnValue->value = (void *) retInt;
            switch (value->type) {
                case EMS_TYPE_INTEGER: {  // Int + int
                    int64_t memInt = bufInt64[EMSdataData(idx)] + (int64_t) value->value;
                    // TODO: Magic max int promotion to float
                    if (memInt >= (1 << 30)) {  // Possible integer overflow, convert to float
                        bufDouble[EMSdataData(idx)] = (double) memInt;
                        oldTag.tags.type = EMS_TYPE_FLOAT;
                    } else { //  Did not overflow to flow, still an integer
                        bufInt64[EMSdataData(idx)] = memInt;
                    }
                }
                    break;
                case EMS_TYPE_FLOAT: {    // Int + float
                    EMSulong_double alias;
                    alias.u64 = (uint64_t) value->value;
                    bufDouble[EMSdataData(idx)] =
                            (double) bufInt64[EMSdataData(idx)] + alias.d;
                    oldTag.tags.type = EMS_TYPE_FLOAT;
                }
                    break;
                case EMS_TYPE_UNDEFINED: // Int + undefined
                    bufDouble[EMSdataData(idx)] = NAN;
                    oldTag.tags.type = EMS_TYPE_FLOAT;
                    break;
                case EMS_TYPE_BOOLEAN:   // Int + bool
                    bufInt64[EMSdataData(idx)] += (int64_t) value->value;
                    break;
                case EMS_TYPE_STRING: {   // int + string
                    int64_t textOffset;
                    EMS_ALLOC(textOffset, value->length + 1 + MAX_NUMBER2STR_LEN,
                              bufChar, "EMSfaa(int+string): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%lld%s",
                            (long long int) bufInt64[EMSdataData(idx)], (const char *) value->value);
                    bufInt64[EMSdataData(idx)] = textOffset;
                    oldTag.tags.type = EMS_TYPE_STRING;
                }
                    break;
                default:
                    fprintf(stderr, "EMSfaa: Data is INT, but FAA arg type is unknown\n");
                    return false;
            }
            //  Write the new type and set the tag to Full, then return the original value
            bufTags[EMSdataTag(idx)].byte = oldTag.byte;
            if (EMSisMapped) bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
            return true;
        }  // End of: Integer + ____

        case EMS_TYPE_FLOAT: {
            double retDbl = bufDouble[EMSdataData(idx)];
            returnValue->type = EMS_TYPE_FLOAT;
            EMSulong_double alias;
            alias.d = retDbl;
            returnValue->value = (void *) alias.u64;

            switch (value->type) {
                case EMS_TYPE_INTEGER:   // Float + int
                    bufDouble[EMSdataData(idx)] += (double) ((int64_t) value->value);
                    break;
                case EMS_TYPE_FLOAT: {   // Float + float
                    EMSulong_double alias;
                    alias.u64 = (uint64_t) value->value;
                    bufDouble[EMSdataData(idx)] += alias.d;
                }
                    break;
                case EMS_TYPE_BOOLEAN:   // Float + boolean
                    bufDouble[EMSdataData(idx)] += (double) ((int64_t) value->value);
                    break;
                case EMS_TYPE_STRING: {   // Float + string
                    int64_t textOffset;
                    EMS_ALLOC(textOffset, value->length + 1 + MAX_NUMBER2STR_LEN,
                              bufChar, "EMSfaa(float+string): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%lf%s", bufDouble[EMSdataData(idx)], (const char *) value->value);
                    bufInt64[EMSdataData(idx)] = textOffset;
                    oldTag.tags.type = EMS_TYPE_STRING;
                }
                    break;
                case EMS_TYPE_UNDEFINED: // Float + Undefined
                    bufDouble[EMSdataData(idx)] = NAN;
                    break;
                default:
                    fprintf(stderr, "EMSfaa: Data is FLOAT, but arg type unknown\n");
                    return false;
            }
            //  Write the new type and set the tag to Full, then return the original value
            bufTags[EMSdataTag(idx)].byte = oldTag.byte;
            if (EMSisMapped) bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
            return true;
        } //  End of: float + _______

        case EMS_TYPE_STRING: {            
            size_t oldStrLen = strlen(EMSheapPtr(bufInt64[EMSdataData(idx)]));
            returnValue->type = EMS_TYPE_STRING;
            returnValue->value = malloc(oldStrLen + 1);  // freed in NodeJSfaa
            if(returnValue->value == NULL) {
                fprintf(stderr, "EMSfaa: Unable to malloc temporary old string\n");
                return false;
            }
            strcpy((char *) returnValue->value, EMSheapPtr(bufInt64[EMSdataData(idx)]));
            int64_t textOffset;
            size_t len;
            switch (value->type) {
                case EMS_TYPE_INTEGER: // string + int
                    len = oldStrLen + 1 + MAX_NUMBER2STR_LEN;
                    EMS_ALLOC(textOffset, len, bufChar, "EMSfaa(string+int): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%s%lld",
                            EMSheapPtr(bufInt64[EMSdataData(idx)]),
                            (long long int) value->value);
                    break;
                case EMS_TYPE_FLOAT: {  // string + dbl
                    EMSulong_double alias;
                    alias.u64 = (uint64_t) value->value;
                    len = oldStrLen + 1 + MAX_NUMBER2STR_LEN;
                    EMS_ALLOC(textOffset, len, bufChar, "EMSfaa(string+dbl): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%s%lf",
                            EMSheapPtr(bufInt64[EMSdataData(idx)]), alias.d);
                }
                    break;
                case EMS_TYPE_STRING: { // string + string
                    len = oldStrLen + 1 + value->length;
                    EMS_ALLOC(textOffset, len, bufChar, "EMSfaa(string+string): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%s%s",
                            EMSheapPtr(bufInt64[EMSdataData(idx)]), (const char *) value->value);
                }
                    break;
                case EMS_TYPE_BOOLEAN:   // string + bool
                    len = strlen(EMSheapPtr(bufInt64[EMSdataData(idx)])) + 1 + 5;  // 5==strlen("false")
                    EMS_ALLOC(textOffset, len, bufChar, "EMSfaa(string+bool): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%s%s",
                            EMSheapPtr(bufInt64[EMSdataData(idx)]), (bool) value->value ? "true" : "false");
                    break;
                case EMS_TYPE_UNDEFINED: // string + undefined
                    len = strlen(EMSheapPtr(bufInt64[EMSdataData(idx)])) + 1 + 9; // 9 == strlen("undefined");
                    EMS_ALLOC(textOffset, len, bufChar, "EMSfaa(string+undefined): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "%s%s",
                            EMSheapPtr(bufInt64[EMSdataData(idx)]), "undefined");
                    break;
                default:
                    fprintf(stderr, "EMSfaa(string+?): Unknown data type\n");
                    return false;
            }
            EMS_FREE(bufInt64[EMSdataData(idx)]);
            bufInt64[EMSdataData(idx)] = textOffset;
            oldTag.tags.type = EMS_TYPE_STRING;
            //  Write the new type and set the tag to Full, then return the original value
            bufTags[EMSdataTag(idx)].byte = oldTag.byte;
            if (EMSisMapped) bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
            // return value was set at the top of this block
            return true;
        }  // End of: String + __________

         case EMS_TYPE_UNDEFINED: {
             returnValue->type = EMS_TYPE_UNDEFINED;
             returnValue->value = (void *) 0xf00dd00f;
             switch (value->type) {  // Undefined + Int, dloat, bool, or undef
                case EMS_TYPE_INTEGER:
                case EMS_TYPE_FLOAT:
                case EMS_TYPE_BOOLEAN:
                case EMS_TYPE_UNDEFINED:
                    bufDouble[EMSdataData(idx)] = NAN;
                    oldTag.tags.type = EMS_TYPE_FLOAT;
                    break;
                case EMS_TYPE_STRING: { // Undefined + string
                    int64_t textOffset;
                    EMS_ALLOC(textOffset, value->length + 1 + 3, //  3 = strlen("NaN");
                              bufChar, "EMSfaa(undef+String): out of memory to store string\n", false);
                    sprintf(EMSheapPtr(textOffset), "NaN%s", (const char *) value->value);
                    bufInt64[EMSdataData(idx)] = textOffset;
                    oldTag.tags.type = EMS_TYPE_UNDEFINED;
                }
                    break;
                default:
                    fprintf(stderr, "EMSfaa(Undefined+___: Unknown stored data type\n");
                    return false;
            }
            //  Write the new type and set the tag to Full, then return the original value
            bufTags[EMSdataTag(idx)].byte = oldTag.byte;
            if (EMSisMapped) bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;
            return true;
        }
        default:
            fprintf(stderr, "EMSfaa(?+___: Unknown stored data type\n");
            return false;
    }
    // fprintf(stderr, "EMSfaa: Unknown everything -- this statement is unreachable\n");
    return false;
}


//==================================================================
//  Atomic Compare and Swap
bool EMScas(
    int mmapID,
    EMSvalueType *key,
    EMSvalueType *oldValue, EMSvalueType *newValue,
    EMSvalueType *returnValue)
{
    // TODO:
    int64_t * timer = NULL;

    void *emsBuf = emsBufs[mmapID];
    volatile int64_t *bufInt64 = (int64_t *) emsBuf;
    int64_t idx = EMSkey2index(emsBuf, key, EMSisMapped);
    char * bufChar = (char *) emsBuf;
    volatile EMStag_t *bufTags = (EMStag_t *) emsBuf;
    EMStag_t newTag;
    int64_t textOffset;
    int swapped = false;

    if ((!EMSisMapped  &&  idx < 0) || idx >= bufInt64[EMScbData(EMS_ARR_NELEM)]) {
        fprintf(stderr, "EMScas: index out of bounds\n");
        return false;
    }

    size_t memStrLen;
    unsigned char memType;
retry_on_undefined:
    if(EMSisMapped  &&  idx < 0) {
        memType = EMS_TYPE_UNDEFINED;
    } else {
        //  Wait for the memory to be Full, then mark it Busy while CAS works
        volatile EMStag_t *maptag;
        if (EMSisMapped) { maptag = &bufTags[EMSmapTag(idx)]; }
        else             { maptag = NULL; }
        // Wait until the data is FULL, mark it busy while FAA is performed
        EMStransitionFEtag(
            &bufTags[EMSdataTag(idx)], maptag,
            EMS_TAG_FULL, EMS_TAG_BUSY, EMS_TAG_ANY,
            timer); // TODO:

        memType = bufTags[EMSdataTag(idx)].tags.type;
    }

    //  Read the value in memory
    returnValue->type = memType;
    switch (memType) {
        case EMS_TYPE_UNDEFINED:
            returnValue->value = (void *) 0xf00dcafe;
            break;
        case EMS_TYPE_BOOLEAN:
        case EMS_TYPE_INTEGER:
        case EMS_TYPE_FLOAT:
            returnValue->value =  (void *) bufInt64[EMSdataData(idx)];
            break;
        case EMS_TYPE_BUFFER:
            // char *data = EMSheapPtr(bufInt64[EMSdataData(idx)])
            // memStrLen = strlen(data);
            memStrLen = strlen(EMSheapPtr(bufInt64[EMSdataData(idx)]));
            returnValue->value = malloc(memStrLen);  // freed in NodeJSfaa // returnValue->value = malloc(memStrLen + 1); +1 NULL padding // freed in NodeJSfaa
            if(returnValue->value == NULL) {
                fprintf(stderr, "EMScas: Unable to allocate space to return old buffer\n");
                return false;
            }
            // memcpy(returnValue.value, data, memStrLen);
            memcpy((char *) returnValue->value, EMSheapPtr(bufInt64[EMSdataData(idx)]), memStrLen);
            break;
        case EMS_TYPE_JSON:
        case EMS_TYPE_STRING:
            memStrLen = strlen(EMSheapPtr(bufInt64[EMSdataData(idx)]));
            returnValue->value = malloc(memStrLen + 1);  // freed in NodeJSfaa
            if(returnValue->value == NULL) {
                fprintf(stderr, "EMScas: Unable to allocate space to return old string\n");
                return false;
            }
            strcpy((char *) returnValue->value, EMSheapPtr(bufInt64[EMSdataData(idx)]));
            break;
        default:
            fprintf(stderr, "EMScas: memType not recognized\n");
            return false;
    }

    //  Compare the value in memory the the "old" CAS value
    if (oldValue->type == memType) {
        //  Allocate on Write: If this memory was undefined (ie: unallocated),
        //  allocate the index map, store the undefined, and start over again.
        if(EMSisMapped  &&  idx < 0) {
            idx = EMSwriteIndexMap(mmapID, key,
                timer);
            if (idx < 0) {
                fprintf(stderr, "EMScas: Not able to allocate map on CAS of undefined data\n");
                return false;
            }
            bufInt64[EMSdataData(idx)] = 0xcafebabe;
            newTag.tags.fe = EMS_TAG_FULL;
            newTag.tags.rw = 0;
            newTag.tags.type = EMS_TYPE_UNDEFINED;
            bufTags[EMSdataTag(idx)].byte = newTag.byte;
            goto retry_on_undefined;
        }
        switch (memType) {
            case EMS_TYPE_UNDEFINED:
                swapped = true;
                break;
            case EMS_TYPE_BOOLEAN:
            case EMS_TYPE_INTEGER:
            case EMS_TYPE_FLOAT:
                if (returnValue->value == oldValue->value)
                    swapped = true;
                break;
            case EMS_TYPE_BUFFER:
                // TODO: if (returnValue->length !== oldValue->length || ..
                if (memcmp((const char *) returnValue->value, (const char *) oldValue->value, oldValue->length) == 0) {
                    swapped = true;
                }
                break;
            case EMS_TYPE_JSON:
            case EMS_TYPE_STRING:
                if (strcmp((const char *) returnValue->value, (const char *) oldValue->value) == 0) {
                    swapped = true;
                }
                break;
            default:
                fprintf(stderr, "EMScas: oldTag not recognized");
                return false;
        }
    }

    //  If memory==old then write the new value
    newTag.tags.fe = EMS_TAG_FULL;
    newTag.tags.rw = 0;
    newTag.tags.type = memType;
    if (swapped) {
        if (memType == EMS_TYPE_STRING  ||  memType == EMS_TYPE_JSON ||  memType == EMS_TYPE_BUFFER)
            EMS_FREE((size_t) bufInt64[EMSdataData(idx)]);
        newTag.tags.type = newValue->type;
        switch (newValue->type) {
            case EMS_TYPE_UNDEFINED:
                bufInt64[EMSdataData(idx)] = 0xbeeff00d;
                break;
            case EMS_TYPE_BOOLEAN:
            case EMS_TYPE_INTEGER:
            case EMS_TYPE_FLOAT:
                bufInt64[EMSdataData(idx)] = (int64_t) newValue->value;
                break;
            case EMS_TYPE_BUFFER:
                // EMS_ALLOC(textOffset, newValue->length + 1, // +1 NULL padding
                EMS_ALLOC(textOffset, newValue->length,
                          bufChar, "EMScas(string): out of memory to store buffer\n", false);
                memcpy(EMSheapPtr(textOffset), (const char *) newValue->value, newValue->length);
                bufInt64[EMSdataData(idx)] = textOffset;
                break;
            case EMS_TYPE_JSON:
            case EMS_TYPE_STRING:
                EMS_ALLOC(textOffset, newValue->length + 1,
                          bufChar, "EMScas(string): out of memory to store string\n", false);
                strcpy(EMSheapPtr(textOffset), (const char *) newValue->value);
                bufInt64[EMSdataData(idx)] = textOffset;
                break;
            default:
                fprintf(stderr, "EMScas(): Unrecognized new type\n");
                return false;
        }
    }

    //  Set the tag back to Full and return the original value
    bufTags[EMSdataTag(idx)].byte = newTag.byte;
    //  If there is a map, set the map's tag back to full
    if (EMSisMapped)
        bufTags[EMSmapTag(idx)].tags.fe = EMS_TAG_FULL;

    return true;
}

/*-----------------------------------------------------------------------------+
 |  Extended Memory Semantics (EMS)                            Version 1.3.0   |
 |  Synthetic Semantics       http://www.synsem.com/       mogill@synsem.com   |
 +-----------------------------------------------------------------------------+
 |  Copyright (c) 2011-2014, Synthetic Semantics LLC.  All rights reserved.    |
 |  Copyright (c) 2015-2016, Jace A Mogill.  All rights reserved.              |
 |                                                                             |
 | Redistribution and use in source and binary forms, with or without          |
 | modification, are permitted provided that the following conditions are met: |
 |    * Redistributions of source code must retain the above copyright         |
 |      notice, this list of conditions and the following disclaimer.          |
 |    * Redistributions in binary form must reproduce the above copyright      |
 |      notice, this list of conditions and the following disclaimer in the    |
 |      documentation and/or other materials provided with the distribution.   |
 |    * Neither the name of the Synthetic Semantics nor the names of its       |
 |      contributors may be used to endorse or promote products derived        |
 |      from this software without specific prior written permission.          |
 |                                                                             |
 |    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS      |
 |    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT        |
 |    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    |
 |    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SYNTHETIC         |
 |    SEMANTICS LLC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,   |
 |    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,      |
 |    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR       |
 |    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF   |
 |    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     |
 |    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS       |
 |    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.             |
 |                                                                             |
 +-----------------------------------------------------------------------------*/
#include "ems.h"


//==================================================================
//  Execute Once -- Single execution
//  OpenMP style, only first thread executes, remaining skip
bool EMSsingleTask(int mmapID) {
    void *emsBuf = emsBufs[mmapID];
    int32_t *bufInt32 = (int32_t *) emsBuf;

    // Increment the tally of threads that have reached this statement
    int retval = __sync_fetch_and_add(&bufInt32[EMS_CB_SINGLE], 1);

    //  If all threads have passed through the counter, reset fo next time
    if (retval == bufInt32[EMS_CB_NTHREADS] - 1) {
        bufInt32[EMS_CB_SINGLE] = 0;
    }

    //  Return True if this thread was first to the counter, later
    //  threads return false
    return retval == 0;
}


//==================================================================
//  Critical Region Entry --  1 thread at a time passes this barrier
//
int EMScriticalEnter(int mmapID, int timeout) {
    RESET_NAP_TIME()
    void *emsBuf = emsBufs[mmapID];
    int32_t *bufInt32 = (int32_t *) emsBuf;

    // Acquire the mutual exclusion lock
    while (!__sync_bool_compare_and_swap(&(bufInt32[EMS_CB_CRITICAL]), EMS_TAG_FULL, EMS_TAG_EMPTY)
        && timeout > 0 ) {
        NANOSLEEP;
        timeout -= 1;
    }

    return timeout;
}


//==================================================================
//  Critical Region Exit
bool EMScriticalExit(int mmapID) {
    void *emsBuf = emsBufs[mmapID];
    int32_t *bufInt32 = (int32_t *) emsBuf;

    // Test the mutual exclusion lock wasn't somehow lost
    if (bufInt32[EMS_CB_CRITICAL] != EMS_TAG_EMPTY) {
        return false;
    }

    bufInt32[EMS_CB_CRITICAL] = EMS_TAG_FULL;
    return true;
}


//==================================================================
//  Phase Based Global Thread Barrier
int EMSbarrier(int mmapID, int timeout) {
    void *emsBuf = emsBufs[mmapID];
    int32_t *bufInt32 = (int32_t *) emsBuf;

    int barPhase = bufInt32[EMS_CB_BARPHASE];    // Determine current phase of barrier
    int retval = __sync_fetch_and_add(&bufInt32[EMS_CB_NBAR0 + barPhase], -1);
    if (retval < 0) {
        fprintf(stderr, "EMSbarrier: Race condition at barrier\n");
        return false;
    }

    if (retval == 1) {
        //  This thread was the last to reach the barrier,
        //  Reset the barrier count for this phase and graduate to the next phase
        bufInt32[EMS_CB_NBAR0 + barPhase] = bufInt32[EMS_CB_NTHREADS];
        bufInt32[EMS_CB_BARPHASE] = !barPhase;
    } else {
        //  Wait for the barrier phase to change, indicating the last thread arrived
        RESET_NAP_TIME()
        while (timeout > 0  &&  barPhase == bufInt32[EMS_CB_BARPHASE]) {
            NANOSLEEP;
            timeout -= 1;
        }
    }

    return timeout;
}



//==================================================================
//  Parallel Loop -- context initialization
//
bool EMSloopInit(int mmapID, int32_t start, int32_t end, int32_t minChunk, int schedule_mode) {
    void *emsBuf = emsBufs[mmapID];
    int32_t *bufInt32 = (int32_t *) emsBuf;
    bool success = true;

    bufInt32[EMS_LOOP_IDX] = start;
    bufInt32[EMS_LOOP_START] = start;
    bufInt32[EMS_LOOP_END] = end;
    switch (schedule_mode) {
        case EMS_SCHED_GUIDED:
            bufInt32[EMS_LOOP_CHUNKSZ] = ((end - start) / 2) / bufInt32[EMS_CB_NTHREADS];
            if (bufInt32[EMS_LOOP_CHUNKSZ] < minChunk) bufInt32[EMS_LOOP_CHUNKSZ] = minChunk;
            bufInt32[EMS_LOOP_MINCHUNK] = minChunk;
            bufInt32[EMS_LOOP_SCHED] = EMS_SCHED_GUIDED;
            break;
        case EMS_SCHED_DYNAMIC:
            bufInt32[EMS_LOOP_CHUNKSZ] = 1;
            bufInt32[EMS_LOOP_MINCHUNK] = 1;
            bufInt32[EMS_LOOP_SCHED] = EMS_SCHED_DYNAMIC;
            break;
        default:
            fprintf(stderr, "NodeJSloopInit: Unknown schedule modes\n");
            success = false;
    }
    return success;
}


//==================================================================
//  Determine the current block of iterations to assign to an
//  an idle thread
//  JQM TODO BUG  -- convert to 64 bit using  fe tags
//
bool EMSloopChunk(int mmapID, int32_t *start, int32_t *end) {
    void *emsBuf = emsBufs[mmapID];
    int32_t *bufInt32 = (int32_t *) emsBuf;

    int chunkSize = bufInt32[EMS_LOOP_CHUNKSZ];
    *start = __sync_fetch_and_add(&(bufInt32[EMS_LOOP_IDX]), chunkSize);
    *end = *start + chunkSize;

    if (*start > bufInt32[EMS_LOOP_END]) *end = 0;
    if (*end > bufInt32[EMS_LOOP_END]) *end = bufInt32[EMS_LOOP_END];
    if (bufInt32[EMS_LOOP_SCHED] == EMS_SCHED_GUIDED) {
        //  Compute the size of the chunk the next thread should use
        int newSz = (int) ((bufInt32[EMS_LOOP_END] - *start) / 2) / bufInt32[EMS_CB_NTHREADS];
        if (newSz < bufInt32[EMS_LOOP_MINCHUNK]) newSz = bufInt32[EMS_LOOP_MINCHUNK];
        bufInt32[EMS_LOOP_CHUNKSZ] = newSz;
    }

    return true;
}

/*              xm
==============================================================================*/
#include        "ems.h"

int EMSpush(int mmapID, EMSvalueType *value, int64_t* timer);
int EMSpop(int mmapID, EMSvalueType *returnValue, int64_t* timer);
int EMSenqueue(int mmapID, EMSvalueType *value, int64_t* timer);
int EMSdequeue(int mmapID, EMSvalueType *returnValue, int64_t* timer);

// #define         XM_EXPORT()\
//                     SYM_EXPORT(xm_destroy)                                  \
//                     SYM_EXPORT(xm_read)         SYM_EXPORT(xm_write)        \
//                     SYM_EXPORT(xm_read_fe)      SYM_EXPORT(xm_write_xf)     \
//                     SYM_EXPORT(xm_read_ff)      SYM_EXPORT(xm_write_xe)     \
//                     SYM_EXPORT(xm_write_ef)                                 \
//                     SYM_EXPORT(xm_read_swmr)    SYM_EXPORT(xm_release_swmr) \
//                     SYM_EXPORT(xm_key2idx)      SYM_EXPORT(xm_idx2key)      \
//                     SYM_EXPORT(xm_await_file)

                /*
                struct xmobj {
                    const char *path;
                    int         id;
                    uint32_t flags;
                    uint32_t    len;
                    uint32_t    size;
                    // EMSvalueType *fill_val
                };
                */

typedef struct  xmtype_s
{
    sz_t                length; // for JSON, strs + buf
    void *              value;
    uchar               type;
}               xmtype_t;

// xmtype_t        xmtype;

JS_METHOD(      js_ems_open)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I64(filepath,0)
                    int xmid = ems_open(filepath);
                    JS_RET_I32(xmid)
}
JS_METHOD(      js_ems_create)
{
                    NAPI_ARGV(4)
                    JS_ARGV_U32(max_len, 0)
                    JS_ARGV_U32(max_sz, 1)
                    JS_ARGV_I64(filepath,2)
                    JS_ARGV_U32(flags, 3)
                    int xmid = ems_create(max_len, max_sz, filepath, flags);
                    JS_RET_I32(xmid)
}
JS_METHOD(      js_ems_destroy)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I32(xmid, 0)
                    JS_ARGV_I32(do_unlink, 1)
                    int did = ems_destroy(xmid, do_unlink);
                    JS_RET_I32(did)
}

/*              xm :: r/w
                -----------------------------------------------------------------------*/
bool            xm_read(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_ANY, EMS_TAG_ANY, NULL);
}
bool            xm_write(int mmapID, EMSvalueType *key, EMSvalueType *value)
{
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_ANY, NULL);
}
bool            xm_read_fe(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{                   //  read when full and leave empty
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_FULL, EMS_TAG_EMPTY , NULL);
}
bool            xm_read_ff(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{                   //  read when full and leave Full
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_FULL, EMS_TAG_FULL, NULL);
}
bool            xm_write_xf(int mmapID, EMSvalueType *key, EMSvalueType *value)
{                   //  WriteXF
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_FULL, NULL);
}
bool            xm_write_xe(int mmapID, EMSvalueType *key, EMSvalueType *value) {
                    //  WriteXE
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_EMPTY, NULL);
}
bool            xm_write_ef(int mmapID, EMSvalueType *key, EMSvalueType *value)
{                   //  write when empty & leave full
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_EMPTY, EMS_TAG_FULL, NULL);
}

/*              xm :: multiple readers-single writer lock
                -----------------------------------------------------------------------*/
bool            xm_read_swmr(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{                   //  Read under multiple readers-single writer lock
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_RW_LOCK, EMS_TAG_RW_LOCK, NULL);
}
int             xm_release_swmr(
                    //  Decrement the reference count of the multiple readers-single writer lock
    const int       mmapID,
    EMSvalueType*   key)
{
}

/*              xm :: utils
                --------------------------------------------------------------*/
i64             xm_key2idx(
    void*           emsBuf,
    EMSvalueType*   key,
    bool            is_mapped)
{                   //  Convert any type of key to an index
}
bool            xm_idx2key(
    int             mmapID,
    i64             idx,
    EMSvalueType*   key)
{
}
// void            xm_await_file(
//     c_ch*           path)
// {
//                     return ems_await_owner(path);
// }



/*              xm :: frame
                --------------------------------------------------------------
spsc_q*         xm_job_q;
typedef struct  xm_job_item {
    int id;
}               xm_job_item;

void            xm_frame()
{
                    while (spsc_q_consume(xm_job_q, &job_itm)) {

                    }
}

void            xm_frame_setup()
{
                    xm_job_q = spsc_q_create(sizeof(xm_job_item), 10);
}

// static int      xm_job_
*/

/*              xm :: setup
                --------------------------------------------------------------*/
void            __xm_setup(napi_env env, napi_value exports)
{
                    JS_EXPORT_FN_AS(js_ems_open,     ems_open)
                    JS_EXPORT_FN_AS(js_ems_create,   ems_create)
                    JS_EXPORT_FN_AS(js_ems_destroy,  ems_destroy)
}


/*============================================================================*/


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

/*

# c fn call api

##
- https://rosettacode.org/wiki/Call_a_function_in_a_shared_library#C

## asm
- https://en.wikibooks.org/wiki/X86_Disassembly/Calling_Conventions
- https://cs.lmu.edu/~ray/notes/gasexamples/
- https://locklessinc.com/articles/gcc_asm/
- https://www.ibm.com/developerworks/rational/library/inline-assembly-c-cpp-guide/index.html
- https://github.com/yrp604/rappel
- https://github.com/skirge/asmutils
- https://github.com/gynvael/asmloader

## stack/ideas
https://github.com/zakirullin/tiny-compiler


*/


typedef i64     return_t;
typedef char*   arg_t;
typedef void*   sym_t;

#define         __0_(x)
#define         __1_(x)          , x
#define         __2_(x)   __1_(x), x
#define         __3_(x)   __2_(x), x
#define         __4_(x)   __3_(x), x
#define         __5_(x)   __4_(x), x
#define         __6_(x)   __5_(x), x
#define         __7_(x)   __6_(x), x
#define         _1__(x) x
#define         _2__(x) x __1_(x)
#define         _3__(x) x __2_(x)
#define         _4__(x) x __3_(x)
#define         _5__(x) x __4_(x)
#define         _6__(x) x __5_(x)
#define         _7__(x) x __6_(x)
#define         _8__(x) x __7_(x)

#define         __A0(a)
#define         __A1(a)             , a[2]
#define         __A2(a)      __A1(a), a[3]
#define         __A3(a)      __A2(a), a[4]
#define         __A4(a)      __A3(a), a[5]
#define         __A5(a)      __A4(a), a[6]
#define         __A6(a)      __A5(a), a[7]
#define         __A7(a)      __A6(a), a[8]
#define         A1__(a) (a)[1]
#define         A2__(a) (a)[1] __A1((a))
#define         A3__(a) (a)[1] __A2((a))
#define         A4__(a) (a)[1] __A3((a))
#define         A5__(a) (a)[1] __A4((a))
#define         A6__(a) (a)[1] __A5((a))
#define         A7__(a) (a)[1] __A6((a))
#define         A8__(a) (a)[1] __A7((a))
#define         ___N0
#define         ___N1           , NULL
#define         ___N2      ___N1, NULL
#define         ___N3      ___N2, NULL
#define         ___N4      ___N3, NULL
#define         ___N5      ___N4, NULL
#define         ___N6      ___N5, NULL
#define         ___N7      ___N6, NULL
#define         N8___ NULL ___N7

#define         fn_t(R, A) \
                    ( R (*)( A ) )

                //  void
                // frame_apply(i64 * o)
                //     {   frame_0_apply(o+2, o[1]);
                //     }

void            frame_apply(i64 *o)
{
                    switch (o[1])
    {
                    case 0 : ( fn_t(void, void) o[2] )(); break;
                    case 1 : ( fn_t(void, _1__(i64)) o[2] )(A1__((o+2))); break;
                    case 2 : ( fn_t(void, _2__(i64)) o[2] )(A2__((o+2))); break;
                    case 3 : ( fn_t(void, _3__(i64)) o[2] )(A3__((o+2))); break;
                    case 4 : ( fn_t(void, _4__(i64)) o[2] )(A4__((o+2))); break;
                    case 5 : ( fn_t(void, _5__(i64)) o[2] )(A5__((o+2))); break;
                    case 6 : ( fn_t(void, _6__(i64)) o[2] )(A6__((o+2))); break;
                    case 7 : ( fn_t(void, _7__(i64)) o[2] )(A7__((o+2))); break;
                    case 8 : ( fn_t(void, _8__(i64)) o[2] )(A8__((o+2))); break;
}   }

                // frame_0_apply(i64 *o, size_t len)
                // {
                //     void (*fn)(
                //         i64, i64, i64, i64,
                //         i64, i64, i64, i64) = o[0];
                //     switch (len)
                //     {
                //         case 0 : fn(N8___);
                //         case 1 : fn(o[1] ___N7);
                //         case 2 : fn(o[1],o[2] ___N6);
                //         case 3 : fn(o[1],o[2],o[3] ___N5);
                //         case 4 : fn(o[1],o[2],o[3],o[4] ___N4);
                //         case 5 : fn(o[1],o[2],o[3],o[4],o[5] ___N3);
                //         case 6 : fn(o[1],o[2],o[3],o[4],o[5],o[6] ___N2);
                //         case 7 : fn(o[1],o[2],o[3],o[4],o[5],o[6],o[7] ___N1);
                //         case 8 : fn(o[1],o[2],o[3],o[4],o[5],o[6],o[7],o[8] );
                //     }
                //     return -1;
                // }

                //     void
                // frame_return(i64 * o)
                //     {   o[0] = frame_0_return(o+2, o[1]);
                //     }

void            frame_return(i64 *o)
{
    i64             r;
                    switch (o[1])
    {
                    case 0 : r = ( fn_t(i64, void) o[0] )(); break;
                    case 1 : r = ( fn_t(i64, _1__(i64)) o[2] )(A1__((o+2))); break;
                    case 2 : r = ( fn_t(i64, _2__(i64)) o[2] )(A2__((o+2))); break;
                    case 3 : r = ( fn_t(i64, _3__(i64)) o[2] )(A3__((o+2))); break;
                    case 4 : r = ( fn_t(i64, _4__(i64)) o[2] )(A4__((o+2))); break;
                    case 5 : r = ( fn_t(i64, _5__(i64)) o[2] )(A5__((o+2))); break;
                    case 6 : r = ( fn_t(i64, _6__(i64)) o[2] )(A6__((o+2))); break;
                    case 7 : r = ( fn_t(i64, _7__(i64)) o[2] )(A7__((o+2))); break;
                    case 8 : r = ( fn_t(i64, _8__(i64)) o[2] )(A8__((o+2))); break;
                    default: r = -1;
    }
                    o[0] = r;
}
            
                    // uint32_t = o[1];
                    //  o[0] = frame_0_return(o+2, o[1]);
            
                    // {   i64 (*fn)(
                    //         i64, i64, i64, i64,
                    //         i64, i64, i64, i64) = o[0];
                    //     switch (len)
                    //     {
                    //         case 0 : return (i64) fn(N8___);
                    //         case 1 : return (i64) fn(o[1] ___N7);
                    //         case 2 : return (i64) fn(o[1],o[2] ___N6);
                    //         // case 3 : return ((i64 (*)(_3__(i64)))o[0])(A3__(o));
                    //         case 3 : return (i64) fn(o[1],o[2],o[3] ___N5);
                    //         case 4 : return (i64) fn(o[1],o[2],o[3],o[4] ___N4);
                    //         case 5 : return (i64) fn(o[1],o[2],o[3],o[4],o[5] ___N3);
                    //         case 6 : return (i64) fn(o[1],o[2],o[3],o[4],o[5],o[6] ___N2);
                    //         case 7 : return (i64) fn(o[1],o[2],o[3],o[4],o[5],o[6],o[7] ___N1);
                    //         case 8 : return (i64) fn(o[1],o[2],o[3],o[4],o[5],o[6],o[7],o[8] );
                    //     }
                    //     return -1;
                    // }

/*              TODO:

                    frame arg masks, bit flip for non int-compat arg types

                    frame_masked_return
                        fl_t fl;
                        {   switch ( fl_len(fl) )
                            {   case 0 :
                                    (   fn_t( fl_ret(fl), _1__( fl_argt(fl) ) )
                                        o[0]
                                    )(  A1__( fl_argv(fl) )
                                    );
                                    break;
                            }

                        case 1 : ( fn_t( i64, _1__(i64)) o[0] )(A1__(o)); break;
*/


                    // i64 frame_2_apply(i64 *o, size_t len){
                    //     i64 (*fn)(
                    //         i64, float, i64, i64,
                    //         i64, i64, i64, i64) = o[0];
                    //     double * F = o;
                    //     switch (len) {
                    //         case 0 :
                    //         case 1 : break;
                    //         case 2 : return (i64) fn(o[1],F[2] ___N6);
                    //         case 3 : return (i64) fn(o[1],F[2],o[3] ___N5);
                    //         case 4 : return (i64) fn(o[1],F[2],o[3],o[4] ___N4);
                    //         case 5 : return (i64) fn(o[1],F[2],o[3],o[4],o[5] ___N3);
                    //     }
                    //     return -1;
                    // }

js_val_t        cframe_apply(js_env_t env, js_ctx_t ctx)
{                   JS_ARGV(1)
                    JS_ARRBUF_CAST( i64 *, o, argv[0])
                    frame_apply(o);
                    return NULL;
}
js_val_t        cframe_return(js_env_t env, js_ctx_t ctx)
{                   JS_ARGV(1)
                    JS_ARRBUF_CAST( i64 *, o, argv[0])
                    frame_return(o);
                    return NULL;
}
js_val_t        cframep_return(js_env_t env, js_ctx_t ctx)
{                   JS_ARGV(1)
                    JS_ARGV_U64(o, 0)
                    // JS_ARRBUF_CAST( i64 *, o, argv[0])
                    frame_return((i64 *)o);
                    return NULL;
}
void            test_ccall_3_target(int i, u8 u, char c)
{                   printf("    %s\n","test_ccall_3_target");
                    printf("        i=%i f=%u c=%c\n",i,u,c);
}
int             test_frame_return(int x, int y)
{                   //printf("    test_frame_return    x=%i y=%i\n",x,y);
                    return x * y * x * y - y * x + x * y * x * y - y * x;
}


#define         HASH_DEFAULT_SEED \
                    0;

        // static
        // u32_t
        // hash32(const void * buffer, size_t length) {
        //     return XXH32( buffer, length, HASH_DEFAULT_SEED );
        // }

        // static
        // u64_t
        // hash64(const void * buffer, size_t length) {
        //     return XXH64( buffer, length, HASH_DEFAULT_SEED );
        // }


        // sin ( x(max(2,3),'/',3,'*','pi')
        // sin_, max_,2, 3,_, '/',3.'*','pi',_
        // sin(max(2,3)/3*pi)


        // NAPI_METHOD(fn_ref)
        // {
        //     NAPI_ARGV(1)
        //     JS_ARRBUF_CAST(char *, arrbuf_ptr, argv[0])
        //     JS_RET_U64(arrbuf_ptr)
        // }
        // NAPI_METHOD(fn_deref)
        // {
        //     NAPI_ARGV(1)
        //     JS_BUF_CAST(char *, arrbuf_ptr, argv[0])
        //     JS_RET_U64(arrbuf_ptr)
        // }

//

        // // struct

        // struct fun_frame {
        //   void * fun_ptr;
        //   void * caller_ptr;
        // };
        // struct fun_call {
        //   uint32_t frame_h;
        // };
        // void * caller_ptrs[64];
        // void * fun_ptrs[64];
        // void * fun_keys[64];

        // void fun_caller_0_0(void * fun) {
        //   fun();
        // }

        // void fun_caller_88_0(void * fun) {
        //   fun();
        // }

        // NAPI_METHOD(fun_reg_88_8) {
        //   NAPI_ARGV(2)
        //   NAPI_ARGV_UTF8(fn_name, 64, 0)
        //   NAPI_ARGV_BUFFER_CAST(uint64_t *, f, 0)
        //   // static int fun_h;
        //   static void * fun;
        //   static uint64_t fun_ret;
        //   // fun_h = int (*f);
        //   // fun = fun_ptrs + fun_h;
        //   fun = f;
        //   fun_ret = *(f+1);
        //   argv_0 = *(f+2);
        //   argv_1 = *(f+3);
        //   void * fun = fun_ptrs + (*f);
        //   fun_ret = ( (uint64_t (*)(uint64_t, uint64_t, uint64_t, uint64_t))
        //             fun)( argv_0, argv_1 );
        //   return NULL;
        // }

        // NAPI_METHOD(fun_call_88_8) {
        //   NAPI_ARGV(1)
        //   NAPI_ARGV_BUFFER_CAST(uint64_t *, f, 0)
        //   // static int fun_h;
        //   static void * fun;
        //   static uint64_t fun_ret;
        //   // fun_h = int (*f);
        //   // fun = fun_ptrs + fun_h;
        //   fun = f;
        //   fun_ret = *(f+1);
        //   argv_0 = *(f+2);
        //   argv_1 = *(f+3);
        //   void * fun = fun_ptrs + (*f);
        //   fun_ret = ( (uint64_t (*)(uint64_t, uint64_t, uint64_t, uint64_t))
        //             fun)( argv_0, argv_1 );
        //   return NULL;
        // }

        // NAPI_METHOD(call) {
        //   NAPI_ARGV(1)
        //   NAPI_ARGV_BUFFER_CAST(struct things *, t, 0)


        //   t->operations++;

        //   return NULL;
        // }


struct things
{
    uint64_t a_number;
    char a_char;
    char input[61];
    char output[61];
    int operations;
};
        // class struct_things {
        //   constructor (buffer, offset) {
        //     this.rawBuffer = buffer
        //     this.rawBufferSlice = buffer.slice(offset)
        //     this.rawArrayBuffer = this.rawBuffer.buffer
        //     this._a_char = this.rawBuffer.slice(offset + 8, offset + 9)
        //     this.input = this.rawBuffer.slice(offset + 9, offset + 70)
        //     this.output = this.rawBuffer.slice(offset + 70, offset + 131)
        //     this._operations = new Int32Array(this.rawArrayBuffer, offset + 132, 1)
        //   }
        //   get a_char () {
        //     return this._a_char[0]
        //   }
        //   set a_char (val) {
        //     this._a_char[0] = val
        //   }
        //   get operations () {
        //     return this._operations[0]
        //   }
        //   set operations (val) {
        //     this._operations[0] = val
        //   }
        // }

JS_METHOD(tick)
    {   NAPI_ARGV(1)
        JS_BUF_CAST(struct things *, t, argv[0])
        t->operations+=2;
        return NULL;
    }
// JS_METHOD(get_num)
//     {   NAPI_ARGV(1)
//         JS_ARGV_I32(n, 0)
//         n = n << 1;
//         JS_RET_I31(n)
//     }

JS_METHOD(copy_string)
    {   NAPI_ARGV(1)
        JS_BUF_CAST(struct things *, t, argv[0])

        t->operations++;
        memcpy(t->output, t->input, 61);

        return NULL;
    }

sc_ch       version[] = "v0.0.0";

            // napi_value version_napi;

void        test_require_ptr(napi_env env)
{               napi_value global, fn, arg;
                void * fnptr;
                napi_status status = napi_get_global(env, &global);
                status = napi_get_named_property(env, global, "require", &fn);
                fnptr = &fn;
                printf("\nc require ptr ==> %"PRIu64"\n",fnptr);
                printf("\nc require ptr ==> %"PRIu64"\n",&fn);
                status = napi_get_named_property(env, global, "require", &fn);
                fnptr = &fn;
                printf("\nc require ptr ==> %"PRIu64"\n",fnptr);
}

void        test_call_to_js(napi_env env)
{               printf("\n%s", "test_call_to_js");
                napi_value global, fn, arg;

                printf("\n  %s %i", "test_call_to_js", 1);
                napi_status status = napi_get_global(env, &global);
                if (status != napi_ok) return;
            
                printf("\n  %s %i", "test_call_to_js", 2);
                status = napi_get_named_property(env, global, "__test_call_from_c", &fn);
                if (status != napi_ok) return;
            
                printf("\n  %s %i", "test_call_to_js", 3);
                // const arg = 1337
                status = napi_create_int32(env, 1337, &arg);
                if (status != napi_ok) return;
            
                napi_value* argv = &arg;
                size_t argc = 1;
                napi_value return_val;
            
                printf("\n  %s %i", "test_call_to_js", 4);
                // call
                status = napi_call_function(env, global, fn, argc, argv, &return_val);
                if (status != napi_ok) return;
            
                printf("\n  %s %i", "test_call_to_js", 5);
                // Convert the result back to a native type
                int32_t result;
                status = napi_get_value_int32(env, return_val, &result);
                if (status != napi_ok) return;
            
                printf("\n  %s %i", "test_call_to_js ==>", result);
}


s_void          __setup(napi_env env, napi_value exports)
{
                    MOD_SETUP();
}
s_void          __teardown()
{
                    MOD_TEARDOWN();
}

            NAPI_INIT()
{               __setup(env, exports);
                napi_add_env_cleanup_hook(env, &__teardown, NULL);
                // printf("\n\n%s\n\n",version);
                JS_EXPORT_FN(copy_string)
                JS_EXPORT_FN(tick)
                JS_EXPORT_FN(cframe_apply)
                JS_EXPORT_FN(cframe_return)
                JS_EXPORT_FN(cframep_return)
                // JS_EXPORT_FN(get_num)
                // JS_EXPORT_FN_DEFS()
                // NAPI_EXPORT_FUNCTION(call)
                NAPI_EXPORT_UTF8(version, strlen(version))
                // NAPI_OK( napi_create_string_utf8(env, version, strlen(version), &version_napi) )
                // NAPI_OK( napi_set_named_property(env, exports, "version", version_napi) )
                // return exports;
                // test_call_to_js(env);
                // test_require_ptr(env);

                //  char str[] = "Hello\n";
                // write(0, str, 6);   // write() to STDIN
}

/***+***+***+***|***************************************************************/
