const LOG = function(...args) {
    console.log('casm',...args)
};


const c = global.c = require('./c.js');
const d = global.d = require('./d.js');
let {APP_DATA_DIR} = process.env;
if (!APP_DATA_DIR)
    APP_DATA_DIR = "/tmp"
function set_data_path(path) { return APP_DATA_DIR = path; }
function get_data_path() { return APP_DATA_DIR; }

const fn = fn_module();
const NOOP = global.NOOP = function(){};
const ARRAY_EMPTY = [];


global.PutTakeMap = class PutTakeMap extends Map {
    constructor() {
        super()
        const that = this;
        let allocs = 0;
        this.put = function(v) {
            this.set( allocs, v);
            return allocs++;
        }
        this.take = function(k) {
            const val = that.get(k);
            that.delete(k);
            return val;
        }
        this.nextid = function () {
            return allocs
        }
    }
}
const AGENTS = new Map();
const MODULES = new Map();
const MODULE_PATHS = new Map();
const REQS = new PutTakeMap();

module_set('put-take-map',PutTakeMap);
module_path_set('fs',__dirname+'/fs/index.js');
module_path_set('dev',__dirname+'/dev.js');

let _exec;
let _eval;
const casm = global.casm = module.exports = {
    c,
    d,
    fn,
    agent:undefined,
    AGENTS,
    MODULES,
    MODULE_PATHS,
    REQS,
    js_eval: eval,
    mount,
    remount,
    premount_clear,
    set_data_path,
    get_data_path,
    q_pathof,
    exec(name,fn,args) {return casm.agent.exec(name,fn,args);},
    eval(name,str) {return casm.agent.eval(name,str);},
    raf(fn){return casm.agent.request_async_frame(fn);},
    module_set,
    module_get,
    module_has,
    module_del,
    module_path_set,
    module_path_get,
    module_path_has,
    module_path_del,
    require(name) {
        return MODULES.has(name)
            ? MODULES.get(name)
            : MODULE_PATHS.has(name)
                ? module_set(name, require(MODULE_PATHS.get(name)) )
                : null
        ;
    },
    _gen_res(name, q_path, rid, data, status) {
        const response = AGENTS.get(name)
            .JS()
            .in(q_path);

        let val = ''
        if (status) {
            val = `"${data}"`
        }
        else {
            if (data && data.buffer) {
                throw new Error('xxx')
            }
            else
                val = JSON.stringify(data);
        }

        response
            .src(`if (casm.REQS.has(${rid})) {casm.REQS.take(${rid})[${status}](${val});}`)
            .$();
    },
}

function module_set(name,mod) {MODULES.set(name,mod); return mod;}
function module_get(name) {return MODULES.get(name);}
function module_has(name) {
    return MODULES.has(name)
        || module_path_has(name);
}
function module_del(name) {return MODULES.delete(name);}
function module_path_set(name,p) {MODULE_PATHS.set(name,p); return p;}
function module_path_get(name) {return MODULE_PATHS.get(name);}
function module_path_has(name) {return MODULE_PATHS.has(name);}
function module_path_del(name) {return MODULE_PATHS.delete(name);}

function q_pathof(name) { return `${APP_DATA_DIR}/.q_${name}` }
function q_exists(name) {  }

// const  _MOUNT_src =  _MOUNT.toString();
// function MOUNT(_name) {
//     // let ret;
//     // eval(_MOUNT_src + '\n ret = _MOUNT(_name);');
//     // return ret;
//     return new Function('_name', _MOUNT.toString() + '\n return _MOUNT(_name);');
// }

