
/*

ns
|--db
    |--tbl
    |   |--col
    |------ent

ns map-of-dbs
    db map-of-tbls
        tbl map-of-cols
            col map-of-vals
                val
w   x   y   z   .

get(w,x,t,z)
    get(x,y,z)
        get(y,z)
            get(z)

d4 = new map4d("db", "tbl", "col", "ent")

d4
.db("app")
.tbl("layout")
.cols(["x","y","z"])

const root = new Map()

// db
app = som(root)
    .touch('main.win')

// tbl
sbar = som(app)
    .touch('sbar')

// col
som(sbar)
.touch('')



*/

const v8 = require('v8');
const d = casm.d;
const hash = d.hash32;

const dd = module.exports = (function dd() {}).prototype

const EMPTY_MAP = new Map()
const EMPTY_BYTES = new Uint8Array(0);
const NOOP = function () {}

const MUTATED   = (1<<27);
const REMOVED   = (1<<28) | MUTATED;
const ADDED     = (1<<29) | MUTATED;
const UPDATED   = (1<<30) | MUTATED;

// const {DEVICE_ID, DEV_MODE} = process.env;

dd.PERSIST = d.PERSIST;
dd.CLEAR = d.CLEAR;

dd.open = open;
dd.touch = touch;

dd.get_ns = get_ns;
dd.get_dir = get_dir
dd.get_path = get_path;
dd.get_native_handle = get_native_handle;

dd.set_dir = set_dir;

let _ns = ''
let _dir = ''
let _path = ''
let _handle;
let _setup = function _setup_default() {open('dat');};

function open(ns) {
    if (_set_ns(ns)) _open( _path );
    return dd;
}
function touch(ns, flags) {
    if (_set_ns(ns)) _touch(_path, flags|0);
    return dd;
}

function get_ns() {return _ns;}
function get_dir() {return _dir ? _dir : _dir = "/tmp";}
function get_path(path) { return _path; };
function get_native_handle() {return _handle;}

function set_dir(dirname) {
    _dir = dirname;
    return dd;
}

function _set_ns(name) {
    const _new_path = _ns_to_path(name);
    if (_new_path === _path)
        return false
    _ns = name
    _path = _new_path
    return true
}
function _ns_to_path(name) {return get_dir() + '/' + name + '.dd'}
function _did_setup() {_setup = NOOP;}
function _open(path) {
    _did_setup();
    _handle = d.openx(path);
}
function _touch(path, flags) {
    _did_setup();
    const capacity = 1 << 12
    const size = 1 << 30
    _handle = d.mkx(path, capacity, size, flags)
    return dd;
}

// =============================================================================
// ENT

dd.ent = ent;
function ent(str) {return hash(str);}
// function ent_to_str(e) {return }


// ============================================================================
// TIME

const utc_offset = BigInt( (new Date()).getTimezoneOffset() * 60000 )
function now_utc64() { return BigInt(Date.now()) + utc_offset; }
function tick() {
    if (live_wids_l > 0)
        _tick()
}
function _tick() {
    txn(function ($) {
        $.key_i32(0);
        $.REQ()
    })
    // for (let wid of live_wids)
}



// =============================================================================
// DB
let _db;
const _dbs = new Map()

dd.DB = DB
dd.db_touch = db_touch
dd.db_alloc = db_alloc
dd.db_has = db_has
dd.db_get = db_get
dd.db_load = db_load
dd.db_commit = db_commit

function DB(name) { db_touch( ent(name) ); }
function db_touch(d) {return db_has(d) ? db_get(d) : db_alloc(d);}
function db_alloc(d) {_dbs.set(d, _db = _db_new(d)); return _db;}
function db_has(d) {return _dbs.has(d);}
function db_get(d) {return _db = _dbs.get(d);}
function db_load(d) {}
function db_commit(d) {}

function _db_new(id) {
    _setup();
    return new Map();
    // return new DB(id);
}

