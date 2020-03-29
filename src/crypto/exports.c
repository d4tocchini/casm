

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


