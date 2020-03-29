const v8 = require('v8')
const c = require('./c.js');
const {
    NULL,
    bindings,
    c_malloc,       // c_malloc_align,
    c_malloc_str,
    c_free,
    malloc_align,
    mem_get_string, // mem_set_string,
    mem_set_u64,    mem_set_i64,
    mem_get_i64,    // mem_get_u64,
    mem_set_u32,    mem_set_i32,
    mem_get_u32,    mem_get_i32,
    mem_set_u8,
    mem_get_u8,     // mem_get_i8 , // mem_set_i8 ,
    mem_get_hex,
    mem_slice_u8 ,
    jit_sym,
    jit_string,
    jit_compile,
    arrbuf_ref,
    // crypto_box_verify,
} = c;
const {
    jit_load_wdefaults,
    module_init_count,
    xor256to64,
    // xxhash64,
    xxhash32,   //xxhash31, TODO:
    str_hash64,     str_hash31,
    ems_open,
    ems_create,
    ems_destroy,
    crypto_str_hash256, //crypto_hash256, crypto_hash64,  crypto_hash32,
    crypto_box_enc, crypto_box_dec,
    // _eval_run,
} = bindings;

// TYPES
const   UNDEFINED       = 0
const   INTEGER         = 1
const   STRING          = 2
const   BUFFER          = 3
const   FLOAT           = 4
const   BOOLEAN         = 5
const   INVALID         = 6
const   JSON            = 7

// INST FLAGS
const   REOPEN          = 0b00000000010000000000000000000000;

const   USE_MAP         = 0b00000000100000000000000000000000;
const   PERSIST         = 0b00000001000000000000000000000000;
const   CLEAR           = 0b00000010000000000000000000000000;
const   FILL            = 0b00000100000000000000000000000000;
const   FILL_JSON       = 0b00001100000000000000000000000000;
const   SET_E           = 0b00010000000000000000000000000000;
const   SET_F           = 0b00110000000000000000000000000000;
const   PIN_THREADS     = 0b01000000000000000000000000000000;
const   MLOCK_PCT_MASK  = 0b00000000000000000000000001111111;
        // const MAP_T = 0;  // const ARR_T = 1;

// CRYPTO CONSTS
const   hydro_secretbox_HEADERBYTES   = (20 + 16)
// const   hydro_secretbox_CONTEXTBYTES  = 8
// const   hydro_secretbox_KEYBYTES      = 32
// const   hydro_secretbox_PROBEBYTES    = 16

// JIT LIB
const   t0 = process.hrtime();
const   cdef_key = 0;
const   cdef = jit_load_wdefaults(cdef_key);

if (module_init_count == 1) { // incase chromium process reuse!
        jit_string( cdef, `
#define NULL                    ((void*)0)

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

typedef   signed int            i32;
typedef unsigned int            u32;
typedef   signed long long      i64;
typedef unsigned long long      u64;
typedef   i64                   sz_t;
typedef unsigned     char       uch;
typedef        const char       c_ch;
typedef struct                  // Internal EMS representation of a JSON value
{
            sz_t    length;         // Defined only for JSON and strings
            void *  value;
            uch     type;
}                               xm_t;

static  xm_t * volatile         rkey;
static  xm_t * volatile         rval;
static  xm_t * volatile         wval;
static  int  * volatile         xid;
static  int  * volatile         status;
static  i64  * volatile         timer = NULL;

void    _bind_rkey(void* addr)  { rkey = addr; }
void    _bind_rval(void* addr)  { rval = addr; }
void    _bind_wval(void* addr)  { wval = addr; }
void    _bind_xid(int* addr)    { xid = addr; }
void    _bind_status(uch* addr) { status = addr; }
void    _bind_timer(void* addr) { timer = addr; }

#define TAG_X                   ((uch)4) // Never stored, used for matching
#define TAG_RW                  ((uch)3) // read/write lock
#define TAG_BUSY                ((uch)2)
#define TAG_E                   ((uch)1)
#define TAG_F                   ((uch)0)

#define BEGIN() \
            i64 timeout = *timer;


#define WRITE_W_TAGS(wtag_i, wtag_f)\
            BEGIN()\
            *status = EMSwriteUsingTags(*xid, rkey, wval, (wtag_i), (wtag_f), &timeout);

#define READ_W_TAGS(rtag_i, rtag_f)\
            BEGIN()\
            *status = EMSreadUsingTags(*xid, rkey, rval, (rtag_i), (rtag_f), &timeout);

void    writeXX() { WRITE_W_TAGS( TAG_X, TAG_X) }
void    writeXF() { WRITE_W_TAGS( TAG_X, TAG_F) }
void    writeXE() { WRITE_W_TAGS( TAG_X, TAG_E) }
void    writeEF() { WRITE_W_TAGS( TAG_E, TAG_F) }
void    writeFF() { WRITE_W_TAGS( TAG_F, TAG_F) }
void    write()   { writeXX(); }
void    readXX()  { READ_W_TAGS( TAG_X,  TAG_X) }
void    readFE()  { READ_W_TAGS( TAG_F,  TAG_E) }
void    readEF()  { READ_W_TAGS( TAG_E,  TAG_F) }
void    readFF()  { READ_W_TAGS( TAG_F,  TAG_F) }
void    read()    { readXX(); }
void    stk_produce() {BEGIN() *status = EMSpush   (*xid, wval, &timeout); }
void    stk_consume() {BEGIN() *status = EMSpop    (*xid, rval, &timeout); }
void    stk_get_top() {BEGIN() *status = ems_stack_get_top(*xid, &timeout); }
void    q_produce()   {BEGIN() *status = EMSenqueue(*xid, wval, &timeout); }
void    q_consume()   {BEGIN() *status = EMSdequeue(*xid, rval, &timeout); }

void    readRW()  { READ_W_TAGS( TAG_RW, TAG_RW) }
void    releaseRW()  {
    BEGIN() *status = EMSreleaseRW(*xid, rkey);
}
        `)
        jit_compile(cdef);
}