/*
dd.db_has = DBS.has_col;

const DBS = SO(function(k){
    return TBLS.touch(k)
})
const TBLS = SO(function (k, map) {
    return COMPS.touch(k)
})
const COMPS = SO(function(k, map) {
    COMP_DELTAS = SO(NOOP)
    COMP_CHS = SO(NOOP)
    COMP_DATS = SO(NOOP)
})

function SOM (_new_) {
    let cols
    let _col

    function __(map) {return _col = map;}

    function has_col(k) {return cols.has(k);}
    function get_col(k) {return __(cols.get(k));}
    function set_col(k, v) {cols.set(k, v);}
    function del_col(k) {return cols.delete(k);}

    function touch(k) {return has(k) ? get(k) : alloc(k);}
    function alloc(k) {
        return __( _new_( k ) );
    }

    function has(x,y) {
        return has_col(x) && (get_col(x)&&col_has(y))
    }
    function get(x,y) {
        return get_col(x) ? col_get(y) : undefined;
    }
    function del(x,y) {
        return get_col(x) && col_del(y)
    }
    function set(x,y,v) {
        get_col(x) && col_set(y, v);
    }

    function col_has(y) {return _col.has(y);}
    function col_get(y) {return _col.get(y);}
    function col_del(y) {return _col.delete(y);}
    function col_set(y, v) {_col.set(y, v);}

    function has2(x,y) {return has(x) && get(x).has(y);}

    function ctor() {}
    const p = ctor.prototype;


    p.has = has;
    p.get = get;
    p.set = set;
    p.del = del;

    p.HAS = hook_arg_1_1(has, ent);
    p.GET = hook_arg_1_1(get, ent);
    p.SET = hook_arg_1_2(set, ent);
    p.DEL = hook_arg_1_1(del, ent);

    p.TOUCH = hook_arg_1_1(touch, ent);

    const self = new ctor();

    return function(target) {
        map = target;
        return self;
    }
}
*/

// function DB(id) {
//     const map = new Map()
//     this.tbl = curry_i__i(d, tbl_touch);
//     this.has = map_curry_has(map);
//     this.get = map_curry_get(map);
//     this.set = map_curry_set(map);
//     this.delete = map_curry_delete(map);

//     function create(k) {

//     }
//     function alloc(k) {
//         set(k, create(k));
//     }
//     function touch (k) {
//         return has(k) ? get(k) : alloc(k);
//     }
// }



// ============================================================================
// TABLE

let _t;
let _tbl_allocs = 0

dd.TBL = TBL;
dd.tbl = tbl;
dd.tbl_alloc = tbl_alloc;
dd.tbl_has = tbl_has;
dd.tbl_get = tbl_get;
dd.tbl_alloc_count = tbl_alloc_count

function TBL(d_name, t_name) {return tbl(ent(d_name), ent(t_name));}
function tbl(d, t) {return tbl_has(d, t) ?_db.get(t) :tbl_alloc(d, t) ;}
function tbl_alloc(d, t) {db_touch(d).set(t, _t = _tbl_new(t)); return _t;}
function tbl_has(d, t) {return db_has(d) && (db_get(d).has(t));}
function tbl_get(d, t) {return db_has(d) ? db_get(d).get(t) : undefined;}
function tbl_alloc_count() {return _tbl_allocs}

function _tbl_free(d, t) {
    return (tbl_has(d, t)) ?_db.delete(t) :false ;
}
function _tbl_new(t) {
    const tbl = new Table(t)
    const parent = _db
    tbl._did_free = function() {
        parent.delete(t)
    }
    return tbl
}

// const wgen_key_prefix = 1n << 50n;
const $tbl = Table.prototype;

$tbl.ent = ent;

