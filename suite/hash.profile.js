const src = require('../../src.js')
const {hash64} = casm.d
const {ent} = require('../dd.js');



let prev = 0

const {map:map_xor} = 
run("ent_xor_prev",function(word){
    let next = ent(word);
    let val = next ^ prev;
    prev = next
    return val;
},false);


run("ent_xor_prev_2",function(word){
    let next = ent(word);
    let val = next ^ prev;
    prev = val
    return val;
},false,map_xor);



const{map:map_ent} = 
run("ent",ent,false);

run("hash64",hash64,false);

run("murmurhash2_32_gc",murmurhash2_32_gc,false);
run("murmurhash3_32_gc",murmurhash3_32_gc,false);



let n = 0;
let iter = 0;
(function () {
    const hset =new Set()    
    for (const h1 of map_ent.keys()) {
        for (const h2 of map_ent.keys()) {        
            if (h1 == h2) continue
            const h3 = (h1-h2);
            if (++iter > 2_000_000) return
            if (hset.has(h3)) {
                n++
            }   
            else 
                hset.add(h3)        
        }
    }
})();


console.log(`${n} ent hash nested minus collisions out of ${iter} `)

function run(name, hash_fn, print_collisions, map) {
    map || (map = new Map())
    const dups = new Map()
    console.time(`${name}`)
    require('./_words.js')(function(word){
        const hash = hash_fn(word)
        if (map.has(hash)) {
            let set
            if (!dups.has(hash))
                dups.set(hash, set = new Set())
            else
                set =dups.get(hash)
            set.add(word)
            set.add(map.get(hash))
        }
        else
            map.set(hash, word);
    })
    console.timeEnd(`${name}`)
    console.log(`   ${dups.size} collisions of 466,552 words`)
    if (print_collisions) {
    for (const [hash,set] of dups) {
        console.log(`   ! ${[...set]}`)
    }
    }   
    return {map, dups}
}




function murmurhash3_32_gc(key, seed) {
	var remainder, bytes, h1, h1b, c1, c1b, c2, c2b, k1, i;
	
	remainder = key.length & 3; // key.length % 4
	bytes = key.length - remainder;
	h1 = seed;
	c1 = 0xcc9e2d51;
	c2 = 0x1b873593;
	i = 0;
	
	while (i < bytes) {
	  	k1 = 
	  	  ((key.charCodeAt(i) & 0xff)) |
	  	  ((key.charCodeAt(++i) & 0xff) << 8) |
	  	  ((key.charCodeAt(++i) & 0xff) << 16) |
	  	  ((key.charCodeAt(++i) & 0xff) << 24);
		++i;
		
		k1 = ((((k1 & 0xffff) * c1) + ((((k1 >>> 16) * c1) & 0xffff) << 16))) & 0xffffffff;
		k1 = (k1 << 15) | (k1 >>> 17);
		k1 = ((((k1 & 0xffff) * c2) + ((((k1 >>> 16) * c2) & 0xffff) << 16))) & 0xffffffff;

		h1 ^= k1;
        h1 = (h1 << 13) | (h1 >>> 19);
		h1b = ((((h1 & 0xffff) * 5) + ((((h1 >>> 16) * 5) & 0xffff) << 16))) & 0xffffffff;
		h1 = (((h1b & 0xffff) + 0x6b64) + ((((h1b >>> 16) + 0xe654) & 0xffff) << 16));
	}
	
	k1 = 0;
	
	switch (remainder) {
		case 3: k1 ^= (key.charCodeAt(i + 2) & 0xff) << 16;
		case 2: k1 ^= (key.charCodeAt(i + 1) & 0xff) << 8;
		case 1: k1 ^= (key.charCodeAt(i) & 0xff);
		
		k1 = (((k1 & 0xffff) * c1) + ((((k1 >>> 16) * c1) & 0xffff) << 16)) & 0xffffffff;
		k1 = (k1 << 15) | (k1 >>> 17);
		k1 = (((k1 & 0xffff) * c2) + ((((k1 >>> 16) * c2) & 0xffff) << 16)) & 0xffffffff;
		h1 ^= k1;
	}
	
	h1 ^= key.length;

	h1 ^= h1 >>> 16;
	h1 = (((h1 & 0xffff) * 0x85ebca6b) + ((((h1 >>> 16) * 0x85ebca6b) & 0xffff) << 16)) & 0xffffffff;
	h1 ^= h1 >>> 13;
	h1 = ((((h1 & 0xffff) * 0xc2b2ae35) + ((((h1 >>> 16) * 0xc2b2ae35) & 0xffff) << 16))) & 0xffffffff;
	h1 ^= h1 >>> 16;

	return h1 >>> 0;
}


function murmurhash2_32_gc(str, seed) {
    var
      l = str.length,
      h = seed ^ l,
      i = 0,
      k;
    
    while (l >= 4) {
        k = 
          ((str.charCodeAt(i) & 0xff)) |
          ((str.charCodeAt(++i) & 0xff) << 8) |
          ((str.charCodeAt(++i) & 0xff) << 16) |
          ((str.charCodeAt(++i) & 0xff) << 24);
      
      k = (((k & 0xffff) * 0x5bd1e995) + ((((k >>> 16) * 0x5bd1e995) & 0xffff) << 16));
      k ^= k >>> 24;
      k = (((k & 0xffff) * 0x5bd1e995) + ((((k >>> 16) * 0x5bd1e995) & 0xffff) << 16));
  
      h = (((h & 0xffff) * 0x5bd1e995) + ((((h >>> 16) * 0x5bd1e995) & 0xffff) << 16)) ^ k;
  
      l -= 4;
      ++i;
    }
    
    switch (l) {
    case 3: h ^= (str.charCodeAt(i + 2) & 0xff) << 16;
    case 2: h ^= (str.charCodeAt(i + 1) & 0xff) << 8;
    case 1: h ^= (str.charCodeAt(i) & 0xff);
            h = (((h & 0xffff) * 0x5bd1e995) + ((((h >>> 16) * 0x5bd1e995) & 0xffff) << 16));
    }
  
    h ^= h >>> 13;
    h = (((h & 0xffff) * 0x5bd1e995) + ((((h >>> 16) * 0x5bd1e995) & 0xffff) << 16));
    h ^= h >>> 15;
  
    return h >>> 0;
  }