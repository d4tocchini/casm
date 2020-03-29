
//  node --allow-natives-syntax
SUITE("monomorphic",()=>{
    // https://benediktmeurer.de/2017/06/29/javascript-optimization-patterns-part2/
    // https://benediktmeurer.de/tags/v8/
    // https://www.npmjs.com/package/v8-natives
    // https://github.com/v8/v8/blob/master/src/runtime/runtime.h
    // https://glebbahmutov.com/blog/detecting-function-optimizations-in-v8/
    // var obj = { a: true, b: false };
    // console.log(%HasFastProperties(obj)); // true (Fast mode)
    // delete obj.a;
    // console.log(%HasFastProperties(obj)); // false (Dictionary mode)

    // %HasSmiElements

    // https://benediktmeurer.de/2017/07/14/faster-collection-iterators/

    function monomorph(a,b) {return %HaveSameMap( a, b )}
    TEST("Obj",(Y)=>{
        Y( monomorph({x: 100}, {x: 123}) );
        Y( monomorph({x: 100}, {x: -123}) );
        Y( monomorph({x: (1<<31) + 1}, {x: 1}) );
        Y( !monomorph({x: 100}, {x: -123.123}) );
        Y( !monomorph({x: 100}, {x: null}) );
        Y( monomorph({x: 100}, {x: 'asdf'}) );
        Y( !monomorph({x: 100}, {y: 220}) );
    })
    TEST("Fun",(Y)=>{
        Y( monomorph(function(){}, function(){return 100}) );
        Y( monomorph(function(x,y,z){return[x,y,z]}, function(a,b){return 100}) );
        var f = function(x,y,z){return[x,y,z]};
        f.xxx = 'xxx'
        Y( !monomorph(f, function(x,y,z){return[x,y,z]} ));
    })
    TEST("Map",(Y)=>{
        var int_int = new Map();
        var int_str = new Map();
        var str_str = new Map();
        var str_fun = new Map();
        int_int.set(99, 99);
        int_int.set(999, 999);
        int_str.set('x', 99);
        int_str.set('y', 999);
        str_str.set('x', 'x');
        str_str.set('y', 'y')
        str_fun.set('f',function(){})
        Y( monomorph(int_int, int_str) );
        Y( monomorph(int_int, str_str) );
        Y( monomorph(int_int, str_fun) );
        Y( monomorph(int_int, new Map()) );

        Y( monomorph(int_int.get, str_fun.get) );
        Y( monomorph(int_int.values, str_fun.values) );
        Y( monomorph(int_int.values(), str_fun.values()) );
    })
})