// JIT SYMBOLS

const   $writeXX = jit_sym(cdef,'writeXX');
const   $writeXF = jit_sym(cdef,'writeXF');
const   $writeXE = jit_sym(cdef,'writeXE');
const   $writeEF = jit_sym(cdef,'writeEF');
const   $writeFF = jit_sym(cdef,'writeFF');
const   $write = jit_sym(cdef,'write')
const   $readXX = jit_sym(cdef,'readXX');
const   $readFE = jit_sym(cdef,'readFE');
const   $readEF = jit_sym(cdef,'readEF');
const   $readFF = jit_sym(cdef,'readFF');
const   $readRW = jit_sym(cdef,'readRW');
const   $releaseRW = jit_sym(cdef,'releaseRW');

const   $read = jit_sym(cdef,'read');
const   $stk_produce = jit_sym(cdef,'stk_produce');
const   $stk_consume = jit_sym(cdef,'stk_consume');
const   $stk_get_top = jit_sym(cdef,'stk_get_top');
const   $q_produce = jit_sym(cdef,'q_produce');
const   $q_consume = jit_sym(cdef,'q_consume');
const   $_bind_rkey = jit_sym(cdef,'_bind_rkey');
const   $_bind_rval = jit_sym(cdef,'_bind_rval');
const   $_bind_wval = jit_sym(cdef,'_bind_wval');
const   $_bind_xid = jit_sym(cdef,'_bind_xid');
const   $_bind_status = jit_sym(cdef,'_bind_status');
const   $_bind_timer = jit_sym(cdef,'_bind_timer');

console.log("jit d ", (process.hrtime(t0)[1]/1e6) + "ms" )

// STATIC ALLOC

// const   TYPE_BYTE_SIZE = 24;

const   $rkey = malloc_align(24+24+24,16);
const   $rkey_data = $rkey + 8n;
// const   $rkey_type = $rkey + 16n;

const   $rval = $rkey + 24n;

const   $wval = $rval + 24n;
const   $wval_data = $wval + 8n;
const   $wval_type = $wval + 16n;

const   $xid = malloc_align(4+4,8);
const   $status = $xid + 4n;
const   $timer = malloc_align(8,8);
const   _STR_SZ = 1<<11; // 2048
const   _STR_PTR = malloc_align(_STR_SZ, 16);
const   secret_p = malloc_align(32,16);
        // TODO: change to mem_malloc ot something..

let     secret_h64 = 0n;

// JS-JIT PTR BINDS

c.call1($_bind_rkey, $rkey);
c.call1($_bind_rval, $rval);
c.call1($_bind_wval, $wval);
c.call1($_bind_xid, $xid);
c.call1($_bind_status, $status);
c.call1($_bind_timer, $timer);
// ... point read val to tmp ptr
mem_set_i64($rval + 8n, _STR_PTR);
mem_set_u8($rkey + 16n, INTEGER);

