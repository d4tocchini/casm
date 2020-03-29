
const RUN_SIZE      = 10000;
const WARMUP_SIZE   = 1000;
const WARMUP_LOOPS  = 100;

let size = WARMUP_SIZE;

function warmup(TESTS) {
    console.log("... warmup ...")
    for (const test of TESTS) {
        if (test.name[0] === '_') continue
        for (let i = 0; i < WARMUP_LOOPS; ++i) test();
    }
    size = RUN_SIZE;
}

function run(TESTS) {
    for (const test of TESTS) {
        const name = test.name;
        if (name[0] === '_') {
            test()
            continue
        }
        console.time(name);
        test();
        console.timeEnd(name);
    }
}

function _() {
    console.log("----------")
}


// TypedArray vs Dataview
// https://benediktmeurer.de/2018/09/18/improving-dataview-performance-in-v8/

// Map/Set Iteration
// https://benediktmeurer.de/2017/07/14/faster-collection-iterators/

const v8 = require('v8')

class LinkedList {
    constructor() {
        this.key_map = new Map()
        // this.keys = [];
        this.values = [];
        this.size = 0;
    }
    has(k) {
        return this.key_map.has(k)

        // return this.keys.includes(k);
    }
    indexOf(k) {
        return this.key_map.get(k)
        // return this.keys.indexOf(k);
    }
    push(k, v) {
        this.key_map.set(k, this.size++);
        this.values.push(v)

        // this.values[this.size++] = v;
        // this.keys[this.size] = k;
    }
    add(k, v) {
        return (this.key_map.has(k))
            ? this.set(k, v)
            : this.push(k, v)

        // const i = this.indexOf(k); return i>>>31 ? this.push(k, v) : this.set(i,k,v);
    }
    set(k, v) {
        const i = 0|this.key_map.get(k)
        // this.keys[i] = k;
        this.values[i] = v;
    }
    // get(k) {const i = this.indexOf(k); return (i>>>31) ? undefined : this.values[i];}
    get(k) {
        return this.values[this.key_map.get(k)];

        // return this.values[this.indexOf(k)];
    }
    // forEach(f) {
    //     const l = this.size;
    //     let i = 0
    //     while (i < l) {
    //         f(this.values[i], this.keys[i], i++);
    //     }
    // }
    forEach(f) {
        for (const [k, i] of this.key_map) {
            f(this.values[i], k, i)
        }
    }
}




let s;
let m
let a;
let t
let l;

let mom
let mos
let m64
let m64b
let mxor
let mxor16
const m64b_a32 = new Uint32Array(2)
const m64b_a64 = new BigInt64Array(m64b_a32.buffer)
let pool = []
let pool_l = 0
let mool = new Map()
let mool_l = 0
function Iterate(){}
function SetIterate(){}

