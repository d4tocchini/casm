// node --trace-deopt ./suite/bench

const {
    SUITE,
    BENCH,
} = require('../../lib/dev.js')
var mkdirp = require('mkdirp');
var rimraf = require('rimraf');
const dir = __dirname + '/.db'

rimraf(dir, function(err) {
    // if (err) {
    //   return done(err);
    // }
    // setup clean directory
    mkdirp(dir, function(err) {
        mkdirp(dir+"/lmdb",function(){
    //   if (err) {
    //     return done(err);
    //   }
    //   done();
            run();
        })
    });
});

function run() {
    // SUITE('bench',function(){
        const libs = {
            casm: CASM('casm'),
            casm_txn: CASM('casm_txn'),
            casm_crypto: CASM_CRYPTO(),
            lmdb: LMDB(),
        }
        const size = 1024 << 6;
        BENCH("casm:setup", function() {
            libs.casm.setup({size});
        })
        run(libs.casm);
        run(libs.casm);

        libs.casm_txn.setup({size});
        run_fn('casm_opt',function(){
            libs.casm_txn.txnfn( _run );
        })
        run_fn('casm_opt',function(){
            libs.casm_txn.txnfn( _run );
        })

        libs.casm_crypto.setup({size});
        run(libs.casm_crypto);
        run(libs.casm_crypto);

        BENCH("lmdb:setup", function() {
            libs.lmdb.setup({size});
        })

        run(libs.lmdb);
        run_fn('lmdb_opt',function(){
            libs.lmdb.txnfn( _run );
        })

        run(libs.lmdb);
        run_fn('lmdb_opt',function(){
            libs.lmdb.txnfn( _run );
        })


        teardown('casm');
        teardown('lmdb');

        function run(lib) {
            BENCH(lib.name+":io",function() {
                _run(lib)
            });
        }
        function run_fn(name,fn) {
            BENCH(name+":io",function() {
                fn();
            });
        }
        function teardown(name) {
            const lib = libs[name];
            BENCH(name+":teardown",function() {
                lib.teardown();
            })
        }
        function _run(lib) {
            let i = 0
            let val
            while (i<1000) {
                lib.set('xxx'+i,'hello world'+(i*Math.random()))
                val = lib.get('xxx'+i)
                lib.set('yyy'+i,'hello worldasdf'+i)
                val = lib.get('yyy'+i)
                ++i
            }
            i = 0
            while (i<1000) {
                lib.set('xxx'+i,'hello world'+(i*Math.random()))
                val = lib.get('xxx'+i)
                lib.set('yyy'+i,'hello worldasdf'+i)
                val = lib.get('yyy'+i)
                ++i
            }
        }

    // })
}

function LMDB() {
    const lmdb = require('node-lmdb');
    let env;
    let dbi;
    return {
        name:'lmdb',
        setup,
        get,
        set,
        geti,
        seti,
        teardown,
        txnfn
    }
    var txn
    function setup({size}) {

        env = new lmdb.Env();
        env.open({
            path: dir+"/lmdb",
            maxDbs: 10,
            mapSize: 2<<20 // maximum database size
        });
        dbi = env.openDbi({
            name: "xxx",
            create: true // will create if database did not exist
        })
        // txn = env.beginTxn();
    }
    function teardown(){
        // txn.commit();
        dbi.close();
    }
    function set(k, v) {
        var txn = env.beginTxn();
        try {
            txn.putString(dbi, k, v);
            txn.commit();
        }
        catch(e) {
            txn.abort();
            throw new Error(e)
        }
    }
    function get(k) {
        var txn = env.beginTxn();
        var val;
        try {
            val = txn.getString(dbi,k);  // getStringUnsafe
            txn.commit();
        }
        catch(e) {
            txn.abort();
            throw new Error(e)
        }
        return val;
    }
    function seti(i,v) {

    }
    function geti(i) {

    }

    function txnfn(fn) {
        const txn = env.beginTxn();
        function txn_get(k) {
            let val = txn.getString(dbi,k);  // getStringUnsafe
            return val;
        }
        function txn_set(k,v) {
            txn.putString(dbi, k, v);
        }
        try {
            fn({
                name:'lmdb_opt',
                get: txn_get,
                set: txn_set
            });
            txn.commit();
        }
        catch(e) {
            txn.abort();
            throw new Error(e)
        }
    }
    function txn_end() {

    }

}
const casm = require('../../lib/casm.js/index.js')
function CASM(name) {
    const {d} = casm;
    return {
        name,
        setup,
        get,
        set,
        geti,
        seti,
        teardown,
        txnfn
    }
    function setup({size}) {
        d.pushx(
            d.mkx(dir+'/' +name,2<<28,size,d.PERSIST|d.CLEAR)
        );
    }
    function teardown() {
        d.popx();
    }
    function set(k,v) {
        d.key_str(k);
        d.set_str(v);
        d.PUT();
    }
    function _set_v_k(v,k) {
        d.key_str(k);
        d.set_str(v);
        d.PUT();
    }
    function get(k) {
        // d.key_str(k);
        d.REQ();
        return d.get_str();
    }
    function seti(i,v) {

    }
    function geti(i) {

    }
    function txnfn(fn) {
        const map = new Map();
        function txn_get(k) {
            return map.get(k);
        }
        function txn_set(k,v) {
            map.set(k,v);
        }
        fn({
                name:'casm_opt',
                get: txn_get,
                set: txn_set
        });
        map.forEach(_set_v_k);
    }
}
function CASM_CRYPTO() {
    const {d} = casm;
    return {
        name:'casm_crypto',
        setup,
        get,
        set,
        geti,
        seti,
        teardown,
    }
    function setup({size}) {
        d.pushx(
            d.mkx(dir+'/casm_crypto',2<<30,size<<1,d.PERSIST|d.CLEAR)
        );
        d.secret_set_phrase('password')
    }
    function teardown() {
        d.popx();
    }
    function set(k,v) {
        d.secure_key_str(k);
        d.secure_set_str(v);
        d.PUT();
    }
    function get(k) {
        d.secure_key_str(k);
        d.REQ();
        return d.secure_get_str();
    }
    function seti(i,v) {

    }
    function geti(i) {

    }
}

function fs_sync() {

}

function fs_async() {

}

function mem() {

}