function Table(id) {
    ++_tbl_allocs;
    const self = this
    this.id = id;
    const wgen_key = BigInt(id) << 32n;
    const wheader_key = wgen_key << 1n;
    const wbody_key = wgen_key << 2n

    // const timestamp_key = Bigint(wid) | (1n<<33n)
    // let timestamp = 0n
    // const ent_set = new Set();
    // const time_stamps = []
    // const ent_comps = new Map();

    let tbl_flags = 0;
    let curr_gen = 0;
    let next_gen = 0;
    let prev_gen = 0;
    let dns = new Map();
    let chs = new Map();
    let deltas = new Map();

    const comps_gen = new Map();
    const comps_delta_min = new Map()
    const comps_state = new Map();
    let   comps_buf = new Map()

    let _comp = EMPTY_MAP;

    let _header_buf = EMPTY_BYTES
    let _body_buf = EMPTY_BYTES
    let _needs_load_t = 0
    let _needs_merge_t = 0
    let _needs_load_l = 0
    let _needs_merge_l = 0
    let _needs_load = []
    let _needs_merge = []

    const _on_set_fns = new Map()
    let _on_set_canceled
    function on_set(c, fn) {
        if (_on_set_fns.has(c))
            _on_set_fns.get(c).add(fn);
        else
            _on_set_fns.set(c, new Set([fn]));
    }
    function off_set(c, fn) {
        if (_on_set_fns.has(c))
            _on_set_fns.get(c).delete()
    }
    function _on_set(c, e, v, prev) {
        _on_set_canceled = false;
        return _on_set_fns.has(c)
            ? __on_set_emit(c, e, v, prev) : v
        ;
    }
    function __on_set_emit(c, e, v, prev) {
        const fnset = _on_set_fns.get(c)
        for (let fn of fnset) {
            v = fn(v, e, c, prev, _on_set_cancel);
            if (_on_set_canceled === false)
                break
        }
        return v;
    }
    function _on_set_cancel() {
        _on_set_canceled = true
    }

    function free() {
        tbl_flags = 0;
        curr_gen = 0;
        next_gen = 0;
        prev_gen = 0;
        dns.clear();
        chs.clear();
        deltas.clear()
        comps_gen.clear()
        comps_delta_min.clear()
        comps_state.clear()
        comps_buf.clear()
        _comp = EMPTY_MAP;
        _header_buf = EMPTY_BYTES
        _body_buf = EMPTY_BYTES
        _needs_load = []
        _needs_merge = []

        _on_set_fns.clear();

        self._did_free()
    }
    this._did_free = NOOP

    this.comp = comp;
    this.col = col;
    this.cols = cols;
    this.comp_is_hidden = comp_is_hidden;
    this.comp_gen_get = function(c) {return 0|(comps_gen.get(c));}
    this.del = del;
    this.free = free;

    this.has = function(c, e) {dn_has(c, e);}
    // this.comp_count = function() {return dns.size}
    // this.updated_comp_count = function() {return ups.size}
    this.getter = getter;
    this.setter = setter;
    this.get = getter();
    this.get_i32 = getter();
    this.get_i64 = getter();
    this.get_str = getter();
    this.get_arr = getter();
    this.get_map = getter();
    this.get_buf = getter();
    this.set = setter(false);
    this.set_i32_fast = setter(false);
    this.set_i32 = setter(true);
    this.set_i64 = setter(true);
    this.set_str = setter(true);
    this.set_arr = setter(false);
    this.set_map = setter(false);
    this.set_buf = setter(false);

    this.needs_commit = needs_commit;
    this.rectify = rectify;

    function needs_commit () {return (tbl_flags & MUTATED) && _still_has_chs();}
    function _still_has_chs() {
        const count = chs.size;
        return (count === 0) ? (tbl_flags = 0) : 1
    }
    function has_rms() {return tbl_flags & REMOVED;}
    function has_ups() {return tbl_flags & REMOVED;}


    function rectify() {
        try {
        if (needs_commit())
            txn(_txn_rectify_writer)
        else
            txn(_txn_rectify_reader)
        }
        catch(e) {
            console.log(e)
            // console.warn(e)
        }
    }
    function _did_rectify() {
        prev_gen = curr_gen;
        curr_gen = next_gen;
        tbl_flags = 0;
    }
    function _on_mutate(c, e, flag) {
        tbl_flags |= flag;
    }
    // function _on_access(c, e) {}

    function cols(list) {
        list.forEach(col);
        rectify();
        return self;
    }
    function col(c) {
        if (!comps_gen.has(c)) {
            comps_gen.set(c,0);
            comps_delta_min.set(c,0)
            comps_state.set(c,0);
        }
    }
    function comp(name) {
        const c = hash(name);
        col(c)
        return c
    }
    function comp_is_hidden(c) {
        return (!comps_state.has(c)) || (comps_state.get(c) >>> 31);
    }


    // TODO:
    // this.clear = function () {
    //     this.clear_saved();
    //     this.clear_updated();
    // }
    // this.clear_saved = function () {
    //     for (const map of dns.values())
    //         map.clear()
    // }
    // this.clear_updated = function () {
    //     for (const map of ups.values())
    //         map.clear()
    // }

    // this.comp_enable_prev_cache = function() {
    //     return _comp_state.get(c);
    // }

    function dn_put(c) {
        if (dns.has(c)) return dns.get(c);
        const map = new Map();
        dns.set(c, map);
        return map;
    }
    function ch_put(c) {
        if (chs.has(c)) return chs.get(c);
        const map = new Map();
        chs.set(c, map);
        return map;
    }
    function delta_put(c) {
        if (deltas.has(c)) return deltas.get(c);
        const map = new Map();
        deltas.set(c, map);
        return map;
    }

    this.dn_has = dn_has
    this.ch_has = ch_has
    this.ch_get = ch_get
    this.dn_size = dn_size

    function dn_has (c, e)  {return dns.has(c) && (_comp = dns.get(c)).has(e);}
    function ch_has (c, e)  {return chs.has(c) && (_comp = chs.get(c)).has(e);}
    function ch_get (c, e)  {return chs.has(c)  ? (_comp = chs.get(c)).get(e) : 0;}
    function dn_size (c, e)  {
        return dns.has(c) ? (_comp = dns.get(c)).size : 0;
    }

    function del(c, e) {
        if (dn_has(c, e)) {
            _comp.delete(e);
            ch_put(c).set(e, undefined);
            _on_mutate(c, e, REMOVED);
        }
    }   // _on_access(c, e)

    function getter() {
                // _on_access(c, e)
                //(up_has(c, e) || dn_has(c, e))
        return function(c, e) {
            return dn_has(c, e)
                ? _comp.get(e) : undefined;
            }   // TODO: default?
    }

    function setter(comparable) {
                // _on_access(c, e)
                    // ||(ch_has(c, e) && (v === (prev = _comp.get(e))))
        return comparable
            ? function(c, e, v) {
                let prev;
                const dn_map = dn_put(c);
                if  ((dn_map.has(e) && (v === (prev = dn_map.get(e))))
                ) return
                v = _on_set(c, e, v, prev);
                if (_on_set_canceled ) {
                    _on_set_canceled = false
                    return
                }
                dn_map.set(e,v);
                ch_put(c).set(e,v);
                _on_mutate(c, e, UPDATED)
            }
            : function(c, e, v) {
                const dn_map = dn_put(c);
                const prev = dn_map.has(e) ? dn_map.get(e) : undefined;
                v = _on_set(c, e, v, prev);
                if (_on_set_canceled) {
                    _on_set_canceled = false
                    return
                }
                dn_map.set(e,v);
                ch_put(c).set(e,v);
                _on_mutate(c, e, UPDATED)
            }
    }


    function _write_mutations() {
        const MAX_GEN_DELTA = 8;
        let needs_body = false
        for (const [c, ch_map] of chs) {
            if (comp_is_hidden(c))
                continue

            const c_next_gen = next_gen;
            const c_gen = comps_gen.get(c);

            if (c_gen === 0) {
                needs_body = true;
                __init_delta(c);
                comps_delta_min.set(c, c_next_gen)
                comps_gen.set(c, c_next_gen);
                if (deltas.has(c))
                    deltas.delete(c)
                continue
            }

            let delta_l = ch_map.size;

            let next_delta = ch_map;
            if (deltas.has(c)) {
                const delta_map = deltas.get(c);
                delta_l += delta_map.size

                // TODO: set max delta length per comp
                if (delta_l > 64) {
                    needs_body = true;
                    map_merge(delta_map, ch_map)
                    __commit_delta(c, delta_map);
                    comps_delta_min.set(c, c_next_gen)
                    comps_gen.set(c, c_next_gen);
                    deltas.delete(c)
                    continue
                }

                const c_delta_min = (0|comps_delta_min.get(c));
                if ((c_next_gen - c_delta_min) > MAX_GEN_DELTA) {
                    needs_body = true;
                    __commit_delta(c, delta_map);
                    comps_delta_min.set(c, c_gen)
                }
                else
                    map_merge(next_delta = delta_map, ch_map);
            }
            else {
                if (delta_l > 64) {
                    needs_body = true;
                    __commit_delta(c, ch_map);
                    comps_delta_min.set(c, c_next_gen)
                    comps_gen.set(c, c_next_gen);
                    continue
                }
                comps_delta_min.set(c, c_gen)
            }

            deltas.set(c, next_delta);
            comps_gen.set(c, c_next_gen);
        }
        chs.clear()

        _write_header()
        if (needs_body) _write_body()
    }
    function __init_delta(c) {
        const dn_map = dn_put(c);
        comps_buf.set(c, map_serialize(dn_map));
    }
    function __commit_delta(c, delta_map) {
        const dn_map = dn_put(c);
        map_merge_chs(dn_map, delta_map)
        comps_buf.set(c, map_serialize(dn_map));
    }
    function _read_body() {
        comps_buf = map_deserialize(
            _body_buf = _load_body(wbody_key)
        )
        return true
    }
    function _write_body() {
        // console.log("_write_body")
        _store_body(wbody_key, _body_buf = v8.serialize(comps_buf))
    }
    function _write_header() {
        // console.log("_write_header")
        const ser = serializer();
        ser.writeValue(deltas)
        ser.writeUint32(comps_gen.size)
        for (const [c, gen] of comps_gen) {
            ser.writeUint32(c);
            ser.writeUint32(gen);
            ser.writeUint32(comps_delta_min.get(c));
        }

        // console.log("_write_header",{deltas, comps_state})

        _store_header(wheader_key, _header_buf = ser.releaseBuffer())
    }
    function _read_header() {
        _header_buf = _load_header(wheader_key)
        if (!_header_buf)
            return false

        let should_load_body = false
        const des = deserializer(_header_buf);

        deltas = des.readValue();

        const _needs_commit = needs_commit()

        // console.log("_read_header",{_needs_commit,comps_state})

        let up_comps_l = des.read_u32();
        while (up_comps_l--) {
            const c = des.read_i32();
            const up_gen = des.read_u32();
            const up_delta_min = des.read_u32();

            // console.log(`comps_state.has(${c}) = ${comps_state.has(c)}`,comps_state)

            if (!comps_state.has(c)) {
                if (_needs_commit) should_load_body = true;
                comps_state.set(c, -1);
                comps_gen.set(c, 0);
                comps_delta_min.set(c, 0) // TODO:??
                continue;
            }

            const dn_gen = comps_gen.get(c);
            const dn_delta_min = comps_delta_min.get(c);

            // console.log({up_comps_l,c,dn_gen,up_gen, up_delta_min, dn_delta_min})

            if (up_gen === dn_gen) continue;

            const is_hidden = comp_is_hidden(c);
            const is_less_delta = dn_gen < up_delta_min;

            comps_delta_min.set(c, up_delta_min)

            if (_needs_commit && is_less_delta)
                should_load_body = true

            if (is_hidden) continue
            if (is_less_delta) {
                should_load_body = true
                _comp_needs_load(c)
            }
            if (dn_gen <= up_delta_min && deltas.has(c))
                _comp_needs_merge(c);

            comps_gen.set(c, up_gen);

            // const prev = dns.get(c); // TODO:
        }



        // console.log({deltas,should_load_body,_needs_merge,_needs_load})

        return should_load_body;
    }

    function _txn_rectify_reader() {
        next_gen = _r_lock(wgen_key);
        if (next_gen === curr_gen)
            _r_unlock_immediate()
        else {
            if (_read_header())
                _read_body()
            _r_unlock(wgen_key);
            _merge_if_needed()
            _did_rectify()
        }
    }

    function _txn_rectify_writer() {
        next_gen = _w_lock(wgen_key);
        if (next_gen === curr_gen) {
            next_gen++
            _write_mutations()
        }
        else {
            if (_read_header())
                _read_body()
            _merge_if_needed();
            if (needs_commit()) {
                next_gen++
                _write_mutations()
            }
        }
        _w_unlock(wgen_key, next_gen);
        _did_rectify()
    }


    function _comp_needs_load(c) {
        if (_needs_load_l === _needs_load_t)
            _needs_load_l = _needs_load_t = _needs_load.push(c)
        else {
            _needs_load[_needs_load_l++] = c
        }
    }
    function _comp_needs_merge(c) {
        if (_needs_merge_l === _needs_merge_t)
            _needs_merge_l = _needs_merge_t = _needs_merge.push(c)
        else {
            _needs_merge[_needs_merge_l++] = c
        }
    }
    function _merge_if_needed() {
        // console.log({_needs_load, _needs_merge, deltas})
        if (_needs_load_l > 0) _merge_load()
        if (_needs_merge_l > 0) _merge_deltas()
    }
    function _merge_load() {
        const comps = _needs_load;
        let l = _needs_load_l;
        _needs_load_l = 0;
        while (l--) {
            const c = comps[l];
            const next = map_deserialize(comps_buf.get(c));
            // ensure local changes
            if (chs.has(c)) {
                if (map_merge_chs_and_dedup(
                        next,
                        chs.get(c),
                        cmp_simple_equals)
                === 0)
                    chs.delete(c)
            }
            dns.set(c, next);
        }
    }
    function _merge_deltas() {
        const comps = _needs_merge;
        let l = _needs_merge_l
        _needs_merge_l = 0;
        while (l--) {
            const c = comps[l];
            if (!deltas.has(c))
                throw new Error("_comps_merge_upstream ?!?!? " + c +'  ' + (c|0) )
            const delta = deltas.get(c);
            if (chs.has(c)) {
                if (map_merge_and_dedup(delta, chs.get(c), cmp_simple_equals)
                === 0)
                    chs.delete(c)
            }
                // map_merge_chs_exclude( dn_put(c), deltas.get(c), chs.get(c) );
            // else
            map_merge_chs( dn_put(c), delta );
        }
        
    }

        // comps_buf_sz = des.readUint32();
        // comps_buf_pos = des.readValue();
        // comps_buf = des.readRawBytes(comps_buf_sz);
            // const idx = (changed[i++] << 1);
            // const beg = comps_buf_pos[idx];
            // const end = comps_buf_pos[idx + 1]
            // const comp_buf = comps_buf.subarray(beg, end);
            // const c = comps[i];
            // _comp_set_buf(c, comp_buf);
}


