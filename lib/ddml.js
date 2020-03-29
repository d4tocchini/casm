

/*

*/

const TAB_POW = 2;
const TIDX_SHIFT = 20;
const VIDX_MASK = (1 << TIDX_SHIFT) - 1;
const _keys = new Map();
const _vals = new Map();
const _tmpls = new Map();
const _tlist = [];

function def_primitive(tname, cb) {
    if (_typs.has(name))
        return // TODO:
    const tidx = _tlist.push(name) - 1;
    const tbit = tidx << TIDX_SHIFT;
    const keys = [];
    const vals = [];

    _keys.set(name, keys);
    _vals.set(name, vals);

    function key(k) {
        keys.push(k);
    }
    function val(v) {
        return tbit | (vals.push(v) - 1);
    }
    _tmpls.set(name, cb(key,val))
}

def_primitive('fn',function(key,val){
    return function ([k]) {
        key(k)
        return _setter;
    }
    function _setter(fn) {
        return val(fn);
    }
})

def_primitive('i32',function(key,val){
    return function ([k], v) {
        key(k)
        return val(v);
    }
})

function DDML(pads, , ...lines) {
    return tmpl48_parse(pads, lines, lines.length);
}

const TMPL_DEPTH_MAX = (1<<8)-1;
const TMPL_ENTRIES_MAX = (1<<16)-1;
const TMPL_TYPES_MAX = (1<<16)-1;


//      tmpl24      tab_4 typ_8 cmd_12     //
//      tmpl16      tab_2 typ_6 cmd_8      //


// header
//

/*

tmpl
    buf t[
        header 32 [ depth_span_8 | ent_span_8 | length 16 ]
        body [ ( depth|delta | tidx|vidx ) | ... ]
    ]
    entries
    type_names [ string, .... ]
    keys m{
        [type_idx]: [key, ...],
        ...
    }
    vals m{
        [type_idx] : [val...],
    }
*/


//       tmpl32: header_5 [ tab_8|typ_8|cmd_16 ... ]     //
function tmpl32_parse(pads, lines, l) {

    const TMPL_HEADER_BYTES = 5
    const bytes = new Uint8Array((l << 4) + TMPL_HEADER_BYTES);
    const u16arr = new Uint16Array(bytes.buffer)

    // TMPL_HEADER_BYTES
    //      len(16bits) tab_bits | typ_bits | cmd_bits |
    u16arr[0] = len;
    bytes[2] = 8;
    bytes[3] = 8;
    bytes[4] = 16;

    // BYTES [ tab|typ|cmd ... ]
    let prev_indent = 0;
    while (l--) {
        const indent = pad_to_depth(pads[l]);
        const typ_cmd = lines[l];
        const idx = l << 2;
        u16arr[(idx + 2) >> 1] = typ_cmd & VIDX_MASK;
        bytes[idx + 1] = typ_cmd >> TIDX_SHIFT;
        bytes[idx] = indent - prev_indent;
        prev_indent = indent;
    }
    return bytes;
}
        // if (delta === 0)
        //     _stay(i)
        // else if (delta >>> 31)
        //     _down()
        // else
        //     _up()


const _uarray_ctors = [
    Uint8Array,
    Uint16Array,
    Uint32Array,
    // ?, ?, ?, BigUint64Array
];
const _iarray_ctors = [
    Int8Array,
    Int16Array,
    Int32Array,
    // ?, ?, ?, BigInt64Array
];
function uarray_alloc(bits, length) {
    return new _uarray_ctors[bits >> 3](length);
}
function iarray_alloc(bits, length) {
    return new _iarray_ctors[bits >> 3](length);
}
function iarray_from_array(bits, array) {
    return new _iarray_ctors[bits >> 3](array);
}


function pad_to_depth(pad) {
    return (pad.length - pad.lastIndexOf('\n'))  //- 1)
        >> TAB_POW;
}