function get_status() {return mem_get_u32($status);}
function set_xid(val) {return mem_set_u32($xid, val);}
function get_xid() {return mem_get_u32($xid);}

function hash64(str) {return str_hash64(str, str.length, 0n);}
function hash32(str) {return str_hash31(str, str.length, 0);}
function secret_hash64(str) {return str_hash64(str, str.length, secret_h64);}
function secret_hash32(str) {return str_hash31(str, str.length, Number(secret_h64));}
function secret_set_phrase(pass) {
    if (typeof pass !== 'string') {
        console.warn(`secret_set_phrase requires string, got ${pass}`)
        return
    }
    crypto_str_hash256(secret_p, pass, pass.length, NULL)
    secret_h64 = xor256to64(secret_p);
}
function secret_get_hex() { return mem_get_hex(secret_p, 32); }
function secret_get_ptr() { return secret_p; }

function box_size(mlen){ return hydro_secretbox_HEADERBYTES + (mlen|0); }
function msg_size(box_sz){ return (box_sz|0) - hydro_secretbox_HEADERBYTES; }

function _ems_sizeof(ptr) { return mem_get_i32(ptr)|0; }
function _ems_typeof(ptr) { return mem_get_u8(ptr+16n)|0; }
// function ems_valof(ptr) { return mem_get_i32(ptr+8n); }

let _timeout_ns = 0n;

function get_timeout(ms) { return _timeout_ns }
function set_timeout(ms) {mem_set_i64($timer, (_timeout_ns = (ms)? ms_to_ns(ms|0) : 0n) );}
function ms_to_ns(ms) {
    const x = BigInt((ms<<10) - (ms<<5) + (ms<<3));
    return (x<<10n) - (x<<5n) + (x<<3n);
}

const _key_by_typeof = {
    'string':key_str,
    'number':key_i32,
    'bigint':key_i64,
    'object':key_json,
    'function':key_fn,
    'undefined':key_undefined
};
const _set_by_typeof = {
    'string':set_str,
    'number':set_i32,
    'bigint':set_i64,
    'object':set_json,
    'function':set_fn,
    'undefined':set_undefined
};
const GET_BY_TYPE_IDX = [
    function(){ return null; }, // UNDEFINED
    get_i32_unsafe, // INTEGER TODO: i32
    get_str, // STRING
    get_buf, // BUFFER
    get_float, // TODO: FLOAT
    get_bool,
    function(){ return undefined; }, // TODO: INVALID?
    get_json, // JSON
];
const GET_UNSAFE_BY_TYPE_IDX = [
    function(){ return null; }, // UNDEFINED
    get_i32_unsafe, // INTEGER TODO: i32
    get_str_unsafe, // STRING
    get_buf_unsafe, // BUFFER
    get_float, // TODO: FLOAT
    get_bool,
    function(){ return undefined; }, // TODO: INVALID?
    get_json_unsafe, // JSON
];

function key(k) {_key_by_typeof[typeof k](k);}
function key_str(k) {mem_set_i64($rkey_data, hash64(k));}
function key_i32(k) {key_i64(BigInt(k|0));}
function key_i64(k) {mem_set_i64($rkey_data, k);}
function key_json(k) {throw new Error('key_json WIP...');}
function key_fn(k) {throw new Error('key_fn WIP...');}
function key_undefined() {throw new Error('key_undefined WIP...');}

let _wval_str_p = c_malloc(1);
function set(v) {_set_by_typeof[typeof v](v);}
function set_str(str) {
    const l = str.length;
    mem_set_u32($wval, l);
    c_free(_wval_str_p);
    _wval_str_p = c_malloc_str(str,l+1);
    mem_set_u64($wval_data, _wval_str_p);
    mem_set_u8($wval_type, STRING);
}
function set_i32(val) {
    mem_set_i32($wval_data, val|0);
    mem_set_u8($wval_type, INTEGER);
}
function set_i64(val) {
    mem_set_i64($wval_data, val);
    mem_set_u8($wval_type, INTEGER);
}
function set_buf({buffer,byteLength,byteOffset}) {
    mem_set_u32($wval, byteLength);
    mem_set_u64($wval_data, arrbuf_ref(buffer) + BigInt(byteOffset));
    mem_set_u8($wval_type, BUFFER);
}
function set_json(val) {
    const {buffer,byteLength} = v8.serialize(val);
    mem_set_u32($wval, byteLength);
    mem_set_u64($wval_data, arrbuf_ref(buffer));
    mem_set_u8($wval_type, JSON);
}
function set_fn() {throw new Error('set_fn WIP...');}
function set_undefined() {throw new Error('set_undefined WIP...');}