// ============================================================================
// CONCURRENT IO

function txn(fn) {
    txn_begin()
    fn(d)
    txn_end()
}
function txn_begin() {d.pushx(_handle);}
function txn_end() {d.popx();}

function _load_header(k) {
    d.key_i64(k);
    d.REQ_FF();
    let buf = d.get_buf()
    if (!buf) {d.key_i64(k); d.REQ_XX(); buf = d.get_buf()}
    if (!buf) {d.REQ_FF();buf = d.get_buf()}
    return buf;
}
function _load_body(k) {
    d.key_i64(k);
    d.REQ_FF();
    let buf = d.get_buf()
    if (!buf) {d.key_i64(k);d.REQ_XX(); buf = d.get_buf()}
    if (!buf) {d.REQ_FF();buf = d.get_buf()}
    // else
    //     return decompress(buf)
    return buf;
}
function _store_body(k, buf) {
    d.key_i64(k);
    d.set_buf(buf);//d.set_buf(compress(buf));
    d.PUT_FF();
}
function _store_header(k, buf) {
    d.key_i64(k);
    d.set_buf(buf);
    d.PUT_FF();
}

function _r_lock(k) {
    d.key_i64(k)
    // d.REQ_RW()
    d.REQ_FE()
    const v = 0|(d.get_i32());
    // console.log('_r_lock',{k,v})
    return v
}
function _r_unlock_immediate() {
    // d.RELEASE_RW()
    d.REQ_EF()
}
function _r_unlock(k) {
    d.key_i64(k)
    // d.RELEASE_RW()
    d.REQ_EF()
}
function _w_lock(k) {
    d.key_i64(k)
    d.REQ_FE()
    // d.REQ()
    const v = 0|(d.get_i32());
    // console.log('_w_lock',{k,v})
    return v;
}
function _w_unlock(k, v) {
    d.key_i64(k)
    d.set_i32(v)
    d.PUT_EF()
    // d.PUT()

    // d.REQ_FF()
    // console.log('_w_unlock',{k,v},d.get_i32())
    // return 0|(d.get_i32());
}

