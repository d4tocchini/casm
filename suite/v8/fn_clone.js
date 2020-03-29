
function fn(nb, p) {
    let x = nb + p;
    let y = nb + 241142
    x += Math.random()
    x += 1 + Math.random()
    x += 1 + Math.random()
    x += Math.random()
    x += Math.random()
    x += Math.random()
    x += 1 + Math.random()
    x += 1 + Math.random()
    x += Math.random()
    x += Math.random()
    x += Math.random()
    x += 1 + Math.random()
    x += 1 + Math.random()
    x += Math.random()
    x += Math.random()
    x += Math.random()
    x += 1 + Math.random()
    x += 1 + Math.random()
    x += Math.random()
    x += Math.random()
    return x + y
}

// s.replace(/(^[^{]*{)|(}$)/ig,"")

// s.replace( /(^function[ \t]*)([^\(]*)(\()([^\)()]*)([^{]*{)|([^}]*}$)/gi, function(m,_2,name,_3,args,_4,_5,_6){ console.log({m,name,args,_4,_5,_6}); return "";} )




// parts = s.match( /(^function[ \t]*)([^\(]*)(\()([^\)()]*)([^{]*{)(.*)(}$)/ )
// name = parts[2]; args = parts[4]; body = parts[6];
// console.log(fn.toString())

const fna = fn


for (let i = 0; i < 10000; ++i) {
    fna(i, i);
}
let fnb = fn_clone(fn)
for (let i = 0; i < 1000; ++i) {
    // fnb = fn_clone(fnb)
    fnb(i + '', i + '');
}

function fn_clone(f) {
    // return f
    // const [, name, args, body] = f.toString().match( /^function[ \t]*([^\(]*)\(([^\)]*)[^{]*{([^}]*)}$/ )
    const [, args, body] = f.toString().match( /^[^\(]*\(([^\)]*)[^{]*{([^}]*)}$/ )
    return Function( args, body)
}