
function init(name) {

}

const _maps = new Map();
const _muts = new Map();
const _gens = new Map();
const _histories = new Map();

const EMPTY_MAP = new Map();

function ShMap() {
    this._id = 0;
    this.head = 0n;
    this._gen = 0;
    this._map = EMPTY_MAP;
    this._mut = EMPTY_MAP;
    this._history = EMPTY_ARRAY;
}

const proto = ShMap.prototype;

proto.mount = function (id) {
    this._id = id;
    this.head = 0n;
    this._gen = _gens.get(id)
    this._map = _maps.get(id);
    this._mut = _muts.get(id);
    this._history = _histories.get(id)
}


proto.has = function (k) {
    return this._map.has(k);
}

proto.get = function (k) {
    return this._map.get(k)
}

proto.set = function (k, v) {
    const m = this._map;
    if (m.has(k) && m.get(k) === v) {
        m.set(k, v);
        this.op_remove(k, v);
    }
    return this;
}

proto.delete = function (k) {
    const m = this._map;
    if (m.has(k)) {
        m.delete(k)
        this.op_del(k);
    }
    return this;
}

proto.op_make = function () {

}

proto.op_assign = function (k, v) {
    this._mut.set(k, v)
}

proto.op_remove = function (k) {
    this._mut.set(k, undefined)
}


proto.needs_save - function () {
    return this._mut.size;
}

proto.save = function () {
    if (this.needs_save() === 0)
        return

    // TODO:
    const handle
    const head = io_append_log(handle, this._id, ser_patch(this._mut))

    map_merge(this._map, this._mut);
    this._mut.clear();
}

proto.revert = function () {

}

proto.needs_load = function () {
    const local_head = this.get_head()

    const handle
    const head = io_get_head(head, this._id)
    return head - local_head
}

proto.load = function () {
    const head_diff = this.needs_load()
    if (head_diff === 0)
        return

    const bufs = []

    const local_head // TODO:
    let count = 0
    let idx = local_head + head_diff;
    while (idx !== local_head) {
        const buf = io_read_log(idx);
        bufs.unshift(buf);
        ++count;
    }

    this.apply_patches(bufs);

}

proto.apply_patches = function (bufs) {
    const cur_
    const cur_stamp
    const l = bufs.length
    let i = 0
    while (i < l) {
        const buf = bufs[i++];
        const des = patch_des(buf);
        const idx = des.read_u32();
        const stamp = des.read_u32();
        const mut = des.read_value();
    }

    map_merge(this._map, );
}



// (de)serialize

function patch_des(buf) {

}
function patch_ser() {

}


function ser_patch (mut) {
    return v8.serialize(mut);
}

function des_patch (buf) {
    return v8.deserialize(buf);
    // this._history.push(mut);
}


// i/o

function io_touch (path, flags) {
    const capacity = 1 << 12
    const size = 1 << 30
    const handle = d.mkx(path, capacity, size, flags)
    return handle;
}

function io_open (path) {
    const handle = d.openx(path);
    return handle;
}

function io_get_head (handle, id) {
    txn_begin_meta(handle);

    const head_idx = _r_lock(id)
    _r_unlock_immediate();

    txn_end();
    return head_idx;
}

function io_append_log (handle, id, buf) {
    txn_begin_meta(handle);

    const prev_idx = _w_lock(id);
    _w_unlock(id, prev_idx+1)

    txn_end();

    txn_begin_data(handle);

    txn_write_log(prev_idx, id, buf)

    txn_end();
    return head_idx;
}

function io_read_log (handle, idx) {

}


// transactions

function txn_begin_meta (handle) {

}

function txn_begin_data (handle) {

}

function txn_end () {

}

function txn_read_meta (id) {

}

function txn_write_log (prev_idx, id, buf) {

}

// utils

function map_merge (out, override) {

}

function map_defaults (out, defaults) {

}