// ============================================================================
// DE/COMPRESS

function compress(b) {_COMPRESS_MODULE_(); return compress(b);}
function decompress(b) {_COMPRESS_MODULE_(); return decompress(b);}

function _COMPRESS_MODULE_() {
    const zlib = require('zlib');
    // const _compress_params =
    const _decompress_params = { params: {
        // [zlib.constants.BROTLI_PARAM_MODE]: zlib.constants.BROTLI_MODE_TEXT,
        [zlib.constants.BROTLI_PARAM_QUALITY]: 6,
        // [zlib.constants.BROTLI_PARAM_SIZE_HINT]: b.byteLength
    }}

    compress = function(b) {return zlib.brotliCompressSync(
        b, {
            params: {
                // [zlib.constants.BROTLI_PARAM_MODE]: zlib.constants.BROTLI_MODE_TEXT,
                [zlib.constants.BROTLI_PARAM_QUALITY]: 6,
                [zlib.constants.BROTLI_PARAM_SIZE_HINT]: b.byteLength
            }
        }
    );}
    decompress = function(b) {return zlib.brotliDecompressSync(b, _decompress_params);}
}

// ============================================================================
// DE/SERIALIZE


class Des extends v8.DefaultDeserializer {
    constructor(buf) {
        super(buf)
    }
}
const $$des = v8.DefaultDeserializer.prototype
const $des = Des.prototype;
$des.read_val = $$des.readValue
$des.read_u32 = $$des.readUint32;
$des.read_i32 = function() {return (0|this.readUint32());}

