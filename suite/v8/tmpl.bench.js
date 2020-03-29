
require('./_bench.js')

// =========================================================

let fn_map = new Map();

function FN(keys) {
    const k = keys[0]+loop_i();
    return function(fn) {
        fn_map.set(k, fn)
    }
}

let _fnst_k = ''
function FN_STATIC(keys) {
    _fnst_k = keys[0]+loop_i();
    return _fnst_return;
}
function _fnst_return(fn) {
    fn_map.set(_fnst_k, fn)
}

function set_fn(k, fn) {
    fn_map.set(k+loop_i(), fn)
}


function FN_K1(strings, key) {
    const k = strings[0] + key;
    return function(fn) {
        fn_map.set(k, fn)
    }
}

let _fnst_k1 = ''
function FN_STATIC_K1(strings, key) {
    _fnst_k1 = strings[0] + key;
    return _fnst_return_k1;
}
function _fnst_return_k1(fn) {
    fn_map.set(_fnst_k1, fn)
}

function set_fn_k1(k, i, fn) {
    fn_map.set(k+i, fn)
}


function FN_K2(strings, i, fn) {
    fn_map.set(strings[0] + i, fn)
}



const _pure_cache = new Map();
function FN_PURE(strs, i, body) {
    fn_map.set(strs[0] + i, _pure_new_fn(i,body));
}
function _pure_new_fn(i,body) {
    let fn;
    if (_pure_cache.has(body)) {
        fn = _pure_cache.get(body)
    }
    else {
        fn = new Function('i',body);
        _pure_cache.set(body, fn);
    }
    return function(){return fn(i)};
}



const SPECS = [
    _(`set_fn`,
        function (){fn_map.clear()},
        function (){return (fn_map.size === loop_l()) && (0 === 0|fn_map.get('key'+0)());}
    ),

        function func (i) {
            set_fn('key',function(){return i})
        },
        function tmpl (i) {
            FN`key`(function(){return i})
        },
        function tmpl_static (i) {
            FN_STATIC`key`(function(){return i})
        },

        function func_k1 (i) {
            set_fn_k1('key',i,function(){return i})
        },
        function tmpl_k1 (i) {
            FN_K1`key${i}`(function(){return i})
        },
        function tmpl_static_k1 (i) {
            FN_STATIC_K1`key${i}`(function(){return i})
        },

        function tmpl_k2 (i) {
            FN_K2`key${i}${function(){return i}}`
        },

        // function tmpl_pure_string (i) {
        //     FN_PURE`key${`${i}`}${`return i`}`
        // },




]
bench(SPECS, 1);
bench(SPECS, 100);
bench(SPECS, 1000);
bench(SPECS, 10_000);
bench(SPECS, 500_000);
bench(SPECS, 10);
bench_report()