function get()  {return GET_BY_TYPE_IDX[_ems_typeof($rval)]();}
function get_str()  {const l = _ems_sizeof($rval); return (l>0) ? get_str_unsafe(l) : null;}
function get_buf()  {const l = _ems_sizeof($rval); return (l>0) ? get_buf_unsafe(l) : null;}
function get_json() {const l = _ems_sizeof($rval); return (l>0) ? get_json_unsafe(l) : null;}
function get_str_unsafe(l) {return mem_get_string(_STR_PTR, l);}
function get_buf_unsafe(l) {return mem_slice_u8(_STR_PTR, l);}
function get_json_unsafe(l){return v8.deserialize( mem_slice_u8(_STR_PTR, l) );}
// TODO:  TEST THIS WORKS CROSS PLATFORM, CURRENTLY INTS CAST TO i64
function get_i32() {return (_ems_typeof($rval) === INTEGER) ? get_i32_unsafe() : null;}
function get_i64() {return (_ems_typeof($rval) === INTEGER) ? get_i64_unsafe() : null;}
function get_i32_unsafe() {return mem_get_i32(_STR_PTR)|0;}
function get_i64_unsafe() {return mem_get_i64(_STR_PTR);}
function get_float(){throw new Error('get_float WIP...');}
function get_bool() {return (!get_i32())^1;}
function get_fn() {throw new Error('get_fn WIP...');}

function secure_key(k) {mem_set_i64($rkey_data, secret_hash64(k));}
function secure_key_str(k) {mem_set_i64($rkey_data, secret_hash64(k));}

function secure_set(msg) {
    if (typeof msg === 'string')
        return secure_set_str(msg);
    else if (msg.buffer)
        return secure_set_buffer(msg);
    return secure_set_json(val);
}
function secure_set_str(msg) {
    const msg_l = msg.length;
    const box_l = box_size(msg_l);

    // !! TODO:!!!!
    // if (_wval_str_p !== 0n)
    c_free(_wval_str_p);
    _wval_str_p = c_malloc_str(msg, box_l); // !! TODO:!!!!

    // secret_box_p = c_malloc(box_l);
    mem_set_u32($wval, box_l);
    const status = crypto_box_enc(_wval_str_p,
        _wval_str_p, msg_l,
        secret_p, 0);
    // TODO:
    // if (status == -1)
        // return null
    mem_set_u64($wval_data, _wval_str_p); // ->val -->
    // mem_set_u64($key + 8n, secret_box_p); // ->val -->
    mem_set_u8($wval_type, STRING);
}
function secure_set_buf({buffer,byteLength,byteOffset}) {
    mem_set_u8($wval_type, BUFFER);
    _secure_set_buf(buffer, byteLength, byteOffset)
}
function secure_set_json(val) {
    mem_set_u8($wval_type, JSON); // ->type // TODO: dedup
    const {buffer, byteLength, byteOffset} = v8.serialize(val)
    _secure_set_buf(buffer, byteLength, byteOffset)
}
function _secure_set_buf(buffer, byteLength, byteOffset) {
    const box_l = box_size(byteLength);
    const msg_p = arrbuf_ref(buffer) + BigInt(byteOffset);

    c_free(_wval_str_p);
    _wval_str_p = c_malloc(box_l);

    mem_set_u32($wval, box_l);
    mem_set_u64($wval_data, _wval_str_p); // ->val -->
    const status = crypto_box_enc(_wval_str_p,
        msg_p, byteLength,
        secret_p, 0);
}
function secure_get() {
    const box_l = _ems_sizeof($rval);
    return (_secure_decrypt(box_l)) ?
        GET_UNSAFE_BY_TYPE_IDX[ _ems_typeof($rval) ]( msg_size(box_l) )
        : null
    ;
}
function secure_get_str() {
    const box_l = _ems_sizeof($rval);
    const status = _secure_decrypt(box_l);
    let val= (status) ? get_str_unsafe(msg_size(box_l)) : null;
    return val
}
function secure_get_buf() {
    const box_l = _ems_sizeof($rval);
    return (_secure_decrypt(box_l)) ? get_buf_unsafe(msg_size(box_l)) : null;
}
function secure_get_json() {
    const box_l = _ems_sizeof($rval);
    return (_secure_decrypt(box_l)) ? get_json_unsafe(msg_size(box_l)) : null;
}
function _secure_decrypt(box_l) {
    return (box_l > hydro_secretbox_HEADERBYTES) &&
        (
            -1 !== crypto_box_dec(_STR_PTR,
                _STR_PTR, box_l,
                secret_p, 0)
        )
    ;
}