function serializer() {
    const ser = new v8.DefaultSerializer();
    ser.writeHeader()
    return ser;
}
function deserializer(buf) {
    if (!buf) debugger
    const des = new Des(buf);
    des.readHeader()
    return des;
}

const _sertmp = new v8.Serializer()

// ============================================================================
// Utils

function curry_i__i( i0, fn ) {return function( i1 ) {return fn(i0, i1);};}

function map_curry_has(map) {return function( k ) {return map.has(k);}}
function map_curry_get(map) {return function( k ) {return map.get(k);}}
function map_curry_set(map) {return function( k, v ) {map.set(k, v);}}
function map_curry_delete(map) {return function( k ) {return map.delete(k);}}


function map_serialize(map) {
    return v8.serialize(map)
    // _sertmp.writeHeader()
    // _sertmp.writeValue(map)
    // return _sertmp.releaseBuffer()
}
function map_deserialize(buf) {
    return v8.deserialize(buf)
    // console.log({buf})
    // return deserializer(buf).readValue()
}
function map_merge(out_map, in_map) {
    for (const [e,v] of in_map)
        out_map.set(e,v);
}
function map_merge_chs(out_map, ch_map) {
    for (const [e,v] of ch_map) {
        if (v !== undefined)
            out_map.set(e,v);
        else
            out_map.delete(e)
    }
}
function map_merge_and_dedup(out_map, ch_map, cmp_fn) {
    for (const [e,v] of ch_map) {
        if (out_map.has(e) && cmp_fn(out_map.get(e), v))
            ch_map.delete(e)
        else
            out_map.set(e,v);
    }
    return ch_map.size;
}
function map_merge_chs_and_dedup(out_map, ch_map, cmp_fn) {
    for (const [e,v] of ch_map) {
        if (v !== undefined) {
            if (out_map.has(e) && cmp_fn(out_map.get(e), v))
                ch_map.delete(e)
            else
                out_map.set(e,v);
        }
        else {
            if (out_map.has(e))
                out_map.delete(e)
            else
                ch_map.delete(e)
        }
    }
    return ch_map.size;
}
function map_merge_chs_exclude(out_map, ch_map, ex_map) {
    for (const [e,v] of ch_map) {
        if (ex_map.has(e))
            continue
        if (v !== undefined)
            out_map.set(e,v);
        else
            out_map.delete(e)
    }
}
function cmp_simple_equals(a, b) {
    return a === b
}




function MapEmitter() {
    const on_set = new Set();
    this.init_event = function(event) {
        let data = new Map()
        data.set("event", event);
        return data;
    }
    this.emit_event = function (data) {
        return (on_set.size > 0)
            ? _emit(data) : data;
    }
    this.end_event = function (data) {

    }
    function _emit(data) {
        for (const fn of on_set) {
            fn(this)
            if (!data.has("event")) break
        }
        return data
    }
    this.on_size = function() {
        return on_set.size
    }
    this.on = function (fn) {
        on_set.add(fn)
    }
    this.off = function (fn) {
        on_set.delete(fn)
    }
    this.clear = function() {
        on_set.clear();
    }
}
