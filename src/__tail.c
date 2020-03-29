

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