const   xidmap = c.map_new();
const   xstack = new Uint16Array(32);
let     xstack_l = 0;

function xstack_length() {return xstack_l;}
function xids_length() {return xidmap.size;}
function _xids_set(h32,xid) {return xidmap.set(h32, xid);}

// NOTE: existence happens on c-side...  beware renderer proc reuse

function mkx(path, max_len, max_sz, flags) {
    const path_l = path.length
    const path_p = c_malloc_str(path, path_l+1);
    const path_h32 = xxhash32(path_p, path_l, 0);
    const xid = xidmap.has(path_h32)
        ?   xidmap.get(path_h32)
        :   _xids_set(path_h32,
                ems_create(max_len, max_sz, path_p, flags|PERSIST|USE_MAP) )
    ;
    c_free(path_p);
    return xid;
}
function mkxa(path, max_len, max_sz, flags) {
    const path_l = path.length
    const path_p = c_malloc_str(path, path_l+1);
    const path_h32 = xxhash32(path_p, path_l, 0);
    const xid = xidmap.has(path_h32)
        ?   xidmap.get(path_h32)
        :   _xids_set(path_h32, ems_create(max_len, max_sz, path_p, flags|PERSIST))
    c_free(path_p);
    return xid;
}
function openx(path) {
    const path_l = path.length
    const path_p = c_malloc_str(path, path_l+1);
    const path_h32 = xxhash32(path_p, path_l, 0);
    const xid = xidmap.has(path_h32)
        ?   xidmap.get(path_h32)
        :   _xids_set(path_h32, ems_open(path_p))
    ;
    c_free(path_p);
    return xid;
}
function cwx()      { return get_xid() }
function cx(xid)    { return set_xid(xid) }
function pushx(xid) { return set_xid(xstack[xstack_l++] = xid) }
function popx(xid) {
    if (xstack_l === 0) return null
    const popped = xstack[--xstack_l]
    set_xid(xstack[xstack_l - 1]);
    return popped
}
function create(
    thread_id, thread_count,
    max_len, max_sz,
    path, flags)
{
    const path_p = c_malloc_str(path, path.length+1);
    const xid = ems_create(max_len, max_sz, path_p, flags);
    c_free(path_p);
    return xid;
}
function map_create(max_len, max_sz, filepath, flags) {
    return ems_create(max_len, max_sz, filepath, flags)
}   // c.call0($map_create);

function destroy(id, do_unlink){ return ems_destroy(id, do_unlink); }
function teardown(){ c.jit_free(cdef); }

function ASSERT_TYPE(type_idx) {
    if (_ems_typeof($rval) !== type_idx)
        throw new Error('Expecting')
}

const d = module.exports = (function d() {}).prototype;

d.REOPEN            = REOPEN

d.USE_MAP           = USE_MAP;
d.PERSIST           = PERSIST;
d.CLEAR             = CLEAR;
d.FILL              = FILL;
d.FILL_JSON         = FILL_JSON;
d.SET_E             = SET_E;
d.SET_F             = SET_F;
d.PIN_THREADS       = PIN_THREADS;
d.MLOCK_PCT_MASK    = MLOCK_PCT_MASK;

d.PUT    = function () {c.call0($write); };
d.PUT_XX = function (){ c.call0($writeXX); };
d.PUT_XF = function (){ c.call0($writeXF); };
d.PUT_XE = function (){ c.call0($writeXE); };
d.PUT_EF = function (){ c.call0($writeEF); };
d.PUT_FF = function (){ c.call0($writeFF); };
d.REQ    = function (){ c.call0($read); };
d.REQ_XX = function (){ c.call0($readXX); };
d.REQ_FE = function (){ c.call0($readFE); };
d.REQ_FF = function (){ c.call0($readFF); };
d.REQ_EF = function (){ c.call0($readEF); };
d.TOP    = function (){ c.call0($stk_get_top); };
d.PUSH   = function (){ c.call0($stk_produce); };
d.POP    = function (){ c.call0($stk_consume); };
d.ENQ    = function (){ c.call0($q_produce); };
d.DEQ    = function (){ c.call0($q_consume); };

