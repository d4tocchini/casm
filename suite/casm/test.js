
SUITE('casm', function(){
    require('./c.test.js')    
    require('./casm.test.js')
    require('./bindings.test.js')
})


// const pid = process.pid;

// console.log({pid}, `kill -USR2 ${pid}`)

// function onSIGUSR2() {
//     console.log('SIGUSR2...');
//     _eval_run();
//     console.log('...SIGUSR2');
// }
// process.on('SIGUSR2', onSIGUSR2);
// process.stdin
//     .on('data', data => {
//       console.log('@@@@@@@',data)
//       process.exit(0)
//     })
//     .on('end', () => {
//         console.log('_____',data)
//       process.exit(0)
//     });

//     // bindings._eval_flush();
//     setTimeout(bindings._eval_flush,5)
//     process.nextTick(bindings._eval_flush)
