
const {performance: perf} = require('perf_hooks');
function now_us() {return (perf.now() * 1000)|0};
function now_ms() {return (perf.now())|0};


/*

const {e,a} = ddml

const tests = w.mktmp("dev:hash_bench.suite");

`
    ${el`dev_world#hash_bench.suite`}
    ${el`ext.suite`}
`

`


// w.install(tests, "suite", w.SYSS``

const {q,fn} = syss_tags

const $ = syss_tmpl(function(tag,{q,fn}){
    return tag`
        ${q``}
            ${}
    `
})

const $ = syss_record();
syss`
    ${}
`

const syss = ddml_tmplit(function(){
    return ddml_parse
})

syss_define`suite`(function(dat,tag,h){
    return tag`

`
})

syss_install(dom, 'suite')


syss()`

`

syss`
    ${q`spec!.skip`}
        ${`el:setup`(function(d,e){
            w.focus(d,e);

            const id = w.read_str("id");
            w.write_str("");

            const run = w.read_fn('run');
            w.write_fn('runner', run.toString().includes('async')
                ? fn_clockify(run)
                : fn_sync_clockify(run)
            ;
        }}
        ${fn`el:mount`(async function(d,e){
            w.focus(d,e);

            const before = w.read_fn("before");
            const runner = w.read_fn("runner");
            before();
            const p = runner();
            if (!p.resolved) await p;

        }}
        ${fn`el:print`(async function(d,e){
            const duration = w.read_i32("time:total");

        }}

`)

w.mount(tests, w.DDML
`
    ${e`suite #hash_fns`}
        ${e`spec #murmur3_js`}
            ${fn`before`(function(){

            })}
            ${fn`run`(function(){

            })}
        ${e`>test #murmur2.js`}
`)

console.log( w.print(tests) );




SUITE`
    ${TEST `hash-functions` }
        ${SETUP (function(){
            this.set_str()
        })}
        ${RUN (function(){
            this.get_str()
        })}
        ${THEN (function(){

        })}


`
SUITE(function(){
    return [
        _`hash-fns`,

        $``

        str_hash31 = () => {
            return 100
        },
    ]
})

*/

const report = new Map();
const RESOLVED = Promise.resolve()



async function SUITE(cb) {
    const TESTS = cb();
    const RUNNERS = warmup(TESTS);
    const l = RUNNERS.length;
    let i = 0;
    while (i < l) {
        const p = RUNNERS[i++]();
        if (!p.resolved)
            await p
    }
}
function warmup(TESTS) {
    const l = TESTS.length;
    let i = 0;
    const RUNNERS = new Array(l);
    while (i < l) {
        const fn = TESTS[i];
        const name = fn.name;
        const pre = fn.toString().includes('async') ? 'async' : 'sync';
        let post = 'call';
        if (name[0]!==('_')) {
            post = 'clock'
            // warmup
            for (let i = 0; i < 3; ++i) {
                fn();
            }
        }
        RUNNERS[i] = runners[`${pre}-${post}`](i, name, fn)
        ++i;
    }
}
const runners = {
    "sync-clock"(idx, name, fn){
        tbl.set("name",idx,name);
        return function () {
            tbl.set_now_us("t0",idx);
            fn();
            tbl.set_now_us("t1",idx);
            return RESOLVED
        }
    },
    "async-clock"(idx, name, fn) {
        tbl.set("name",idx,name);
        return async function () {
            tbl.set_now_us("t0",idx);
            await fn()
            tbl.set_now_us("t1",idx);
        }
    },
    "sync-call"(idx, name, fn) {
        // tbl.set("name",idx,name);
        return function () {
            fn()
            return RESOLVED
        }
    },
    "async-call"(idx, name, fn) {
        // tbl.set("name",idx,name);
        return async function () {
            await fn()
        }
    },

}




function _(s) {
    return function() {
        put(h1(s))
    }
}
function __(s) {
    return function() {
        put(h2(s))
    }
}
function ___(s) {
    return function() {
        put(h3(s))
    }
}

let _stk_i = 0
let _stk = ['','','']
function h1(s) {
    _stk_i = 0
    _stack[]
    return
}
function h2(s) {
    return `|-* ${s}`
}
function h3(s) {
    return `|-* ${s}`
}
function put(s) {
    console.log(s)
}