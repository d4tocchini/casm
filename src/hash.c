
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