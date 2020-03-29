
require('./_bench.js')

// =========================================================
const vm = require('vm');
global.fn = function() {return 0}
global.fn2 = function(a,b) {return a+b}
const EMPTY_ARRAY = []
const SPECS = [
    // _(`str_to_fn argc=0`,
    //     function (){},
    //     // function (){return (fn() === loop_l()-1);}
    // ),

    //     function eval_assignment (i) {
    //         eval(`fn=function(){return ${i}}`);
    //     },
    //     function new_Function (i) {
    //         fn = new Function(`return ${i}`);
    //     },
    //     function vm_script_assign (i) {
    //         (new vm.Script(`fn=function(){return ${i}}`,"vm.js")).runInThisContext();
    //     },
    //     function vm_runInThisContext(i) {
    //         vm.runInThisContext(`fn=function(){return ${i}}`)
    //     },
    //     function vm_compileFunction (i) {
    //         fn = vm.compileFunction(`return ${i}`,EMPTY_ARRAY);
    //     },

    _(`str_to_fn argc=2`,
        function (){},
        // function (){return (fn(1,2) === loop_l()-1+1+2);}
    ),

        function eval_assignment_2 (i) {
            eval(`fn=function(a,b){return a+b+${i}}`);
        },
        function new_Function_2 (i) {
            fn = new Function('a','b',`return a+b+${i}`);
        },
        function new_vm_script_assign_this_ctx_2 (i) {
            (new vm.Script(`fn=function(a,b){return a+b+${i}}`,
                {filename:"vm.js"})).runInThisContext();
        },
        function vm_runInThisContext_2(i) {
            vm.runInThisContext(`fn=function(a,b){return a+b+${i}}`)
        },
        function vm_compileFunction_2 (i) {
            fn = vm.compileFunction(`return a+b+${i}`,['a','b']);
        },

]
bench(SPECS, 2);
bench(SPECS, 10);
bench(SPECS, 100_000);
bench(SPECS, 10_000);
bench(SPECS, 1000);
bench(SPECS, 100);
bench(SPECS, 10);
bench(SPECS, 1000);
bench_report()