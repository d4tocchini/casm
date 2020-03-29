
module.exports = SUITE('c.js',() => {
let c

TEST("require", function(Y)
{
                Y(!!(c = require('../../lib/c.js')))
})
const           { N } = c

TEST('syms', function(Y,N)
{
                const o = {}
                Y( o.$cc_new = c.sym("jit_new") )
                Y( o.$crypto_hash256 = c.sym("crypto_hash256") )
                // Y( o.$ems_init = c.sym("ems_init") )
                Y( o.$tcc_new = c.sym("tcc_new"), "lib sym" )
                Y( o.$get_elf_sym_addr = c.sym("get_elf_sym_addr"), "lib static internal sym")
                LOG(o)
})
// TODO:
                // c.hook('printf',)
                // c.eval('printf("")')
    // https://www.studytonight.com/c/datatype-in-c.php
        // __alignof__(uint8_t) (unsigned long) 1
        // long double    sizeof = 16 alignof = 16
TEST('malloc types', function(Y,N)
{
            const {u32} = c;
            // console.log("------- c.js / type def + malloc")
            const p = c.malloc_tp(1, u32)
            // console.log("malloc pointer",p)
            c.mem_set_u32(p, 200.1);
                                        Y(
            c.mem_get_u32(p) == 200       ,'mem_get');
            c.mem_set_tp(u32, p, 199);
                                        Y(
            c.mem_get_tp(u32, p) == 199   ,'mem_get_tp');
            c.mem_set_tp('u32', p, 198);
                                        Y(
            c.mem_get_tp('u32', p) == 198 ,'mem_get_tp name');

            Y( c.sizeof(u32) == 4, 'sizeof(u32)' )
            Y( c.alignof(u32) == 4, `alignof(u32) ${c.alignof(u32)}` )
            // mem_set_tp(u32, p, 100)
})
TEST('struct api',function(Y,N){
    const           things_t = c.struct( "things",
                        [
                            [c.u32,              "a_number"],
                            [c.char,             "a_char"],
                            [c.arr(c.char,[16]), 'input'],
                            [c.ptr(c.char),     'output'],
                            [c.i8,               'ops'],
                            // [c.i64,              'bigint'],
                        ]
                    )
                    // console.log({things_t},c.sizeof(things_t), c.offsetin(things_t,2)        )
        Y           (c.alignof(things_t)== 8)
    const           ptr = c.malloc_tp(1, things_t)
    const           things_bytes = c.mem_get_tp(things_t, ptr)
    LOG             (   {ptr},
                        things_bytes.length,
                        `things_t is_ptr == false = ${c.type_is_ptr(things_t)}`,
                        `things_t is_complex == true = ${c.type_is_complex(things_t)}`
                    )
    const           things_buf = c.buf_from_bytes(things_bytes)

                    c.mem_setin([things_t,'ops'], ptr, 20);

    let             ops = c.mem_getin([things_t,'ops'], ptr)
    Y               (ops == 20)
    let             ops_from_buf = c.buf_getin(
                        things_buf,
                        [things_t,'ops'],
                        0)
    Y               (ops_from_buf == 20)
    const           output_p = c.malloc(16+1)
                    c.mem_set_char(output_p,'a');
                    c.mem_set_char(output_p+1n,'b');
                    c.mem_setin(
                        [things_t,'output'],
                        ptr,
                        output_p);
    let             output_from_mem
                    = c.mem_getin(
                        [things_t,'output',0],
                        ptr)

    let             output_from_ptr
                    = c.mem_getin(
                        [c.ptr(c.arr(c.char,[16])), 0],
                        ptr + c.N(c.offsetin(things_t,'output')))

    let             input = c.mem_getin([things_t,'input'], ptr)

    LOG             ({
                        // ops,ops_from_buf,things_bytes,
                        output_from_mem,
                        output_from_ptr
                    })
                    // console.log({input})
})

TEST("frames", function(Y,N)
{
let             ITERATIONS = 1000

TEST("C frames", function(Y,N)
{                   const {
                        sym,
                        frame_new,frame_mount,frame_ret,
                        frame_set,frame_return,framep_return
                    } = c

    let             fn = sym( "test_frame_return" )
    Y               (fn, "test_frame_return" )
    const           f = frame_new()
    const           ptr = c.buf_ref(f)
    Y               (f)
    T0              ("frame_return test")


    TEST( "frame_return", (Y,N)=>
    {

            let             iter = ITERATIONS
                            while (iter--)
            {
                            frame_mount(f,fn,3)

                            frame_set(f,'i32',0,iter + 55)
                            frame_set(f,'i32',1,(10.01))

                            // console.timeEnd("frame_mount")
                            // console.time("frame_return")

                            // A
                            // framep_return(ptr)
                            // let r = frame_ret(f)

                            // B
                            frame_return(f)
                            let r = frame_ret(f)

                            // console.timeEnd("frame_return")
                            // console.log(
                            //     r
                            // ,   c.frame_ret(f)
                            // ,   c.frame_get(f,'i32',0)
                            // ,   c.frame_get(f,'i32',1)
                            // ,   f
                            // )
            }
    })

    T1              ("frame_return test")
    LOG             (frame_ret(f))
}               ); // TEST

TEST( "frame_apply", (Y,N) =>
{
    const           f = c.frame_new();
                    Y(f);
    const           fn = c.sym( "test_ccall_3_target" );
                    Y(fn)

                    c.frame_mount(f, fn, 3)
                    c.frame_set(f,'i32' ,0, 6500)
                    c.frame_set(f,'u8'  ,1, 10.01)
                    c.frame_set(f,'char',2, 'x')
                    c.frame_apply(f)
    LOG             (
                        c.frame_get(f,'i32' ,0)
                    ,   c.frame_get(f,'u8'  ,1)
                    ,   c.frame_get(f,'char',2)
                    )
}               ) // TEST


TEST("JS frame simulation", function(Y,N)
{                   function fn(arr)
                    {   let x = arr[3]
                        let y = arr[4];
                        arr[0] = x * y * x * y - y * x + x * y * x * y - y * x;
                    }
    Y               (fn,"test_frame_return")
                    let f;
    T0              ("fn_return test")


    TEST("fn_return", (Y,N)=>
    {
                            let iter = ITERATIONS
                            while (iter--)
        {
                            f = [0,0,0,0,0];
                            // console.time("frame_mount")
                            f[1] = 5
                            f[2] = fn; // c.frame_mount(f,fn,3)
                            f[3] = iter + 55
                            f[4] = (10.01|0)
                            // console.timeEnd("frame_mount")
                            // console.time("frame_return")
                            fn(f)
                            let r = f[0]
                            // console.timeEnd("frame_return")
                            // console.log(
                            //     r
                            // ,   c.frame_ret(f)
                            // ,   c.frame_get(f,'i32',0)
                            // ,   c.frame_get(f,'i32',1)
                            // )
        }
    })
    T1              ("fn_return test")
    LOG             (f[0])
}               );
})

TEST('jit: lowlevel api', function(Y,N)
{
    // let i = 6;
    // while (i--) {
    // T0          ("jit compile")
                let lib
    Y(
                lib = c.jit_alloc_wdefaults()
    ,"alloc")
    N(
                c.jit_string(lib, `

                    int foo(int a, int b)  {
                        return (a+b)<<1;
                    }
                    void js_foo(int * argv, int * ret) {
                        *ret = foo(argv[0], argv[1]);
                    }

                `)
    ,"string")
    N(
                c.jit_compile(lib)
    ,"compile");
                const $foo = c.jit_sym(lib,'js_foo')

    // T1              ("jit compile")
    Y($foo);
    // T0              ("jit frame")
                const argc = 2; const rets = 1;
                const $argv = c.malloc_align((2)<<2, 8|0);
                const $ret = c.malloc_align((1)<<2, 4|0);
                c.mem_set_i32($argv, 111|0);
                c.mem_set_i32($argv+4n, 222|0);
    // T1              ("jit frame")
    // T0              ("jit callread")
                c.callr($foo, $argv, $ret)
                const val = c.mem_get_i32($ret);
                c.jit_free(lib)
    // T1              ("jit callread")
    Y(val==666)
    LOG({val, $foo})
    // }
})
TEST('jit_run', function(Y,N)
{
    let i = 3;
    while(i--)
    {
                const exe = c.jit_alloc_wdefaults()
    T0("string")
                c.jit_string(exe, `
                    void main(int argc, char** argv) {
                        printf("%s %i\n","HELLO WORLD",${i});
                    }
                `)
    T1("string")
    T0("run")
                c.jit_run(exe,0,0)
    T1("run")
                c.jit_free(exe)
    }
})
TEST('jit: ptr binding', function(Y,N)
{
                const ptr = c.malloc_align(4,4)
        Y       (ptr %4n == 0n, "aligned")
        // LOG({ptr})

                const cdef = c.jit_alloc_wdefaults()
                c.jit_string(cdef,`

                    const volatile int * x = (const int *)${ptr};

                    void fn() {
                        int v = *x;
                        printf("x = %u\n", *x);
                        *x = v >> 1;
                    }

                `)
                c.jit_compile(cdef);

                const $fn = c.jit_sym(cdef,'fn');



                const ofs = c.mem_offset(ptr)

                const u32a = c.mem_slice_tp(c.u32, ptr, 1)
                const nuff = Buffer.from(u32a.buffer,u32a.byteOffset,
                    u32a.byteLength)

                let i = 2
                while(i--)
                {
                                        let val
                    T0('call0')
                                        c.mem_set_u32(ptr, 2222);
                                        c.call0($fn);
                                        val=c.mem_get_u32(ptr);
                    T1('call0') 
                    Y(val == 1111)  
                    
                    T0('call0') 
                                        u32a[0] = 2222
                                        c.call0($fn);
                                        val=u32a[0]
                    T1('call0') 
                    Y(val == 1111)  

                    T0('call0') 
                                        nuff.writeInt32LE(2222)
                                        c.call0($fn);
                                        val=nuff.readInt32LE(0)
                    T1('call0')
                    Y(val == 1111)
                }
                    c.jit_free(cdef);
})
TEST_skip('jit: lib apis', function(Y,N)
{
                const cdef = c.jit_alloc_wdefaults()
                c.jit_string(cdef,`

typedef   signed int        i32;
typedef unsigned int        u32;
typedef   signed long long  i64;
typedef unsigned long long  u64;
typedef   i64            sz_t;
typedef unsigned     char   uch;
typedef        const char   c_ch;

#define EMS_USE_MAP             0b00000000100000000000000000000000
#define EMS_PERSIST             0b00000001000000000000000000000000
#define EMS_CLEAR               0b00000010000000000000000000000000
#define EMS_FILL                0b00000100000000000000000000000000
#define EMS_FILL_JSON           0b00001100000000000000000000000000
#define EMS_SET_E               0b00010000000000000000000000000000
#define EMS_SET_F               0b00110000000000000000000000000000
#define EMS_PIN_THREADS         0b01000000000000000000000000000000
#define EMS_MLOCK_PCT_MASK      0b00000000000000000000000001111111

#define EMS_TYPE_UNDEFINED      ((uch)0)
#define EMS_TYPE_INTEGER        ((uch)1)
#define EMS_TYPE_STRING         ((uch)2)
#define EMS_TYPE_BUFFER         ((uch)3)
#define EMS_TYPE_FLOAT          ((uch)4)
#define EMS_TYPE_BOOLEAN        ((uch)5)
#define EMS_TYPE_INVALID        ((uch)6)
#define EMS_TYPE_JSON           ((uch)7)

typedef struct {
    sz_t length;  // Defined only for JSON and strings
    void * value;
    uch type;
}
xm_t;           // Internal EMS representation of a JSON value

static xm_t  * volatile  rkey;
static xm_t  * volatile  rval;
static xm_t  * volatile  wkey;
static xm_t  * volatile  wval;
static int  *  volatile xmid;
static uch  *  volatile status;

void _bind_rkey(void* addr){ rkey = addr; }
void _bind_rval(void* addr){ rval = addr; }
void _bind_wkey(void* addr){ wkey = addr; }
void _bind_wval(void* addr){ wval = addr; }
void _bind_xmid(int* addr){ xmid = addr; }
void _bind_status(uch* addr){ status = addr; }

void map_create()
{
    *xmid = ems_create(16U, 1024U << 3, "/tmp/m.xm", EMS_PERSIST|EMS_CLEAR|EMS_USE_MAP );
}
void write()
{
   *status = xm_write(0, wkey, wval);
}
void read()
{
    xm_t ret={.length=0, .value=(void*)0, .type=EMS_TYPE_STRING};
    *status = xm_read(0, rkey, &ret);
    sz_t rlen = ret.length;
    if (rlen) {
        rval->length = rlen;
        rval->type = ret.type;
        memcpy(rval->value, ret.value, rlen);
    }
    else {
        rval->length = 0;
    }
}

                `)
/*
    xm_t key={.length=3, .value="ab", .type=EMS_TYPE_STRING};
    xm_t val={.length=4, .value="abc", .type=EMS_TYPE_STRING};
    *status = xm_write(0, &key, &val);
*/
/*
    printf("%s %i %s\n","xm_read", rkey->type, rkey->value);
    xm_t key={.length=2, .value="ab", .type=EMS_TYPE_STRING};
    // printf("%s %i %s\n","xm_read", rkey->type, rkey->value);

    // *status = xm_read(0, &key, &ret);
    printf("read v ==> %u %s\n", ret.length,ret.value);
    // printf("read v ==> %u %s\n", rval->length,rval->value);
    printf("read v ==> %u %s\n", rval->length, rval->value);
*/
                c.jit_compile(cdef);
                const $map_create = c.jit_sym(cdef,'map_create')
                const $write = c.jit_sym(cdef,'write')
                const $read = c.jit_sym(cdef,'read')
                const $_bind_rkey = c.jit_sym(cdef,'_bind_rkey')
                const $_bind_rval = c.jit_sym(cdef,'_bind_rval')
                const $_bind_wkey = c.jit_sym(cdef,'_bind_wkey')
                const $_bind_wval = c.jit_sym(cdef,'_bind_wval')
                const $_bind_xmid = c.jit_sym(cdef,'_bind_xmid')
                const $_bind_status = c.jit_sym(cdef,'_bind_status')

    Y($map_create>0n)
    Y($write>0n)
    Y($read>0n)
    Y($_bind_rkey>0n)
    Y($_bind_rval>0n)
    Y($_bind_wkey>0n)
    Y($_bind_wval>0n)
    Y($_bind_xmid>0n)
    Y($_bind_status>0n)
    LOG({
        $map_create,
        $write,
        $read,
        $_bind_rkey,
        $_bind_rval,
        $_bind_wkey,
        $_bind_wval,
        $_bind_xmid,
        $_bind_status,
    })

                const $rkey = c.malloc_align(24<<2,16)
                const $rval = $rkey + 24n
                const $wkey = $rval + 24n
                const $wval = $wkey + 24n

                const $xmid = c.malloc_align(4,4)
                const $status = c.malloc_align(4,4)

                c.call1($_bind_rkey, $rkey)
                c.call1($_bind_rval, $rval)
                c.call1($_bind_wkey, $wkey)
                c.call1($_bind_wval, $wval)
                c.call1($_bind_xmid, $xmid)
                c.call1($_bind_status, $status)

                const INVALID = 0;
                const BOOLEAN = 1;
                const STRING = 2;
                const FLOAT = 3;
                const INTEGER = 4;
                const UNDEFINED = 5;
                const JSON = 6;
                const BUFFER = 7;


                const {
                    xxhash64,
                    malloc_align,
                    mem_set_string, mem_get_string,
                    mem_set_i64,    mem_get_i64,
                    mem_set_i32,    mem_get_i32,
                    mem_set_u8,     mem_get_u8,
                } = c
                const _STR_SZ = 1<<11 ; // 2048
                const _STR_PTR = malloc_align(_STR_SZ, 16)

                let bytes_k, bytes_v
                // function wkey_string(str) {
                //     const l = str.length + 1
                //     c.mem_set_i32($wkey, l)
                //     bytes_k = new Uint8Array(l)
                //     c.bytes_set_string(bytes_k,str,0)
                //     bytes_k[l - 1] = 0;
                //     c.mem_set_i64($wkey + 8n, c.arrbuf_ref(bytes_k.buffer))
                //     c.mem_set_u8($wkey + 16n, STRING)
                // }

            //     function wval_string(str) {
            //         const l = str.length + 1
            //         c.mem_set_i64($wval, c.N(l))
            //         bytes_v = new Uint8Array(l)
            //         c.bytes_set_string(bytes_v,str,0)
            //         bytes_v[l - 1] = 0;
            // // console.log( c.bytes_get_string(bytes_v,0,l) )
            //         c.mem_set_i64($wval + 8n, c.arrbuf_ref(bytes_v.buffer))
            //         c.mem_set_u8($wval + 16n, STRING)
            //     }
                // function rval_string(str) {
                //     const l = str.length + 1
                //     c.mem_set_u32($val, l)
                //     bytes_k = new Uint8Array(l)
                //     c.bytes_set_string(bytes_k,str,0)
                //     c.mem_set_i64($rkey + 8n, c.arrbuf_ref(bytes_k.buffer))
                //     c.mem_set_u8($rkey + 16n, STRING)
                // }


                function key_strhash($key, str) {
                    const l = str.length
                    mem_set_string(_STR_PTR, str)
                    mem_set_i64($key + 8n,
                        xxhash64(_STR_PTR, l, 0n))
                    mem_set_u8($key + 16n, INTEGER)
                }
                function key_str($key, str) {
                    const l = str.length
                    // ->length  // Defined only for JSON and strings
                    mem_set_i32($key, l)
                    // ->val -->
                    mem_set_string(_STR_PTR, str)
                    // ->val
                    mem_set_i64($key + 8n, _STR_PTR)
                    // ->type
                    c.mem_set_u8($key + 16n, STRING)
                }
                function key_str($key, str) {
                    const l = str.length
                    // ->length  // Defined only for JSON and strings
                    mem_set_i32($key, l)
                    // bytes_k = new Uint8Array(l)
                    // c.bytes_set_string(bytes_k,str,0)
                    // bytes_k[l - 1] = 0;
                    // ->value
                    mem_set_string(_STR_PTR, str)
                    mem_set_i64($key + 8n, _STR_PTR)
                    // ->type
                    c.mem_set_u8($key + 16n, STRING)
                }

                // point read val to tmp ptr
                mem_set_i64($rval + 8n, _STR_PTR)
                mem_set_u8($rkey + 16n, INTEGER)
                mem_set_u8($wkey + 16n, INTEGER)

                function ems_sizeof(ptr) {
                    return mem_get_i32(ptr)
                }
                // function ems_valof(ptr) {
                //     return mem_get_i32(ptr+8n)
                // }
                function ems_typeof(ptr) {
                    return mem_get_u8(ptr+16n)
                }
                function write(key, val)
                {
                    key_strhash($wkey,key)
                    key_str($wval, val)
                    c.call0($write)
                }
                function read(key) {
                    key_strhash($rkey,key)
                    c.call0($read)
                    const l = ems_sizeof($rval) 
                    if (l) {
                        // ems_typeof($rval)
                        return mem_get_string(_STR_PTR, l)
                    }
                    return null
                }
                function writebox(key,val,boxkey)
                {
                    key_strhash($wkey,key)
                    encrypt_box_val()
                    key_str($wval, val)
                }
                function readbox(key,boxkey)
                {

                }
                function map_create() {
                    c.call0($map_create);
                }

                map_create();

                let ITER = 100
                let i
                T0('write')
                // write("abc", "ABC");
                // write("xxyyzz", "XXYYZZ");
                // read("abc");
                let input;
                i = ITER;
                while(i--){
                    input = "hey there bill boy" + i + "abcd9876"
                    write("msg" + (i & 1), input);
                }
                T1('write')
                T0('read')
                let ouput;
                i = ITER;
                while(i--){
                    ouput = read("msg" + (i & 1));
                }
                T1('read')
                console.log({input,ouput})
                Y(input === ouput, "write/read")


                console.log({
                    input,ouput,    mem:mem_get_string(_STR_PTR,input.length),
                    inlen:input.length, outlen:ems_sizeof($rval),
                });

                (function(){

                let jsmap = new Map();
                T0('write-js')
                let input;
                i = ITER;
                while(i--){
                    input = "hey there bill boy" + i + "abcd9876"
                    jsmap.set("msg" + (i & 1), input);
                }
                T1('write-js')
                T0('read-js')
                let ouput;
                i = ITER;
                while(i--){
                    ouput = jsmap.get("msg" + (i & 1));
                }
                T1('read-js')

                })();

                // T0('data')
                // // write("msg", "hey there bill boy");
                // read("msg");
                // T1('data')

                c.jit_free(cdef);


})
TEST_skip('jit_run_bench', function(Y,N)
{
    // const c=require('./c.js')
    const LOOPS = 1000;
    function jlog(){ let i = LOOPS; while(i--){ console.log('HELLO WORLD',i); } }
    function clog(){ const exe = c.jit_alloc_wdefaults(); c.jit_string(exe, `void main(int argc, void*argv) {     int i = ${LOOPS};     while(i--)         printf("%s %i\\n","HELLO WORLD",i); }`); c.jit_run(exe,0,0); c.jit_free(exe); }
    console.time('jlog'); jlog(); console.timeEnd('jlog');
    console.time('clog'); clog(); console.timeEnd('clog');

})
// TEST_skip('jit: fn api', function(Y,N)
// {

//                 const bar = c.fn([[i32,'bar'],[i32,'a'],[i32,'b']],`
//                     printf("foo(%d, %d)\\n\", a, b);
//                     return (a*b)<<1;
//                 `)
//                 const barval = bar(111, 222);
//                 Y(barval === 333);
// })

// TEST_skip('jit: fn api w/ static lib syms', function(Y,N)
// {
//                 const encrypt = c.fn([[i32,'bar'],[i32,'a'],[i32,'b']],`
//                     printf("foo(%d, %d)\\n\", a, b);
//                     return (a*b)<<1;
//                 `)
//                 const barval = bar(111, 222);
//                 Y(barval === 333);
// })




// function encrypt_string(msg, key) {
//     const msg_sz = msg.length
//     const out = c.malloc()

//     _encrypt(args)
// }

// _encrypt(args) {
//     c.compile_fn('encrypt',
//         'u8* out, constant void* msg, sz_t msg_sz, constant u8*key',`
//             crypto_box_enc(out, msg, msg_sz, key);
//     `)
// }

// function _encrypt(u8_p$out, c_void_p$msg, sz$msg_sz) {`

// `}

TEST('crypto', function(Y,N)
{ T0('crypto'); for (let i = 0; i<100;i++){
            const {
                crypto_hash256,
                crypto_box_enc, crypto_box_dec,
                crypto_box_verify,
                malloc_align,
                mem_set_string, mem_get_string,
                mem_get_hex,
            } = c

const       hydro_secretbox_CONTEXTBYTES = 8
const       hydro_secretbox_HEADERBYTES = (20 + 16)
const       hydro_secretbox_KEYBYTES = 32
const       hydro_secretbox_PROBEBYTES = 16

            const pass = "AEXILE"
            const pass_l = pass.length
            const pass_p = malloc_align(pass_l,8)
            mem_set_string(pass_p, pass)
            Y(mem_get_string(pass_p, pass_l) == pass)

            const secret_p = malloc_align(32,16)
            crypto_hash256(secret_p, pass_p, pass_l, c.NULL)
            const secret_x = mem_get_hex(secret_p, 32)

            // console.log({secret_x})

            const msg = "Hello, Secret World..."
            const msg_l = msg.length
            const msg_p = malloc_align(msg_l, 8)
            mem_set_string(msg_p, msg)

            // encrypt
            const box_l = box_size(msg_l)
            const box_p = malloc_align(box_l, 8)
            crypto_box_enc(box_p,
                msg_p, msg_l,
                secret_p, 0)

            const out_l = msg_size(box_l)
            const out_p = malloc_align(out_l, 8)
            let out

            // decrypt
            crypto_box_dec(out_p,
                box_p, box_l,
                secret_p, 0)
            out = mem_get_string(out_p, out_l)
        Y   (out === msg)

            // wrong secret key ==> null bytes
            crypto_box_dec(out_p,
                box_p, box_l,
                secret_p - 1n, 0)
            out = mem_get_string(out_p, out_l)
        Y   (out !== msg)
        Y   (out.charCodeAt(0) == out.charCodeAt(1) == out.charCodeAt(2) == 0)



function    box_size(mlen)
{               return hydro_secretbox_HEADERBYTES + mlen
}
function    msg_size(box_sz)
{               return box_sz - hydro_secretbox_HEADERBYTES
}


}T1('crypto')})


TEST('crypto w/ c_malloc', function(Y,N)
{ T0('crypto'); for (let i = 0; i<100;i++){
            const {
                crypto_hash256,
                crypto_box_enc, crypto_box_dec,
                crypto_box_verify,
                malloc_align,
                c_malloc_align, c_free,
                c_malloc_str, c_deref_str,
                mem_set_string, mem_get_string,
                mem_get_hex,
            } = c

const       hydro_secretbox_CONTEXTBYTES = 8
const       hydro_secretbox_HEADERBYTES = (20 + 16)
const       hydro_secretbox_KEYBYTES = 32
const       hydro_secretbox_PROBEBYTES = 16

            const pass = "AEXILE"
            const pass_l = pass.length
            const pass_p = c_malloc_str(pass, pass_l+1)
            Y(c_deref_str(pass_p) == pass)
            // Y(c_deref_utf8(pass_p) == pass)

            const secret_p = malloc_align(32,16)
            crypto_hash256(secret_p, pass_p, pass_l, c.NULL)
            const secret_x = mem_get_hex(secret_p, 32)
            // console.log(c_deref_str(pass_p))
            c_free(pass_p)

            const msg = "Hello, Secret World..."
            const msg_l = msg.length
            const msg_p = malloc_align(msg_l, 8)
            mem_set_string(msg_p, msg)

            // encrypt
            const box_l = box_size(msg_l)
            const box_p = malloc_align(box_l, 8)
            crypto_box_enc(box_p,
                msg_p, msg_l,
                secret_p, 0)

            const out_l = msg_size(box_l)
            const out_p = malloc_align(out_l, 8)
            let out

            // decrypt
            crypto_box_dec(out_p,
                box_p, box_l,
                secret_p, 0)
            out = mem_get_string(out_p, out_l)
        Y   (out === msg)

            // wrong secret key ==> null bytes
            crypto_box_dec(out_p,
                box_p, box_l,
                secret_p - 1n, 0)
            out = mem_get_string(out_p, out_l)
        Y   (out !== msg)
        Y   (out.charCodeAt(0) == out.charCodeAt(1) == out.charCodeAt(2) == 0)



function    box_size(mlen)
{               return hydro_secretbox_HEADERBYTES + mlen
}
function    msg_size(box_sz)
{               return box_sz - hydro_secretbox_HEADERBYTES
}


}T1('crypto')})

})