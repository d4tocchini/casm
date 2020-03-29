#ifndef D4_NAPI_MACROS
#define D4_NAPI_MACROS

#include "napi-macros.h"

// napi_status napi_adjust_external_memory(napi_env env,
    // int64_t change_in_bytes,
    // int64_t* result);
// Call napi_adjust_external_memory() after creating the buffer and before freeing the memory, that gives the GC a more accurate view of memory consumption.
// https://github.com/nodejs/node/issues/21441https://github.com/calebsander/protodb/blob/9a67bc17d1524436d9639e73cd00a008e99d9bfa/mmap-wrapper/mmap.c``

// #define LIKELY(x)      __builtin_expect(!!(x), 1)
// #define UNLIKELY(x)    __builtin_expect(!!(x), 0)

        typedef enum
    {
        JS_ok, // napi_ok,
        JS_invalid_arg, // napi_invalid_arg, 
        JS_object_expected, // napi_object_expected,
        JS_string_expected, // napi_string_expected, 
        JS_name_expected, // napi_name_expected, 
        JS_function_expected, // napi_function_expected,
        JS_number_expected, // napi_number_expected, 
        JS_boolean_expected, // napi_boolean_expected, 
        JS_array_expected, // napi_array_expected, 
        JS_generic_failure, // napi_generic_failure,
        JS_pending_exception, // napi_pending_exception,
        JS_cancelled, // napi_cancelled,
        JS_escape_called_twice, // napi_escape_called_twice,
        JS_handle_scope_mismatch, // napi_handle_scope_mismatch, 
        JS_callback_scope_mismatch, // napi_callback_scope_mismatch,
        JS_queue_full, // napi_queue_full,
        JS_closing, // napi_closing, 
        JS_bigint_expected, // napi_bigint_expected,
        JS_date_expected, // napi_date_expected,
    }
js_status; // napi_status

        typedef napi_value
js_env_t;
// napi_env is used to represent a context that the underlying N-API implementation can use to persist VM-specific state. This structure is passed to native functions when they're invoked, and it must be passed back when making N-API calls. Specifically, the same napi_env that was passed in when the initial native function was called must be passed to any subsequent nested N-API calls. Caching the napi_env for the purpose of general reuse is not allowed.

        typedef napi_value
js_val_t;
// This is an opaque pointer that is used to represent a JavaScript value.

        typedef napi_callback_info
js_ctx_t;
// Opaque datatype that is passed to a callback function. It can be used for getting additional information about the context in which the callback was invoked.

        typedef js_val_t
(*js_cb_f)
    (   napi_env,
        napi_callback_info
    );
// Function pointer type for user-provided native functions which are to be exposed to JavaScript via N-API. Callback functions should satisfy the following signature:

#define NAPI_CALL(env, call)                                      \
  do {                                                            \
    napi_status status = (call);                                  \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      if (!is_pending) {                                          \
        const char* message = (error_info->error_message == NULL) \
            ? "empty error message"                               \
            : error_info->error_message;                          \
        napi_throw_error((env), NULL, message);                   \
        return NULL;                                              \
      }                                                           \
    }                                                             \
  } while(0)

#define JS_OK \
        NAPI_STATUS_THROWS_VOID

#define JS_METHOD(name) \
        napi_value name (napi_env env, napi_callback_info ctx)

#define JS_U32 \
        NAPI_UINT32

