
module.exports = SUITE('bindings.js',() => {

    let bindings
    TEST("require", function(Y){
        Y(!!(bindings = require('../../lib/c.js').bindings))
    })

    const {
        str_hash64, str_hash32, str_hash31,
        xxhash64, xxhash32,
        c_malloc_str, c_free,
    } = bindings;

    function str_xxhash32(str) {
        const str_l = str.length;
        const str_p = c_malloc_str(str,str_l+1);
        const hash = xxhash32(str_p, str_l, 0);
        c_free(str_p);
        return hash;
    }
    function str_xxhash64(str) {
        const str_l = str.length
        const str_p = c_malloc_str(str,str_l+1)
        const hash = xxhash64(str_p, str_l, 0n)
        c_free(str_p)
        return hash;
    }
    TEST('c_malloc xxhash64 c_free',function(Y,N){
        Y(str_xxhash64("hello")!=0)
        Y(str_xxhash64("hello")==str_xxhash64("hello"))
        Y(str_xxhash64("hello")!=str_xxhash64("hello!"))
    })
    TEST('c_malloc xxhash32 c_free',function(Y,N){
        Y(str_xxhash32("hello")!=0)
        Y(str_xxhash32("hello")==str_xxhash32("hello"))
        Y(str_xxhash32("hello")!=str_xxhash32("hello!"))
    })

    TEST('str_hash64',function(Y,N){
        Y(str_hash64("hello",5,0n)!=0)
        Y(str_hash64("hello",5,0n)==str_hash64("hello",5,0n))
        Y(str_hash64("hello",5,0n)!=str_hash64("hello!",6,0n))
    })
    TEST('str_hash32',function(Y,N){
        Y(str_hash32("hello",5,0)!=0)
        Y(str_hash32("hello",5,0)==str_hash32("hello",5,0))
        Y(str_hash32("hello",5,0)!=str_hash32("hello!",6,0))
    })

    TEST('str_hash31',function(Y,N){
        Y(str_hash31("hello",5,0)!=0)
        Y(str_hash31("hello",5,0)==str_hash31("hello",5,0))
        Y(str_hash31("hello",5,0)!=str_hash31("hello!",6,0))
    })


    // function secret_set_phrase(pass) {
    //     if (typeof pass !== 'string') {
    //         console.warn(`secret_set_phrase requires string, got ${pass}`)
    //         return
    //     }
    //     const pass_l = pass.length
    //     const pass_p = c_malloc_str(pass, pass_l+1)
    //     crypto_hash256(secret_p, pass_p, pass_l, NULL)
    //     secret_h64 = xor256to64(secret_p);
    //     c_free(pass_p)
    // }

})