let q_cap = 1024;
let q_size = 1<<20;
function premount_clear(name) {
    return casm.require('fs').rm(q_pathof(name));
}
function mount(_name, _exports) {
    const Q_PATH = q_pathof(_name);
    return _mount(_name, _exports, {
        parent:'',
        path:[_name],
        q_flags: d.PERSIST,
        Q_PATH
    });
}
function remount(_name, _exports) {
    const Q_PATH = q_pathof(_name);
    const exists = 0|(casm.require('fs').exists_sync(Q_PATH));
    // console.log({Q_PATH,exists})
    return _mount(_name, _exports, {
        parent:'',
        path:[_name],
        q_flags: d.PERSIST | (exists*d.CLEAR), // | d.CLEAR, // (exists*d.CLEAR),
        Q_PATH
    });
}
function _mount(_name, _exports, {parent,path,q_flags, Q_PATH}) {
    // LOG('_mount',{_name,parent,path,q_flags, Q_PATH})
    const q_pathof = casm.q_pathof;
    // let q_flags = d.PERSIST|d.CLEAR; // TODO: d.CLEAR d.PERSIST

    const state = new PutTakeMap();
    const CHILDREN = new Map()
    const CHILDREN_KEYS = [];
    let CHILDREN_LEN = 0;
    let NAME = _name

    let Q_CTX = (q_flags & d.REOPEN) ? d.openx(Q_PATH) : d.mkx(Q_PATH,q_cap,q_size,q_flags);

    let tick = tick_active;

    // Object.setPrototypeOf( _exports, )
    if (!_exports.require)
        _exports.require = casm.require;
    const agent = {
        name:NAME,   parent,  path,
        exports:_exports,
        state,

        exec,
        eval,
        require: casm.require,
        update,
        pause, resume,
        parent_get,
        has, get, //set,
        add, rm,
        destroy,
        q_path,

        JS,
        tick,
    }

    // register
    AGENTS.set(NAME, agent);
    AGENTS.set(Q_PATH, agent);
    MODULES.set(NAME, agent.exports);

    // first mount is root .... TODO:
    if (!casm.agent) {
        casm.agent = agent;
    }
    function exec(name, fun, args) {
        args || (args = ARRAY_EMPTY);
        return name == NAME || name == 'self'
            ?   request_async_frame(fun, args)
            :   eval(name, fn.decode_body_bound(fun,args))
        ;
    }
    function eval(name, s) {
        return _JS_API().in(name).async(s).$();
    }
    function JS() { return _JS_API(); }

    function update() { tick(); }
    function pause() { tick = tick_inactive; }
    function resume() { tick = tick_active; }
    function parent_get() { return parent ? AGENTS.get(parent) : null; }
    function has(name) {return CHILDREN.has(name);}
    function get(name) {return CHILDREN.get(name);}
    function add(name, _exports) {
        if (CHILDREN.has(name))
            return null

        path = path.slice(0);
        path.push(name);

        const child = mount(name, _exports, {parent:NAME,path})

        CHILDREN.set(name, child)
        CHILDREN_KEYS[CHILDREN_LEN++] = name;
        return child
    }
    function rm(name) {
        if (!CHILDREN.has(name))
            return 0
        MODULES.delete(NAME)
        AGENTS.delete(NAME)
        AGENTS.delete(Q_PATH)
        CHILDREN.delete(name)
        CHILDREN_KEYS.splice( CHILDREN_KEYS.indexOf(name), 1)
        CHILDREN_LEN--
        return 1
    }
    function destroy() {
        MODULES.delete(NAME)
        AGENTS.delete(NAME)
        AGENTS.delete(Q_PATH)
        if (parent)
            AGENTS.get(parent).rm(_name)
    }

    function q_path() {return Q_PATH;}

    const async_frames = []
    let async_frames_t = 0
    let async_frames_l = 0
    let _async_frame_fun = NOOP;
    let _async_frame_args = ARRAY_EMPTY;

    function request_async_frame(fun,args) {
        args && (_async_frame_args = args);
        _async_frame_fun = fun;
        return new Promise(
            (async_frames_t < async_frames_l) ? _recycle_async_frame : _alloc_async_frame);
    }
    function _alloc_async_frame(resolve,reject) {
        async_frames[async_frames_l++] = {
            fun:_async_frame_fun,
            args:_async_frame_args,
            resolve,
            reject
        }
        _async_frame_fun = NOOP;
        _async_frame_args = ARRAY_EMPTY;
        ++async_frames_t;
    }
    function _recycle_async_frame(resolve,reject) {
        const af = async_frames[fasync_frames_t];
            af.fun = _async_frame_fun;
            af.args = _async_frame_args;
            af.resolve = resolve;
            af.reject = reject;
        _async_frame_fun = NOOP;
        _async_frame_args = ARRAY_EMPTY;
        ++async_frames_t;
    }
    function flush_async_frames() {
        let i = 0;
        while (i < async_frames_t) {
            // const {fun,args,resolve,reject} = async_frames[i++];
            const af = async_frames[i++];
            try {
                const result = af.fun.apply(agent.exports, af.args);
                if (result instanceof Promise) {
                    result.then(resolve).catch(reject)
                }
                else {
                    af.resolve(result);
                }
            }
            catch (e) {
                af.reject(e);
            }
            // TODO: ????????????????? clear?
            // af.args = ARRAY_EMPTY
            // af.fun = af.NOOP
        }
        async_frames_t = 0;
        // async_frames.length = 0;
    }


    function tick_inactive(){
        flush_async_frames();
    }
    function tick_active() {
        _tick()
        const l = CHILDREN_LEN
        let i = 0
        while (i < l) {
            const name = CHILDREN_KEYS[i++];
            CHILDREN.get(name).tick();
        }

        flush_async_frames();
    }
    function _tick() {
        let script = '';

        d.pushx(Q_CTX)
            do {
                if ( deq_next() ) break;
                if ( deq_next() ) break;
                if ( deq_next() ) break;
                if ( deq_next() ) break;
                if ( deq_next() ) break;
                if ( deq_next() ) break;
                if ( deq_next() ) break;
                if ( deq_next() ) break;
            } while(true)
        d.popx()

    }
        // let max = 8;
        // console.log('_tick',{Q_CTX,_name,name:agent.name})

    function deq_next() {
        // const t0= process.hrtime()
        d.DEQ();
        const script = d.get_str();

        if (!script) return true;
        // LOG({script})

        _evaluate.call(_exports, script)

        // console.log("deq", (process.hrtime(t0)[1]/1e6) + "ms" )
        return false;
    }

    function _evaluate(script) {
        try {
            // console.log({script})
            // Function.call(Function,'self',script).call(this,this)
            casm.js_eval(script);
        }
        catch(e) {
            console.error(e)
            console.log({script})
        }
    }

    const _JS_API = (function(){

        let src = '';
        let has_buf = 0;
        let tgt_name = '';
        let tgt_path = '';
        let ret_fn = NOOP;
        function BEGIN() {
            src = '';
            tgt_path = '';
            ret_fn = NOOP;
            return $$;
        }
        const $$ = {
            in(name) {
                tgt_name = name;
                tgt_path = (name[0] === '/') ? name : q_pathof(name);
                return $$;
            },
            add_buf(key,buf){ },
            src(str){ src=str; ret_fn=_ret_nil_wrap; return $$ },
            async(str) { src=str; ret_fn=_ret_async;  return $$;},
            // sync() {  return $$;},
            $: END,
        }
        // console.log('openx',{tgt_path,tgt_q_idx})
        function END() {
            const ret_val = ret_fn();
            const tgt_q_idx = d.openx(tgt_path);
            // LOG({tgt_q_idx, src, ret_val})

            d.pushx(tgt_q_idx);
                d.set_str(src);
                d.ENQ();
            d.popx();
            return ret_val
        }
        function _ret_nil_wrap() {
            src =
`(async function() {var self=this;
${src}
}).call(casm.AGENTS.get('${tgt_name}').exports);`
            ;
            return null;
        }
        function _ret_async() {
            const promise = new Promise(function(res,rej){
                const rid = REQS.put([res,rej]);
                src =
`(async function(){var self=this;
${src}
}).call(casm.AGENTS.get('${tgt_name}').exports).then(
function(v){casm._gen_res('${tgt_name}','${Q_PATH}',${rid},v,0);},
function(err){casm._gen_res('${tgt_name}','${Q_PATH}',${rid},err,1);}
);`
            });
            return promise;
        }
        function _ret_sync() {
            // const rid = REQS.put([res,rej]);
            // src = ''
            // + '(function() {\n'
            // + src
            // + '\n})().then('
            // +   `function(val){ _gen_res('${Q_PATH}',${rid},0); },`
            // +   `function(err){ _gen_res('${Q_PATH}',${rid},1); }`
            // + ');'
        }


        return BEGIN;
    })();

    return agent
}