#define JS_I64(name, val) \
        uint64_t name; \
        bool name##lossless; \
        napi_status name##status = napi_get_value_bigint_int64(env, val, &name, &name##lossless); \
        if ( \
            __builtin_expect (name##status != napi_ok, 0) \
        ) { \
            napi_throw_error(env, "EINVAL", "JS_I64: Expected signed number"); \
        return NULL; \
    }
#define JS_U64(name, val) \
        uint64_t name; \
        bool name##lossless; \
        napi_status name##status = napi_get_value_bigint_uint64(env, val, &name, &name##lossless); \
        if ( \
            __builtin_expect (name##status != napi_ok, 0) \
        ) { \
            napi_throw_error(env, "EINVAL", "JS_U64: Expected unsigned number"); \
        return NULL; \
    }

#define JS_U64_(name, val) \
    bool bigint_lossless##name; \
    uint64_t name; \
    napi_get_value_bigint_uint64(env, val, &name, &bigint_lossless##name);

#define JS_PTR_(name, val) \
    void * name; \
    napi_get_value_external(env, val, &name);

#define JS_BUF_CAST(type, name, val) \
    type name; \
    size_t name##_len; \
    NAPI_STATUS_THROWS(napi_get_buffer_info(env, val, (void **) &name, &name##_len))

#define JS_ARRBUF_CAST(type, name, val) \
    type name; \
    size_t name##_len; \
    NAPI_STATUS_THROWS(napi_get_arraybuffer_info(env, val, (void **) &name, &name##_len))

#define JS_UTF8_MALLOC(name, val)  NAPI_UTF8_MALLOC(name, val)

/*
#define JS_DATAVIEW_CAST(type, name, val) \
    type name; \
    size_t name##_len; \
    NAPI_STATUS_THROWS(napi_get_dataview_info(env, val, (void **) &name, &name##_len))
*/

#define TO_SMI31(n) ((n < 0) ? ((n >> 1) << 1) : (n & 2147483647))
    // suitable for ensuring SMI on V8 JavaScript (64 bit)
    // max: 2³¹-1, min: -2^31
    // https://medium.com/fhinkel/v8-internals-how-small-is-a-small-integer-e0badc18b6da
    // https://www.geeksforgeeks.org/turn-off-the-rightmost-set-bit/ // return -(hash & (hash - 1))

// NAPI_STATUS_THROWS(
#define JS_RET_I32(name) \
    napi_value return_int32; \
    napi_create_int32(env, name, &return_int32); \
    return return_int32;

// NAPI_STATUS_THROWS(
#define JS_RET_I31(name) \
    napi_value return_int32; \
    napi_create_int32(env, TO_SMI31(name), &return_int32); \
    return return_int32;

// NAPI_STATUS_THROWS(
#define JS_RET_U32(name) \
    napi_value return_u32; \
    napi_create_uint32(env, name, &return_u32); \
    return return_u32;

#define JS_RET_I64(name) \
    js_val_t return_val; \
    napi_create_bigint_int64(env, name, &return_val); \
    return return_val;

// TODO: JS_OK ?
#define JS_RET_U64(name) \
    js_val_t return_val; \
    napi_create_bigint_uint64(env, name, &return_val) ; \
    return return_val;

#define JS_RET_PTR(name) \
    js_val_t return_val; \
    napi_create_external(env, name, NULL, NULL, &return_val) ; \
    return return_val;

#define JS_RET_STR(name) NAPI_RETURN_STRING((name))
#define JS_RET_UTF8(name, len) NAPI_RETURN_UTF8(name, len)


#define JS_VAL_UTF8(varname, name, len) \
    napi_value varname; \
    NAPI_STATUS_THROWS(napi_create_string_utf8(env, name, len, &varname))


#define   JS_ARGV(n) \
    napi_value argv[n];\
    size_t argc = n;\
    napi_get_cb_info(env, ctx, &argc, argv, NULL, NULL);
    // #define NAPI_ARGV(n) \
    //     napi_value argv[n]; \
    //     size_t argc = n; \
    //     NAPI_STATUS_THROWS(napi_get_cb_info(env, ctx, &argc, argv, NULL, NULL))

#define   JS_ARGV_I32             NAPI_ARGV_INT32
#define   JS_ARGV_U32             NAPI_ARGV_UINT32
#define   JS_ARGV_I64(name, i)    JS_I64(name, argv[i])
#define   JS_ARGV_U64(name, i)    JS_U64(name, argv[i])
#define   JS_ARGV_UTF8(name, size, i)          NAPI_UTF8(name, size, argv[i])
#define   JS_ARGV_PTR(name, i)    JS_PTR_(name, argv[i])
// #define   JS_EXPORT_FN(name)      NAPI_EXPORT_FUNCTION(name)
#define JS_EXPORT_FN(name) \
    { \
        napi_value name##_fn; \
        NAPI_STATUS_THROWS_VOID(napi_create_function(env, NULL, 0, name, NULL, &name##_fn)) \
        NAPI_STATUS_THROWS_VOID(napi_set_named_property(env, exports, #name, name##_fn)) \
    }
#define JS_EXPORT_FN_AS(name, key) \
    { \
        napi_value name##_fn; \
        NAPI_STATUS_THROWS_VOID(napi_create_function(env, NULL, 0, name, NULL, &name##_fn)) \
        NAPI_STATUS_THROWS_VOID(napi_set_named_property(env, exports, #key, name##_fn)) \
    }
#define JS_EXPORT_U32(name) \
    { \
        napi_value name##_uint32; \
        NAPI_STATUS_THROWS_VOID(napi_create_uint32(env, name, &name##_uint32)) \
        NAPI_STATUS_THROWS_VOID(napi_set_named_property(env, exports, #name, name##_uint32)) \
    }

#define JS_EXPORT_I32(name) \
    { \
        napi_value name##_int32; \
        NAPI_STATUS_THROWS_VOID(napi_create_int32(env, name, &name##_int32)) \
        NAPI_STATUS_THROWS_VOID(napi_set_named_property(env, exports, #name, name##_int32)) \
    }

/*
#define NAPI_EXPORT_UTF8(name, len) \
    { \
        napi_value name##_utf8; \
        NAPI_OK(napi_create_string_utf8(env, name, len, &name##_utf8)) \
        NAPI_OK(napi_set_named_property(env, exports, #name, name##_utf8)) \
    }

#define NAPI_EXPORT_STRING(name) \
    NAPI_EXPORT_UTF8(name, NAPI_AUTO_LENGTH)
*/

#endif