let ser = new v8.Serializer();
let serbuf
const TESTS = [
    _,
    function ser_write() {
        ser.writeUint32(size)
        for (let i = 0; i < size; ++i) {
            ser.writeUint32(i)
        }
        serbuf = ser.releaseBuffer();
    },
    function map_write() {
        m = new Map();
        for (let i = 0; i < size; ++i) {
            m.set(i, i);
        }
    },
    function map_write_hasnt() {
        m = new Map();
        for (let i = 0; i < size; ++i) {
            if (m.has(i))
                continue
            m.set(i,i)
        }
    },
    function set_write() {
        s = new Set();
        for (let i = 0; i < size; ++i) {
            s.add(i);
        }
    },
    function set_write_hasnt() {
        s = new Set();
        for (let i = 0; i < size; ++i) {
            if (s.has(i))
                continue
            s.add(i)
        }
    },
    function array_write() {
        a = []
        for (let i = 0; i < size; ++i) {
            a[i] = i
        }
    },
    function array_write_preallocate() {
        a = new Array(size)
        for (let i = 0; i < size; ++i) {
            a[i] = i
        }
    },
    function array_write_preallocate_while() {
        a = new Array(size)
        let i = 0;
        while (i<size) {
            a[i] = i++
        }
    },
    function array_write_preallocate_while_unroll8() {
        a = new Array(size)
        let i = 0;
        while (i+8<size) {
            a[i] = i++;a[i] = i++;
            a[i] = i++;a[i] = i++;
            a[i] = i++;a[i] = i++;
            a[i] = i++;a[i] = i++;
        }
        while (i<size)
            a[i] = i++
    },
    function array_write_preallocate_negwhile() {
        a = new Array(size)
        let l = size;
        while (l--) {
            a[l] = l;
        }
    },
    function array_write_preallocate_negwhile_unroll8() {
        a = new Array(size)
        let l = size;
        while (l - 8 > 0) {
            a[--l] = l;
            a[--l] = l;
            a[--l] = l;
            a[--l] = l;

            a[--l] = l;
            a[--l] = l;
            a[--l] = l;
            a[--l] = l;
        }
        while (l--) {
            a[l] = l;
        }
    },
    function array_write_push() {
        a = []
        for (let i = 0; i < size; ++i) {
            a.push(i)
        }
    },
    function array_write_if_excludes() {
        a = []
        for (let i = 0; i < size; ++i) {
            if (!a.includes(i))
                a[i] = i
        }
    },
    function array_add_if_neq() {
        a = []
        for (let i = 0; i < size; ++i) {
            if (a[i] !== i)
                a[i] = i
        }
    },
    function u32arr_write() {
        t = new Uint32Array(size);
        for (let i = 0; i < size; ++i) {
            t[i] = i;
        }
    },
    function linked_write_push() {
        l = new LinkedList();
        for (let i = 0; i < size; ++i) {
            l.push(i, i)
        }
    },
    function linked_write_add() {
        l = new LinkedList();
        for (let i = 0; i < size; ++i) {
            l.add(i, i)
        }
    },


    function array_mixed_add() {
        const arr = [];
        for (let i = 0; i < size; ++i) {
            if (!s.has(i))
                continue
            arr[i] = i
        }
    },

    _, // --------------------------------

    function map_has() {
        for (let i = 0; i < size; ++i) {
            m.has(i)
        }
    },
    function set_has() {
        for (let i = 0; i < size; ++i) {
            s.has(i)
        }
    },
    function array_includes() {
        for (let i = 0; i < size; ++i) {
            a.includes(i)
        }
    },
    function u32arr_includes() {
        for (let i = 0; i < size; ++i) {
            t.includes(i)
        }
    },
    function u32arr_indexOf() {
        for (let i = 0; i < size; ++i) {
            t.indexOf(i)
        }
    },
    function u32arr_nested_equals() {
        for (let i = 0; i < size; ++i) {
            for (let j = 0; i < size; ++j) {
                if (t[j] === i)
                    break
            }
        }
    },
    function linked_has() {
        for (let i = 0; i < size; ++i) {
            l.has(i)
        }
    },

    _, // --------------------------------
    function des_read_all() {
        const des = new v8.Deserializer(serbuf);
        let l = des.readUint32()
        let v = 0
        while (l--) {
            v = des.readUint32()
        }
    },
    function u32arr_forof() {
        let v = 0
        for (const x of t) {v = x}
    },
    function u32arr_while() {
        const l = t.length
        let i = 0;
        let v = 0
        while (i < l) {
            v = t[i++];
        }
    },
    function u32arr_forEach() {
        let v = 0;
        t.forEach(function(x){v=x})
    },
    function u32arr_forEach_fn() {
        t.forEach(Iterate)
    },
    function array_forof() {
        let v = 0;
        for (const x of a) {v = x}
    },
    function array_forEach() {
        a.forEach(function(){})
    },
    function array_forEach_fn() {
        a.forEach(Iterate)
    },
    function array_while() {
        const l = a.length
        let i = 0;
        let v = 0;
        while (i < l) {
            v = a[i++];
        }
    },
    function array_for() {
        const l = a.length        ;
        let v = 0;
        for (let i = 0; i < l; i++) {
            v = a[i];
        }
    },
    function linked_forEach() {
        l.forEach(function(v,k,i){})
    },
    function set_forof() {
        let v = 0;
        for (const x of s) {v = x;}
    },
    function set_while() {
        const keys = s.values()
        while (true) {
            const x = keys.next()
            if (x.done) break
        }
    },
    function set_to_arr_while() {
        const a = [...s]
        const l = a.length
        let i = 0;
        while (i < l) {
            const x = a[i];
            ++i
        }
    },
    function set_forof_entries() {
        for (const x of s.entries()) {}
    },
    function set_forEach() {
        s.forEach(function(key, key, set) {});
    },
    function set_forEach_fn() {
        s.forEach(SetIterate);
    },
    function map_forof() {
        for (const x of m) {}
    },
    function MapForOfKeys() {
        for (const x of m.keys()) {}
    },
    function MapForOfValues() {
        for (const x of m.values()) {}
    },

    function MapForEach() {
        let v
        m.forEach(function(val, key, map) {
            v = val
        });
    },
    function MapWhile() {
        const l = m.size
        let i = 0;
        while (i < l) {
            const x = m.get(i);
            ++i
        }
    },

    _, // --------------------------------

    function ArrayGet() {
        let i = 0;
        while (i < size) {
            const x = a[i++];
        }
    },
    function MapGet() {
        let i = 0;
        while (i < size) {
            const x = m.get(i++);
        }
    },

    _, // --------------------------------

    function map_of_map__create() {
        mom = new Map()
        for (let i = 0; i < (size>>4); ++i) {
            const sub = new Map()
            mom.set(i, sub);
            for (let j = 0; j < (size>>2); ++j) {
                sub.set(j, j);
            }
        }
    },
    function map_of_map__pool_populate() {
        pool = [];
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                pool_l = pool.push(new Map())
            }
        }
    },
    function map_of_map__mool_populate() {
        mool = new Map();
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                mool.set(mool_l++, new Map())
            }
        }
    },
    function map_of_map__create_pool() {
        mom = new Map()
        for (let i = 0; i < (size>>4); ++i) {
            const sub = pool[--pool_l]
            mom.set(i, sub);
            for (let j = 0; j < (size>>2); ++j) {
                sub.set(j, j);
            }
        }
    },
    function map_of_map__create_mool() {
        mom = new Map()
        for (let i = 0; i < (size>>4); ++i) {
            const sub = mool.get(--mool_l);
            mom.set(i, sub);
            for (let j = 0; j < (size>>2); ++j) {
                sub.set(j, j);
            }
        }
    },
    function map_of_64__create() {
        m64 = new Map()
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                m64.set(BigInt(i, j), j);
            }
        }
    },
    function map_of_str__create() {
        mos = new Map()
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                mos.set(`${i} ${j}`, j);
            }
        }
    },
    // function map_of_64b__create() {
    //     m64b = new Map()
    //     for (let i = 0; i < (size>>4); ++i) {
    //         m64b_a32[0] = i;
    //         for (let j = 0; j < (size>>2); ++j) {
    //             m64b_a32[1] = j;
    //             m64b.set(m64b_a64[0], j);
    //         }
    //     }
    // },
    function map_of_xor__create() {
        mxor = new Map()
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                mxor.set(i^j, j);
            }
        }
    },
    function map_of_xor16__create() {
        mxor16 = new Map()
        for (let i = 0; i < (size>>4); ++i) {
            const x = to_x16(i);
            for (let j = 0; j < (size>>2); ++j) {
                mxor16.set(x|to_y16(j), j);
            }
        }
    },
    _,
    function map_of_map__has() {
        let v = false;
        for (let i = 0; i < (size>>4); ++i) {
            if (mom.has(i)) {
                const sub = mom.get(i)
                for (let j = 0; j < (size>>2); ++j) {
                    v = sub.has(j);
                }
            }
        }
    },
    function map_of_64__has() {
        let v = false
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                v = m64.has(BigInt(i, j));
            }
        }
    },
    function map_of_xor__has() {
        let v = false
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                v = mxor.has(i^j);
            }
        }
    },
    function map_of_xor16__has() {
        let v = false
        for (let i = 0; i < (size>>4); ++i) {
            const x = to_x16(i);
            for (let j = 0; j < (size>>2); ++j) {
                v = mxor16.has(x|to_y16(j));
            }
        }
    },
    _,
    function map_of_map__get() {
        let v = 0;
        for (let i = 0; i < (size>>4); ++i) {
            if (mom.has(i)) {
                const sub = mom.get(i)
                for (let j = 0; j < (size>>2); ++j) {
                    v = sub.get(j);
                }
            }
        }
    },
    function map_of_64__get() {
        let v = 0
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                v = m64.get(BigInt(i, j));
            }
        }
    },
    function map_of_xor__get() {
        let v = 0
        for (let i = 0; i < (size>>4); ++i) {
            for (let j = 0; j < (size>>2); ++j) {
                v = mxor.get(i ^ j);
            }
        }
    },
    function map_of_xor16__get() {
        let v = 0
        for (let i = 0; i < (size>>4); ++i) {
            const x = to_x16(i);
            for (let j = 0; j < (size>>2); ++j) {
                v = mxor16.get(x|to_y16(j));
            }
        }
    },

];



warmup(TESTS);
run(TESTS);


(function _map_of_xor__create_validate() {
    const map = new Map()
    const size = 20000
    for (let i = 0; i < (size>>4); ++i) {
        const a = to_x16(i);
        for (let j = 0; j < (size>>2); ++j) {
            const b = to_y16(j);
            const k =  a | b;
            if (map.has(k)) {
                console.error("map_of_xor COLLISION "+ map.get(k) + ' - ' + i + ',' + j )
                return
            }
            map.set(k, i+','+j)
        }
    }
})()

function to_x16(i) {
    return to_y16(i) << 16
}
function to_y16(j) {
    return (j >> 16) ^ (j & 65535)
}


(function _map_of_xor__create_validate() {
    const map = new Map()
    const size = 1000
    for (let i = 0; i < (size>>4); ++i) {
        const a = ((i >> 16) ^ (i & 65535)) << 16;
        for (let j = 0; j < (size>>2); ++j) {
            const b = (j >> 16) ^ (j & 65535);
            const k =  a | b;
            if (map.has(k))
                console.error("map_of_xor COLLISION "+ map.get(k) + ' - ' + i + ',' + j )
            map.set(k, i+','+j)
        }
    }
	console.log(map.size)
})()