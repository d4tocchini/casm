
const N = 100000


let m = a()
let m2 = a2()
let mb = b()
c()
d()
e()


// x(m); x(m2); // deopt with diff obj

x(m); x(mb);

y(m); y(m2);

function a(){
    let n = N
    let m = new Map()
    while (n--)
        m.set(n, n+n);
    // m.set('adsf', 'asdf');
    // m.set('adsfss', 'asdfss');
    // m.set('adsfsss', 'asdfsss');
    return m
}
function a2(){
    let n = N
    let m = {get(x){return m[x]}}
    while (n--)
        m[n] = n
    return m
}

function b(){
    let m = new Map()
    for (let i = 0; i < N; ++i) {
        m.set(''+i, ''+i + Math.random());
    }
    return m
}
function c(){
    class M extends Map {
    }
    let m = new M()
    for (let i = 0; i < N; ++i) {
        m.set(''+i, ''+i + Math.random());
    }
}
function d(){
    class M extends Map {
        constructor() {
            super()
            const that = this
            const _set = this.set
            this.set = function(k,v) {
                _set.call(that,k,v)
            }
        }
    }
    let m = new M()
    for (let i = 0; i < N; ++i) {
        m.set(i, i + Math.random());
    }

}
function e(){
    class M {
        constructor() {
            let map = new Map()
            let stringmap = new Map();

            // const that = this
            let _done = __done;
            function __done() {
                return 100;
            }
            function _done_noop(){}
            this.set_done_noop = function() {
                _done = _done_noop
            }
            this.set_done = function() {
                _done = __done;
            }
            this.setter = function(fn)  {
                this.set = fn; // DEOPT!
            }
            this.set_string = function(k,v) {
                stringmap.set(k+'',v+'')
                _done()
            }
            this.set = function(k,v) {
                map.set(k,v)
                _done()
            }
        }
        // set() {

        // }
    }
    let m = new M()
    // NO DEOPT! only if put in 2 functions!
    // let {set,set_string} = m
    m1();
    m2()
    m1();  //DEOPT()
    function m1(){
        m.set_done() // STOPS DEOPT 2nd time
        for (var i = 0; i < N; ++i) {
            m.set(i|0, (i + i)|0);
        }
    }
    function m2() {
        // m.setter(m.set_string)
        m.set_done_noop()
        // m.setter(m.set_string) // DEOPT!
        const a = '100|0';
        const b = '200|0';
        m.set_string(a, b);
    }

    // for (var j = 0; j < N; ++j) {

    // }
    // DEOPT!
    // let set_string = m.set_string
    // for (let i = 0; i < N; ++i) {
    //     set2(''+i, ''+i + i);
    // }
}


function x(m){
    let n = N
    let s
    while (n--)
        s = m.get(n)
}

function y(m){
    let n = N
    let s = m.get(100)
    s = m.get(101)
    s = m.get(102)
    s = m.get(103)
    s = m.get(104)
    return s + "234234234"
}