d.REQ_RW = function (){ c.call0($readRW); };
d.RELEASE_RW = function (){ c.call0($releaseRW); };


// function SET_FF (){ return $_BEGIN($writeFF, 0); }
// d.hash31              = hash31;
d.hash32              = hash32;
d.hash64              = hash64;
d.secret_hash32       = secret_hash32;
d.secret_hash64       = secret_hash64;
d.get_status          = get_status;

d.mkx                 = mkx;
d.mkxa                = mkxa;
d.openx               = openx;
d.pushx               = pushx;
d.popx                = popx;
d.cwx                 = cwx;
d.cx                  = cx;

d.create              = create;
d.map_create          = map_create;
d.xids_length         = xids_length;
d.xstack_length       = xstack_length;

d.get_timeout         = get_timeout;
d.set_timeout         = set_timeout;
d.ms_to_ns            = ms_to_ns;

d.secret_set_phrase   = secret_set_phrase;
d.secret_get_hex      = secret_get_hex;
d.secret_get_ptr      = secret_get_ptr;

d.key                 = key;
d.key_str             = key_str;
d.key_i32             = key_i32;
d.key_i64             = key_i64;
d.key_json            = key_json;
d.key_fn              = key_fn;
d.key_undefined       = key_undefined;

d.set                 = set;
d.set_str             = set_str;
d.set_i32             = set_i32;
d.set_i64             = set_i64;
d.set_buf             = set_buf;
d.set_json            = set_json;
d.set_fn              = set_fn;
d.set_undefined       = set_undefined;

d.get                 = get;
d.get_str             = get_str;
d.get_i32_unsafe      = get_i32_unsafe;
d.get_i64_unsafe      = get_i64_unsafe;
d.get_i32             = get_i32;
d.get_i64             = get_i64;
d.get_buf             = get_buf;
d.get_json            = get_json;
d.get_fn              = get_fn;
// d.get_undefined       = get_undefined;

d.secure_key          = secure_key;
d.secure_key_str      = secure_key_str;

d.secure_set          = secure_set;
d.secure_set_str      = secure_set_str;
d.secure_set_buf      = secure_set_buf;
d.secure_set_json     = secure_set_json;

d.secure_get          = secure_get;
d.secure_get_str      = secure_get_str;
d.secure_get_buf      = secure_get_buf;
d.secure_get_json     = secure_get_json;

d.stk_get_top = function () {c.call0($stk_get_top); return get_status();}

d.writeXX     = function (k,v){ d.key(k);d.set(v);d.PUT_XX(); };
d.writeXF     = function (k,v){ d.key(k);d.set(v);d.PUT_XF(); };
d.writeXE     = function (k,v){ d.key(k);d.set(v);d.PUT_XE(); };
d.writeEF     = function (k,v){ d.key(k);d.set(v);d.PUT_XF(); };
d.write       = function (k,v){ d.key(k);d.set(v);d.PUT(); };
d.readRW      = function (k)  { d.key(k);d.REQ_RW();return d.get(); };
d.readXX      = function (k)  { d.key(k);d.REQ_XX();return d.get(); };
d.readFE      = function (k)  { d.key(k);d.REQ_FE();return d.get(); };
d.readFF      = function (k)  { d.key(k);d.REQ_FF();return d.get(); };
d.read        = function (k)  { d.key(k);d.REQ();   return d.get(); };
d.stk_produce = function (v)  { d.set(v);d.PUSH();return d.stk_get_top(); }
d.stk_consume = function ()   { d.POP();return d.get(); }
d.q_produce   = function (v)  { d.set(v);d.PUSH();return d.stk_get_top(); }
d.q_consume   = function ()   { d.DEQ();return d.get(); }

d.secret_write = function(key, msg) {
    d.secure_key(key);
    d.secure_set(msg);
    d.PUT();
}
d.secret_read = function (key) {
    d.secure_key(key);
    d.REQ();
    return d.secure_get();
}