function        str_match(s,rx) {
    const           m = s.match(rx) || ARRAY_EMPTY; m.shift();
                    return m;
}               // - removes leading element in match array
                // - ensures array is always returned

function fn_module() {
    const           FN_DEC_RX = /^[^\(]*\(([^\)]*)[^{]*{([\S\s]*)}$/m
    const           FN_EMPTY = function () {}
    return {
        clone,
        decode,
        decode_body,
        decode_body_bound,
        decode_args,
        encode,
        code_body_append,
        code_body_prepend,
        code_arg_index_of,
        code_arg_set,
    }
    function        clone(f) {return encode(decode(f)); }
    function        decode(f) {return str_match(f.toString(),FN_DEC_RX); }
                    // return [args,body]
    function        decode_body(f)  {return decode(f)[1]; }
    function        decode_body_bound(f,argv) {
                        let [args_str, body] = decode(f);
                        const args = args_str.split(',');
                        let argc = (argv && argv.length)|0;
                        while (argc > 0) {
                            let key = args[--argc];
                            let val = argv[argc];
                            body = `var ${
                                    key
                                }=${
                                    val != undefined
                                        ? JSON.stringify(val)
                                        : undefined
                                };` + body;
                        }
                        return body;
    }
    // function        decode_body_clean(f)  {return decode_body(f).replace(/[\n][ \t]/g,'\n'); }
    function        decode_args(f)  {return decode(f)[0]; }
    function        encode(code) {return Function.apply(code); }
    function        code_body_append(code,str)  {code[1] += str; return code;}
    function        code_body_prepend(code,str) {code[1] = str + code[1]; return code; }
    function        code_arg_index_of(code,key) {return code[0].indexOf(key); }
    function        code_arg_set(code,key,val) {return code_body_prepend(code, `${key}=${val};` ) }
}


                // function  fn_argv_bind