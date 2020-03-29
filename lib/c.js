
// let         _ctor_str = []
// let         _pub_str = ''
// function    PUB(strs)
// {               _pub_str = _pub_str + strs[0];
// }
// function    CTOR(f)
// {               _ctor_str.push(f)
//                 // _ctor_str = _ctor_str +
//                 //     `\n${ typeof f === "function" ? `(${f.toString()})()` : f.toString() }`
// }
// function    EXPORT()
// {               // console.log(Function(`${_ctor_str}\n return { ${_pub_str} };` ).toString())

//                 // const l = _ctor_str.length
//                 // _ctor_str.forEach( function (f) {
//                 //     f()
//                 // })

//                 // eval(`exports = module.exports = {${_pub_str}};`)
//                 module.exports = (new Function(`return { ${_pub_str} };`))(module);
//                 _ctor_str = null;
//                 _pub_str = null
// }


NULL = 0n;

console.log({__dirname})
const           bindings = require('../build/Release/casm.node');

const           os = require('os')
const           {
                    c_malloc,
                    c_malloc_align,
                    c_malloc_str, c_deref_str,
                    c_free,
                    strcpy, strncpy,
                    memcpy,
                    xxhash64, xxhash32,
                    crypto_hash256,
                    crypto_box_enc, crypto_box_dec, crypto_box_verify,
                    sym,
                    call1,          call0,          callr,
                    copy_string,
                    // printf,
                    // buf_ref,
                    arrbuf_ref,
                    cframe_apply,
                    cframe_return,  cframep_return,
                    // buf_deref,
                    // arrbuf_deref,
                    version,

                    jit_alloc,
                    jit_alloc_wdefaults,
                    jit_free,
                    jit_sym,
                    jit_string,
                    jit_file,
                    jit_outfile,
                    jit_compile,
                    jit_run,
                }
                = bindings;

const           LITTLE_ENDIAN = os.endianness() === 'LE';
const           C_MEM_INITIAL_PAGES = 1
const           ZERO_CHAR = String.fromCharCode(0);
const           ARRAY_EMPTY = []

                // TODO: https://github.com/zandaqo/structurae/blob/master/lib/string-view.js


/* extern lazy mods
============================================================*================*/

// function        extern(modname)
// {
// }

/* js primitives
============================================================*================*/
function        map_new() {                      return new Mapr(); }
function        map_aliased_new() {              return new MapAliased(new Map());     }
function        map_aliased_new_with(aliasmap) { return new MapAliased(aliasmap); }

class           Mapr extends Map
{                   constructor(alias_map)
                    {   super()
    const               _set = this.set
    var                 that = this
                        this.set = function(k, v)
                        {   _set.call(that, k, v)
                            return v
                }   }   }

class           MapAliased extends Map
{                   constructor(alias_map)
                    {   super()
    const               _set = this.set
    const               _get = this.get
    var                 that = this
                        this.set = function(k, v)
                        {   _set.call(that, (alias_map.has(k) ? alias_map.get(k) : k), v)
                            return v
                        }
                        this.get = function(k)
                        {   return _get.call(that, (alias_map.has(k) ? alias_map.get(k) : k) )
                        }
                        this.alias = function (kalias, k)
                        {   return alias_map.set(kalias, k);
                        }
                        this.alias_map = alias_map
}                   }

/* array
============================================================*================*/

function        array_product(a)
{   let             l = a.length
    let             product = a[--l]
                    while (l)
                        product *= a[--l]
                    return product
}
                // function sum()


/* buf
============================================================*================*/
const           char2code = {"0":48,"1":49,"2":50,"3":51,"4":52,"5":53,"6":54,"7":55,"8":56,"9":57,"ÿ":255,"þ":254,"ý":253,"ü":252,"û":251,"ú":250,"ù":249,"ø":248,"÷":247,"ö":246,"õ":245,"ô":244,"ó":243,"ò":242,"ñ":241,"ð":240,"ï":239,"î":238,"í":237,"ì":236,"ë":235,"ê":234,"é":233,"è":232,"ç":231,"æ":230,"å":229,"ä":228,"ã":227,"â":226,"á":225,"à":224,"ß":223,"Þ":222,"Ý":221,"Ü":220,"Û":219,"Ú":218,"Ù":217,"Ø":216,"×":215,"Ö":214,"Õ":213,"Ô":212,"Ó":211,"Ò":210,"Ñ":209,"Ð":208,"Ï":207,"Î":206,"Í":205,"Ì":204,"Ë":203,"Ê":202,"É":201,"È":200,"Ç":199,"Æ":198,"Å":197,"Ä":196,"Ã":195,"Â":194,"Á":193,"À":192,"¿":191,"¾":190,"½":189,"¼":188,"»":187,"º":186,"¹":185,"¸":184,"·":183,"¶":182,"µ":181,"´":180,"³":179,"²":178,"±":177,"°":176,"¯":175,"®":174,"­":173,"¬":172,"«":171,"ª":170,"©":169,"¨":168,"§":167,"¦":166,"¥":165,"¤":164,"£":163,"¢":162,"¡":161," ":160,"":159,"":158,"":157,"":156,"":155,"":154,"":153,"":152,"":151,"":150,"":149,"":148,"":147,"":146,"":145,"":144,"":143,"":142,"":141,"":140,"":139,"":138,"":137,"":136,"":135,"":134,"":133,"":132,"":131,"":130,"":129,"":128,"":127,"~":126,"}":125,"|":124,"{":123,"z":122,"y":121,"x":120,"w":119,"v":118,"u":117,"t":116,"s":115,"r":114,"q":113,"p":112,"o":111,"n":110,"m":109,"l":108,"k":107,"j":106,"i":105,"h":104,"g":103,"f":102,"e":101,"d":100,"c":99,"b":98,"a":97,"`":96,"_":95,"^":94,"]":93,"\\":92,"[":91,"Z":90,"Y":89,"X":88,"W":87,"V":86,"U":85,"T":84,"S":83,"R":82,"Q":81,"P":80,"O":79,"N":78,"M":77,"L":76,"K":75,"J":74,"I":73,"H":72,"G":71,"F":70,"E":69,"D":68,"C":67,"B":66,"A":65,"@":64,"?":63,">":62,"=":61,"<":60,";":59,":":58,"/":47,".":46,"-":45,",":44,"+":43,"*":42,")":41,"(":40,"'":39,"&":38,"%":37,"$":36,"#":35,"\"":34,"!":33," ":32,"\u001f":31,"\u001e":30,"\u001d":29,"\u001c":28,"\u001b":27,"\u001a":26,"\u0019":25,"\u0018":24,"\u0017":23,"\u0016":22,"\u0015":21,"\u0014":20,"\u0013":19,"\u0012":18,"\u0011":17,"\u0010":16,"\u000f":15,"\u000e":14,"\r":13,"\f":12,"\u000b":11,"\n":10,"\t":9,"\b":8,"\u0007":7,"\u0006":6,"\u0005":5,"\u0004":4,"\u0003":3,"\u0002":2,"\u0001":1,"\u0000":0};
const            hex2byte = {"10":16,"11":17,"12":18,"13":19,"14":20,"15":21,"16":22,"17":23,"18":24,"19":25,"20":32,"21":33,"22":34,"23":35,"24":36,"25":37,"26":38,"27":39,"28":40,"29":41,"30":48,"31":49,"32":50,"33":51,"34":52,"35":53,"36":54,"37":55,"38":56,"39":57,"40":64,"41":65,"42":66,"43":67,"44":68,"45":69,"46":70,"47":71,"48":72,"49":73,"50":80,"51":81,"52":82,"53":83,"54":84,"55":85,"56":86,"57":87,"58":88,"59":89,"60":96,"61":97,"62":98,"63":99,"64":100,"65":101,"66":102,"67":103,"68":104,"69":105,"70":112,"71":113,"72":114,"73":115,"74":116,"75":117,"76":118,"77":119,"78":120,"79":121,"80":128,"81":129,"82":130,"83":131,"84":132,"85":133,"86":134,"87":135,"88":136,"89":137,"90":144,"91":145,"92":146,"93":147,"94":148,"95":149,"96":150,"97":151,"98":152,"99":153,"00":0,"01":1,"02":2,"03":3,"04":4,"05":5,"06":6,"07":7,"08":8,"09":9,"0a":10,"0b":11,"0c":12,"0d":13,"0e":14,"0f":15,"1a":26,"1b":27,"1c":28,"1d":29,"1e":30,"1f":31,"2a":42,"2b":43,"2c":44,"2d":45,"2e":46,"2f":47,"3a":58,"3b":59,"3c":60,"3d":61,"3e":62,"3f":63,"4a":74,"4b":75,"4c":76,"4d":77,"4e":78,"4f":79,"5a":90,"5b":91,"5c":92,"5d":93,"5e":94,"5f":95,"6a":106,"6b":107,"6c":108,"6d":109,"6e":110,"6f":111,"7a":122,"7b":123,"7c":124,"7d":125,"7e":126,"7f":127,"8a":138,"8b":139,"8c":140,"8d":141,"8e":142,"8f":143,"9a":154,"9b":155,"9c":156,"9d":157,"9e":158,"9f":159,"a0":160,"a1":161,"a2":162,"a3":163,"a4":164,"a5":165,"a6":166,"a7":167,"a8":168,"a9":169,"aa":170,"ab":171,"ac":172,"ad":173,"ae":174,"af":175,"b0":176,"b1":177,"b2":178,"b3":179,"b4":180,"b5":181,"b6":182,"b7":183,"b8":184,"b9":185,"ba":186,"bb":187,"bc":188,"bd":189,"be":190,"bf":191,"c0":192,"c1":193,"c2":194,"c3":195,"c4":196,"c5":197,"c6":198,"c7":199,"c8":200,"c9":201,"ca":202,"cb":203,"cc":204,"cd":205,"ce":206,"cf":207,"d0":208,"d1":209,"d2":210,"d3":211,"d4":212,"d5":213,"d6":214,"d7":215,"d8":216,"d9":217,"da":218,"db":219,"dc":220,"dd":221,"de":222,"df":223,"e0":224,"e1":225,"e2":226,"e3":227,"e4":228,"e5":229,"e6":230,"e7":231,"e8":232,"e9":233,"ea":234,"eb":235,"ec":236,"ed":237,"ee":238,"ef":239,"f0":240,"f1":241,"f2":242,"f3":243,"f4":244,"f5":245,"f6":246,"f7":247,"f8":248,"f9":249,"fa":250,"fb":251,"fc":252,"fd":253,"fe":254,"ff":255};
const           code2char = ["\u0000","\u0001","\u0002","\u0003","\u0004","\u0005","\u0006","\u0007","\b","\t","\n","\u000b","\f","\r","\u000e","\u000f","\u0010","\u0011","\u0012","\u0013","\u0014","\u0015","\u0016","\u0017","\u0018","\u0019","\u001a","\u001b","\u001c","\u001d","\u001e","\u001f"," ","!","\"","#","$","%","&","'","(",")","*","+",",","-",".","/","0","1","2","3","4","5","6","7","8","9",":",";","<","=",">","?","@","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","[","\\","]","^","_","`","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","{","|","}","~","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""," ","¡","¢","£","¤","¥","¦","§","¨","©","ª","«","¬","­","®","¯","°","±","²","³","´","µ","¶","·","¸","¹","º","»","¼","½","¾","¿","À","Á","Â","Ã","Ä","Å","Æ","Ç","È","É","Ê","Ë","Ì","Í","Î","Ï","Ð","Ñ","Ò","Ó","Ô","Õ","Ö","×","Ø","Ù","Ú","Û","Ü","Ý","Þ","ß","à","á","â","ã","ä","å","æ","ç","è","é","ê","ë","ì","í","î","ï","ð","ñ","ò","ó","ô","õ","ö","÷","ø","ù","ú","û","ü","ý","þ","ÿ"];
const            byte2hex = ["00","01","02","03","04","05","06","07","08","09","0a","0b","0c","0d","0e","0f","10","11","12","13","14","15","16","17","18","19","1a","1b","1c","1d","1e","1f","20","21","22","23","24","25","26","27","28","29","2a","2b","2c","2d","2e","2f","30","31","32","33","34","35","36","37","38","39","3a","3b","3c","3d","3e","3f","40","41","42","43","44","45","46","47","48","49","4a","4b","4c","4d","4e","4f","50","51","52","53","54","55","56","57","58","59","5a","5b","5c","5d","5e","5f","60","61","62","63","64","65","66","67","68","69","6a","6b","6c","6d","6e","6f","70","71","72","73","74","75","76","77","78","79","7a","7b","7c","7d","7e","7f","80","81","82","83","84","85","86","87","88","89","8a","8b","8c","8d","8e","8f","90","91","92","93","94","95","96","97","98","99","9a","9b","9c","9d","9e","9f","a0","a1","a2","a3","a4","a5","a6","a7","a8","a9","aa","ab","ac","ad","ae","af","b0","b1","b2","b3","b4","b5","b6","b7","b8","b9","ba","bb","bc","bd","be","bf","c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","ca","cb","cc","cd","ce","cf","d0","d1","d2","d3","d4","d5","d6","d7","d8","d9","da","db","dc","dd","de","df","e0","e1","e2","e3","e4","e5","e6","e7","e8","e9","ea","eb","ec","ed","ee","ef","f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","fa","fb","fc","fd","fe","ff"];

class           Buf
{                   constructor(buffer, view, bytes) {
                        this.attach(buffer, view, bytes)
                    }
                    attach(buffer, view, bytes) {
                        this.addr = arrbuf_ref(buffer);
                        this.buffer = buffer;
                        this.view = view;
                        this.bytes = bytes;
                        const byteLength = bytes.byteLength;
                        const byteOffset = bytes.byteOffset;
                        this.u32arr = new Uint32Array(buffer, byteOffset, byteLength >> 2);
                    }
}
function        buf_from_size(size)
{                   return buf_from_buffer( new ArrayBuffer(size) )
}
function        buf_from_bytes(bytes)
{                   return new Buf(
                        bytes.buffer,
                        new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength),
                        bytes)
}
function        buf_from_buffer(buffer, offset, size)
{                   return new Buf(
                        buffer,
                        new DataView(buffer, offset, size),
                        new Uint8Array(buffer, offset, size))
}
function        buf_attach(buf, buffer, offset, size)
{                   return buf.attach(
                        buffer,
                        new DataView(buffer, offset, size),
                        new Uint8Array(buffer, offset, size))
}
function        buf_ref(buf)
{                   // TODO.................................keep pointer / ref-methods in buf?
                    return arrbuf_ref(buf.buffer)
}
function        buf_deref_addr(buf,offset)
{                   return buf_get_i64(buf, offset)
}
function        buf_deref_offset(buf, offset)
{   const               addr = buf_deref_addr(buf, offset)
    const           buf_addr = buf_ref(buf)
                    return n(addr - buf_addr)
}
function        bytes_slice(bytes,off,size)     { return bytes.subarray(off, off+size) }
function        bytes_place(bytes,off,arr)      { return bytes.set(arr_u8(arr), off) }

function        buf_slice(buf,off,size)         { return buf.slice(off, off+size) }
function        buf_slice_bytes(buf,off,size)   { return bytes_slice(buf.bytes, off, size) }
function        buf_slice_view(buf,off,size)    { return new DataView(buf, off, size) }
function        buf_place_bytes(buf,off,arr)    { return bytes_place(buf.bytes, off, arr) }
        
function        buf_get_u64(buf, n)   { return buf.view.getBigUint64(n, LITTLE_ENDIAN); }
function        buf_get_i64(buf, n)   { return buf.view.getBigInt64(n, LITTLE_ENDIAN); }
function        buf_get_f64(buf, n)   { return buf.view.getFloat64(n, LITTLE_ENDIAN); }
function        buf_get_f32(buf, n)   { return buf.view.getFloat32(n, LITTLE_ENDIAN); }

// NOTE: only valid for 32 bit ints
function        buf_get_u32(buf, n)   { return buf.u32arr[n>>2]; }
function        buf_get_i32(buf, n)   { return buf.u32arr[n>>2]|0; }
function        buf_set_u32(buf,n,v){   return buf.u32arr[n>>2] = v; }
function        buf_set_i32(buf,n,v){   return buf.u32arr[n>>2] = v; }
// function        buf_get_u32(buf, n)   { return buf.view.getUint32(n, LITTLE_ENDIAN); }
// function        buf_get_i32(buf, n)   { return buf.view.getInt32(n, LITTLE_ENDIAN); }
// function        buf_set_u32(buf,n,v){   buf.view.setUint32(n, v, LITTLE_ENDIAN); return v; }
// function        buf_set_i32(buf,n,v){   buf.view.setInt32(n, v, LITTLE_ENDIAN); return v; }

function        buf_get_u16(buf, n)   { return buf.view.getUint16(n, LITTLE_ENDIAN); }
function        buf_get_i16(buf, n)   { return buf.view.getInt16(n, LITTLE_ENDIAN); }
function        buf_get_i8(buf, n)    { return buf.view.getInt8(n, LITTLE_ENDIAN); }
function        buf_get_u8(buf, n)    { return buf.bytes[n]; }
function        buf_get_char(buf, n)  { return code2char[ buf_get_u8(buf, n) ]; }

function        buf_set_u64(buf,n,v){   buf.view.setBigUint64(n, v, LITTLE_ENDIAN); return v; }
function        buf_set_i64(buf,n,v){   buf.view.setBigInt64(n, v, LITTLE_ENDIAN); return v; }
function        buf_set_f64(buf,n,v){   buf.view.setFloat64(n, v, LITTLE_ENDIAN); return v; }
function        buf_set_f32(buf,n,v){   buf.view.setFloat32(n, v, LITTLE_ENDIAN); return v; }

function        buf_set_u16(buf,n,v){   buf.view.setUint16(n, v, LITTLE_ENDIAN); return v; }
function        buf_set_i16(buf,n,v){   buf.view.setInt16(n, v, LITTLE_ENDIAN); return v; }
function        buf_set_i8(buf,n,v) {   buf.view.setInt8(n, v, LITTLE_ENDIAN); return v; }
function        buf_set_u8(buf,n,v)  {  return buf.bytes[n] = v; }
function        buf_set_char(buf,n,v){  return buf_set_u8(buf, n, char2code[v]) }

function        buf_slice_u64 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<3); return new BigUint64Array(buffer,byteOffset,len);}
function        buf_slice_i64 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<3); return new BigInt64Array(buffer,byteOffset,len);}
function        buf_slice_f64 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<3); return new Float64Array(buffer,byteOffset,len);}
function        buf_slice_f32 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<2); return new Float32Array(buffer,byteOffset,len);}
function        buf_slice_u32 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<2); return new Uint32Array(buffer,byteOffset,len);}
function        buf_slice_i32 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<2); return new Int32Array(buffer,byteOffset,len);}
function        buf_slice_u16 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<1); return new Uint16Array(buffer,byteOffset,len);}
function        buf_slice_i16 (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len<<1); return new Int16Array(buffer,byteOffset,len);}
function        buf_slice_u8  (buf,p,len) { return buf_slice_bytes(buf,p,len); }
function        buf_slice_i8  (buf,p,len) { const {buffer,byteOffset} = buf_slice_bytes(buf,p,len); return new Int8Array(buffer,byteOffset,len);}
const           buf_slice_char = buf_get_string;
const           buf_slice_hex = buf_get_hex
const           bytes_slice_char = bytes_get_string;
const           bytes_slice_hex = bytes_get_hex

function        buf_place_u64 (buf,p,arr){ buf_place_bytes(buf,p, arr_u64(arr) ); }
function        buf_place_i64 (buf,p,arr){ buf_place_bytes(buf,p, arr_i64(arr) ); }
function        buf_place_f64 (buf,p,arr){ buf_place_bytes(buf,p, arr_f64(arr) ); }
function        buf_place_f32 (buf,p,arr){ buf_place_bytes(buf,p, arr_f32(arr) ); }
function        buf_place_u32 (buf,p,arr){ buf_place_bytes(buf,p, arr_u32(arr) ); }
function        buf_place_i32 (buf,p,arr){ buf_place_bytes(buf,p, arr_i32(arr) ); }
function        buf_place_u16 (buf,p,arr){ buf_place_bytes(buf,p, arr_u16(arr) ); }
function        buf_place_i16 (buf,p,arr){ buf_place_bytes(buf,p, arr_i16(arr) ); }
function        buf_place_i8  (buf,p,arr){ buf_place_bytes(buf,p, arr); }
function        buf_place_u8  (buf,p,arr){ buf_place_bytes(buf,p, arr); }
const           buf_place_char = buf_set_string;
const           buf_place_hex = buf_set_hex;
const           bytes_place_char = bytes_set_string;
const           bytes_place_hex = bytes_set_hex;

function        arr_u64(arr)
{                   if (arr instanceof                BigUint64Array) return arr;
                    if ((0|arr.buffer)==0) return new BigUint64Array(arr)
                            return new BigUint64Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>3)
}
function        arr_i64(arr)
{                   if (arr instanceof                BigInt64Array) return arr;
                    if ((0|arr.buffer)==0) return new BigInt64Array(arr)
                            return new BigInt64Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>3)
}
function        arr_f64(arr)
{                   if (arr instanceof                Float64Array) return arr;
                    if ((0|arr.buffer)==0) return new Float64Array(arr)
                            return new Float64Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>3)
}
function        arr_f32(arr)
{                   if (arr instanceof                Float32Array) return arr;
                    if ((0|arr.buffer)==0) return new Float32Array(arr)
                            return new Float32Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>2)
}
function        arr_u32(arr)
{                   if (arr instanceof                Uint32Array) return arr;
                    if ((0|arr.buffer)==0) return new Uint32Array(arr)
                            return new Uint32Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>2)
}
function        arr_i32(arr)
{                   if (arr instanceof                Int32Array) return arr;
                    if ((0|arr.buffer)==0) return new Int32Array(arr)
                            return new Int32Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>2)
}
function        arr_u16(arr)
{                   if (arr instanceof                Uint16Array) return arr;
                    if ((0|arr.buffer)==0) return new Uint16Array(arr)
                            return new Uint16Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>1)
}
function        arr_i16(arr)
{                   if (arr instanceof                Int16Array) return arr;
                    if ((0|arr.buffer)==0) return new Int16Array(arr)
                            return new Int16Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength>>1)
}
function        arr_u8(arr)
{                   if (arr instanceof                Uint8Array) return arr;
                    if ((0|arr.buffer)==0) return new Uint8Array(arr)
                            return new Uint8Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength)
}
function        arr_i8(arr)
{                   if (arr instanceof                Int8Array) return arr;
                    if ((0|arr.buffer)==0) return new Int8Array(arr)
                            return new Int8Array(arr.buffer,arr.byteOffset,
                                            arr.byteLength)
}
function        buf_get_string(buf,off,size)
{                   return bytes_get_string(buf.bytes, off, size)}
function        mem_get_string (ptr, size)
{                   return bytes_get_string(membuf.bytes, mem_offset(ptr), size)}
function        buf_get_hex(buf,off,size)
{                   return bytes_get_hex(buf.bytes, off, size)}
function        mem_get_hex(ptr,size)
{                   return bytes_get_hex(membuf.bytes, mem_offset(ptr), size)}
// TODO:        api with string length...
function        buf_set_string (buf, str, off)
{                   bytes_set_string(buf.bytes, str, off)}
function        mem_set_string (ptr, str)
{                   bytes_set_string(membuf.bytes, str, mem_offset(ptr))}
function        buf_set_hex (buf, str, off)
{                   bytes_set_hex(buf.bytes, str, off)}
function        bytes_get_string(bytes,off,size)
{                   let str = '';
                    off |= 0;
                    while (size >= 8)
                    {   size -= 8
                        str +=  code2char[bytes[off++]] + code2char[bytes[off++]]
                            +   code2char[bytes[off++]] + code2char[bytes[off++]]
                            +   code2char[bytes[off++]] + code2char[bytes[off++]]
                            +   code2char[bytes[off++]] + code2char[bytes[off++]] ;
                    }
                    while (size--)
                        str +=  code2char[bytes[off++]]
                    return str
}
function        bytes_get_hex(bytes,off,size)
{                   let str = ''
                    off |= 0
                    while (size >= 8)
                    {   size -= 8
                        str +=  byte2hex[bytes[off++]] + byte2hex[bytes[off++]]
                            +   byte2hex[bytes[off++]] + byte2hex[bytes[off++]]
                            +   byte2hex[bytes[off++]] + byte2hex[bytes[off++]]
                            +   byte2hex[bytes[off++]] + byte2hex[bytes[off++]] ;
                    }
                    while (size--)
                        str +=  byte2hex[bytes[off++]]
                    return str
}
function        bytes_set_string (bytes, str, off)
{                   let size = str.length
                    let i = (off|0)
                    let j = 0;
                    while (size >= 8)
                    {   size -= 8
                        bytes[i+j] = char2code[str[j++]];
                        bytes[i+j] = char2code[str[j++]];
                        bytes[i+j] = char2code[str[j++]];   bytes[i+j] = char2code[str[j++]];
                        bytes[i+j] = char2code[str[j++]];   bytes[i+j] = char2code[str[j++]];
                        bytes[i+j] = char2code[str[j++]];   bytes[i+j] = char2code[str[j++]];
                    }
                    while (size--)
                        bytes[i+j] = char2code[str[j++]];

}
function        bytes_set_hex (bytes, str, off)
{                   let size = str.length
                    let i = (off|0)
                    let j = 0;
                    while (size >= 8)
                    {   size -= 8
                        bytes[i+j] = hex2byte[str[j++]];   bytes[i+j] = hex2byte[str[j++]];
                        bytes[i+j] = hex2byte[str[j++]];   bytes[i+j] = hex2byte[str[j++]];
                        bytes[i+j] = hex2byte[str[j++]];   bytes[i+j] = hex2byte[str[j++]];
                        bytes[i+j] = hex2byte[str[j++]];   bytes[i+j] = hex2byte[str[j++]];
                    }
                    while (size--)
                        bytes[i+j] = hex2byte[str[j++]];
}
/* memory
============================================================*================*/
const           PAGE_BYTE_POW = 16
const           PAGE_BYTE_POWn = 16n
const           PAGE_BYTE_SIZE = 1 << PAGE_BYTE_POW; //64 << 10
                // let memory_pages = BigInt( C_MEM_INITIAL_PAGES )
let             memory_pages = C_MEM_INITIAL_PAGES
                // heap_size
let             memory_size = memory_pages << PAGE_BYTE_POW;
const           memory = new WebAssembly.Memory({initial: C_MEM_INITIAL_PAGES});
const           membuf = buf_from_buffer( memory.buffer )
let             memptr = membuf.addr;
let             memtop = 8n;
function        mem_offset(p)
{                   return n(p - memptr)
}
function        mem_top()
{                   return n(memtop)
}
function        mem_get_u64(p)      {  return buf_get_u64(membuf,mem_offset(p)); }
function        mem_get_i64(p)      {  return buf_get_i64(membuf,mem_offset(p)); }
function        mem_get_f64(p)      {  return buf_get_f64(membuf,mem_offset(p)); }
function        mem_get_f32(p)      {  return buf_get_f32(membuf,mem_offset(p)); }
function        mem_get_u32(p)      {  return buf_get_u32(membuf,mem_offset(p)); }
function        mem_get_i32(p)      {  return 0|buf_get_i32(membuf,mem_offset(p)); }
function        mem_get_u16(p)      {  return 0|buf_get_u16(membuf,mem_offset(p)); }
function        mem_get_i16(p)      {  return 0|buf_get_i16(membuf,mem_offset(p)); }
function        mem_get_i8 (p)      {  return 0|buf_get_i8(membuf,mem_offset(p)); }
function        mem_get_u8  (p)     {  return 0|buf_get_u8  (membuf,mem_offset(p)); }
function        mem_get_char(p)     {  return buf_get_char(membuf,mem_offset(p)); }

function        mem_set_u64 (p,v)   {  return buf_set_u64 (membuf,mem_offset(p),v); }
function        mem_set_i64 (p,v)   {  return buf_set_i64 (membuf,mem_offset(p),v); }
function        mem_set_f64 (p,v)   {  return buf_set_f64 (membuf,mem_offset(p),v); }
function        mem_set_f32 (p,v)   {  return buf_set_f32 (membuf,mem_offset(p),v); }
function        mem_set_u32 (p,v)   {  return buf_set_u32 (membuf,mem_offset(p),v); }
function        mem_set_i32 (p,v)   {  return buf_set_i32 (membuf,mem_offset(p),v|0); }
function        mem_set_u16 (p,v)   {  return buf_set_u16 (membuf,mem_offset(p),v|0); }
function        mem_set_i16 (p,v)   {  return buf_set_i16 (membuf,mem_offset(p),v|0); }
function        mem_set_i8  (p,v)   {  return buf_set_i8  (membuf,mem_offset(p),v|0); }
function        mem_set_u8  (p,v)   {  return buf_set_u8  (membuf,mem_offset(p),v|0); }
function        mem_set_char(p,v)   {  return buf_set_char(membuf,mem_offset(p),v); }

function        mem_grow(num)
{                   memory_pages += num
                    memory_size = memory_pages << PAGE_BYTE_POW;
                    memory.grow(Number(num))
                    mem_grow_handler()
}
function        mem_grow_handler()
{                   buf_attach(membuf, memory.buffer)
                    memptr = membuf.addr;
}
function        mem_grow_if_needed()
{   const           pages_over = page_idx_from_offset(mem_top()) + 1 - memory_pages
                    if (pages_over > 0)
                        mem_grow(pages_over)
}

/* allocation
============================================================*================*/
function        malloc(size)
{                   const offset = memtop // + memptr
                    memtop += N(size)
                    mem_grow_if_needed()
                    return offset + memptr
}
function        malloc_align(size, align)
{                   const offset = memtop // + memptr
                    memtop += N(size)
                    const pad = offset_pad64(memtop, N(align))
                    memtop += pad
                    mem_grow_if_needed()
                    return offset + pad + memptr
}
        function
page_idx_from_ptr(p)
    {   return page_idx_from_offset(p - memptr)
    }
        function
page_idx_from_offset(ofs)
    {   return ofs >> PAGE_BYTE_POW
    }
function        offset_pad(offset, align)
{                   return (-offset & (align - 1))
}
function        offset_pad64(offset, align)
{                   return (-offset & (align - 1n))
}

        // The number of padding bytes required to align the start of a data structure

        // For example, the padding to add to offset 0x59d for a 4-byte aligned structure is 3. The structure will then start at 0x5a0, which is a multiple of 4. However, when the alignment of offset is already equal to that of align, the second modulo in (align - (offset mod align)) mod align will return zero, therefore the original value is left unchanged.

        // aligntonext(p, r) works by adding an aligned increment, then clearing the r least significant bits of p. A possible implementation is
        function
alignto(p, bits)
    {   return (((p) >> bits) << bits)
    }
        function
aligntonext(p, bits)
    {   return alignto(((p) + (1 << bits) - 1), bits)
    }

        /* type def
        ============================================================*================*/
        // https://hps.vi4io.org/_media/teaching/wintersemester_2013_2014/epc-14-haase-svenhendrik-alignmentinc-paper.pdf
        // 0 ==> 128 bit or ,,,

        // ( size[4] | flags[2] | dimc[1] | ?[1] )[8]

const DISCRETE = UNSIGNED = 1 << 6;
const SIGNED = (1 << 5) | DISCRETE;
const AL_16  = 1 << 4;
const AL_8   = 1 << 3;
const AL_4   = 1 << 2;
const AL_2   = 1 << 1;
const AL_1   = 1;

const CONST = (1 << 9);
const BOOL = (1 << 8) | AL_1 | DISCRETE
const CHAR = (1 << 7) | AL_1 | DISCRETE; // SINGED???

        // const REF = (1 << 15) | AL_8 | DISCRETE;
const
    STR = (1 << 14) | AL_1,
    MEMBER = (1 << 13),
    PTR = (1 << 12),
    MAP = 1 << 11,
    DIM = 1 << 10,
    STRUCT = MAP

        // 8 flag bits for num of arr dimensions
        //      flags | ARR(2)  -> char x[][]
        function
ARR(num)
    {   return DIM | ((num & 255) << 16);
    }
        // top 8 flag bits for num ptr refs
        //      flags + REF + REF -> char** x
const
REF = 1 << 24,
TP_ALIGN_MASK = (AL_16 << 1) - 1,
T_HEAD_FLAGS_SHIFT = 38n,
T_HEAD_SIZE_MASK = (1n << T_HEAD_FLAGS_SHIFT) - 1n,
T_HEAD_FLAGS_MASK = -(T_HEAD_SIZE_MASK),
T_HEAD_DER_FLAGS_SHIFT = T_HEAD_FLAGS_SHIFT + 10n,
T_HEAD_DER_FLAGS_MASK = -(1n << T_HEAD_FLAGS_SHIFT) - 1n
;
const
CH  = CHAR,             char = _tp_alloc_primary(CH),
                        bool = _tp_alloc_primary(BOOL),
U8  = AL_1|UNSIGNED,    u8   = _tp_alloc_primary(U8),
I8  = AL_1|SIGNED,      i8   = _tp_alloc_primary(I8),
U16 = AL_2|UNSIGNED,    u16  = _tp_alloc_primary(U16),
I16 = AL_2|SIGNED,      i16  = _tp_alloc_primary(I16),
U32 = AL_4|UNSIGNED,    u32  = _tp_alloc_primary(U32),
I32 = AL_4|SIGNED,      i32  = _tp_alloc_primary(I32),
U64 = AL_8|UNSIGNED,    u64  = _tp_alloc_primary(U64),
I64 = AL_8|SIGNED,      i64  = _tp_alloc_primary(I64),
F32 = AL_4,             f32  = _tp_alloc_primary(F32),
F64 = AL_8,             f64  = _tp_alloc_primary(F64)
    ;

const
TP_BUILTIN =
{   char,  ch:char, // unsigned / signed
    // ich
    bool,

    u8,   uint8:u8,   uint8_t:u16,
    i8,    int8:i8,    int8_t:i16,
    u16, uint16:u16, uint16_t:u16, 'unsigned short int':u16, 'unsigned short':u16,
    i16,  int16:i16,  int16_t:i16,   'signed short int':i16,   'signed short':i16, 'short int':i16, 'short':i16,
    u32, uint32:u32, uint32_t:u32, 'unsigned int':u32,
    i32,  int32:i32,  int32_t:i32,   'signed int':i32,       'int':i32,
    u64, uint64:u64, uint64_t:u64, 'unsigned long int':u64,  'unsigned long':u64,
    i64,  int64:i64,  int64_t:i64,   'signed long int':i64,    'signed long':i64, 'long int':i64, 'long':i64,

    'uintptr_t':u64,'uintptr':u64,  'uptr':u64,
        'intptr_t':i64, 'intptr':i64,  'iptr':i64,
        'void*':i64,

    f32,  float:f32,
    f64, double:f64,
}
    const
SET_BY_TP =
    {   char  :buf_set_char,      [char]  :buf_set_char,
        // bool:buf_set_bool,    [bool]:buf_set_bool,
        u8  :buf_set_u8,      [u8]  :buf_set_u8,
        i8  :buf_set_i8,      [i8]  :buf_set_i8,
        u16 :buf_set_u16,     [u16] :buf_set_u16,
        i16 :buf_set_i16,     [i16] :buf_set_i16,
        u32 :buf_set_u32,     [u32] :buf_set_u32,
        i32 :buf_set_i32,     [i32] :buf_set_i32,
        u64 :buf_set_u64,     [u64] :buf_set_u64,
        i64 :buf_set_i64,     [i64] :buf_set_i64,
        f32 :buf_set_f32,     [f32] :buf_set_f32,
        f64 :buf_set_f64,     [f64] :buf_set_f64,
    }
    const
GET_BY_TP =
    {   char  :buf_get_char,      [char]  :buf_get_char,
        // bool:buf_get_bool,    [bool]:buf_get_bool,
        u8  :buf_get_u8,      [u8]  :buf_get_u8,
        i8  :buf_get_i8,      [i8]  :buf_get_i8,
        u16 :buf_get_u16,     [u16] :buf_get_u16,
        i16 :buf_get_i16,     [i16] :buf_get_i16,
        u32 :buf_get_u32,     [u32] :buf_get_u32,
        i32 :buf_get_i32,     [i32] :buf_get_i32,
        u64 :buf_get_u64,     [u64] :buf_get_u64,
        i64 :buf_get_i64,     [i64] :buf_get_i64,
        f32 :buf_get_f32,     [f32] :buf_get_f32,
        f64 :buf_get_f64,     [f64] :buf_get_f64,
    }
    const
SLICE_BY_TP =
    {   // bool:buf_get_bool,    [bool]:buf_get_bool,
        char:buf_slice_char,   [char] :buf_slice_char,
        u8  :buf_slice_u8,      [u8]  :buf_slice_u8,
        i8  :buf_slice_i8,      [i8]  :buf_slice_i8,
        u16 :buf_slice_u16,     [u16] :buf_slice_u16,
        i16 :buf_slice_i16,     [i16] :buf_slice_i16,
        u32 :buf_slice_u32,     [u32] :buf_slice_u32,
        i32 :buf_slice_i32,     [i32] :buf_slice_i32,
        u64 :buf_slice_u64,     [u64] :buf_slice_u64,
        i64 :buf_slice_i64,     [i64] :buf_slice_i64,
        f32 :buf_slice_f32,     [f32] :buf_slice_f32,
        f64 :buf_slice_f64,     [f64] :buf_slice_f64,
    }
    const
PLACE_BY_TP =
    {   // bool:buf_get_bool,    [bool]:buf_get_bool,
        char:buf_place_char,   [char] :buf_place_char,
        u8  :buf_place_u8,      [u8]  :buf_place_u8,
        i8  :buf_place_i8,      [i8]  :buf_place_i8,
        u16 :buf_place_u16,     [u16] :buf_place_u16,
        i16 :buf_place_i16,     [i16] :buf_place_i16,
        u32 :buf_place_u32,     [u32] :buf_place_u32,
        i32 :buf_place_i32,     [i32] :buf_place_i32,
        u64 :buf_place_u64,     [u64] :buf_place_u64,
        i64 :buf_place_i64,     [i64] :buf_place_i64,
        f32 :buf_place_f32,     [f32] :buf_place_f32,
        f64 :buf_place_f64,     [f64] :buf_place_f64,
    }
        // const CAST_BY_TP = {
        //     [u8]: function() {}
        // }

    function
cast_u8(x)
    {   return (x|0) & 255
    }
    function
cast_i8(x)   
    {   return (x|0) & 255
    }
    function
cast_u16(x) {
        return (x|0) & 65536
    }
    function
cast_i16(x) {
        return (x|0) & 65536 }
    function
cast_u32(x) {
        return (x|0)
    }
        function
cast_i32(x)
    {   return (x|0)
    }
    function
cast_u64(x)  
    {   return BigInt(x) 
    }
    function
cast_i64(x)  
    {   return BigInt(x) 
    }
    function 
cast_bool(x) 
    {   return (!!x)|0
    }
    function
cast_char(x) 
    {   return BigInt(x)
    }
        /*--------------------------------------------------------*/
    function
malloc_tp(num, tp)
    {   return malloc_align(sizeof(tp), alignof(tp) )
    }
    function
mem_get_tp(tp,p)
    {   return buf_get_tp   (membuf,tp,mem_offset(p));
    }
    function
mem_set_tp(tp,p,v)
    {   return buf_set_tp   (membuf,tp,mem_offset(p),v);
    }
    function
mem_slice_u8(p,len)
    {   return buf_slice_u8 (membuf, mem_offset(p),len);
    }
    function
mem_slice_tp(tp,p,len)
    {   return buf_slice_tp (membuf,tp,mem_offset(p),len);
    }
    function
mem_place_tp(tp,p,arr)
    {   return buf_place_tp(membuf,tp,mem_offset(p),arr);
    }
    function
mem_getin(tpath,p)
    {   return buf_getin (membuf,tpath,mem_offset(p));
    }
        function
mem_setin(tpath,p,val)
    {   return buf_setin (membuf,tpath,mem_offset(p),val);
    }
        function
buf_get_tp  (buf,tp,offset)
    {   return getterof(tp)(buf,offset);
    }
        function
buf_set_tp  (buf,tp,offset,val)
    {   return setterof(tp)(buf,offset,val);
    }
        function
buf_slice_tp(buf,tp,offset,len)
    {   return slicerof(tp)(buf,offset,len) ;
    }
        function
buf_place_tp(buf,tp,offset,arr)
    {   return placerof(tp)(buf,offset,arr) ;
    }
function    buf_getin(buf,tpath,offset)
{           const
        l = tpath.length >> 1
            let
        i = 0
            let
        ob = tpath[i]
        while (buf)
        {   if (type_is_complex(ob))
            {       const
                key_or_i = tpath[++i]
                    const
                [el_tp,el_offset] = complex_map.get(ob)(key_or_i)
                    // ptr
                if (el_offset < 0)
                {       const
                    i = key_or_i;
                        const
                    el_size = sizeof(el_tp)
                    offset = buf_deref_offset(buf, offset)
                    offset += i*el_size
                }
                    // arr|struct: contiguous contaiment
                else
                    offset += el_offset

                    // console.log(i,l,type_is_complex(el_tp))
                if (i == l || !type_is_complex(el_tp))
                    return getterof(el_tp)(buf,offset)
                else
                    buf = getterof(el_tp)(buf,offset)
            }
            else
                buf = null
        }
        throw new Error(`buf_getin ${{tpath,offset}}`)
    }
function    buf_setin(buf,tpath,offset,val)
{
    const       l = tpath.length >> 1
    let         i = 0
    let         ob = tpath[i]
                while(buf)
                {       // const align = alignof(ob)
                        // const size = sizeof(ob)
                    if (type_is_complex(ob))
                    {   const   key_or_i = tpath[++i]
                        const   [el_tp,el_offset] = complex_map.get(ob)(key_or_i)
                                // console.log({is:type_is_ptr(el_tp)},tpath,el_tp)
                                offset += el_offset
                                if (i == l || !type_is_complex(el_tp))
                                    return setterof(el_tp)(buf,offset,val)
                                else
                                    buf = getterof(el_tp)(buf,offset)
                    }
                    else
                        buf = null
                }
                throw new Error(`buf_setin tpath=${tpath} offset=${offset}}`)
}

const       typealias_map = map_new()
const       user_type_map = new MapAliased(typealias_map)
            // const ctypedefs = new MapAliased(typealias_map)
const       complex_getter_cache = map_new()
const       complex_setter_cache = map_new()

function    sizeof(tp)
{               return mem_get_u32( type(tp) )
}
function    alignof(tp)
{               return flagsof(tp) & TP_ALIGN_MASK
}
            // function nameof(tp) { return  }
function    flagsof(tp)
{               return mem_get_u32( type(tp) + 4n )
}
function    getterof(tp)
{               if (GET_BY_TP.hasOwnProperty(tp))
                    return GET_BY_TP[tp]
                if (type_is_ptr(tp))
                    return buf_get_i64
                if (type_is_complex(tp))
                {
                    const   size = sizeof(tp)
                    return  complex_getter_cache.has(size)
                        ?   complex_getter_cache.get(size)
                        :   complex_getter_cache.set(size,
                                function (buf,offset)
                                    {   return buf_slice_u8(buf,offset,size);
                                    })
                    ;
                }
                throw new Error(`setter of ${tp} not found`)
}
function    setterof(tp)
{               if (SET_BY_TP.hasOwnProperty(tp))
                    return SET_BY_TP[tp]
                if (type_is_ptr(tp))
                    return buf_set_i64
                if (type_is_complex(tp))
                {   const   size = sizeof(tp)
                    return  complex_setter_cache.has(size)
                        ?   complex_setter_cache.get(size)
                        :   complex_setter_cache.set(size,
                                function(buf,offset,arr)
                                    {   return buf_slice_u8(buf,offset,size)
                                    }
                            )
                }
                throw new Error(`setter of ${tp} not found`)
}
function    slicerof(tp)
{               if (SLICE_BY_TP.hasOwnProperty(tp)) return SLICE_BY_TP[tp]
                if (type_is_complex(tp)) return buf_slice_u8
                throw new Error(`slicer of ${tp} not found`)
}
function    placerof(tp)
{               if (PLACE_BY_TP.hasOwnProperty(tp)) return PLACE_BY_TP[tp]
                if (type_is_complex(tp)) return buf_place_u8
                throw new Error(`placer of ${tp} not found`)
}

function    type_is_complex(tp) {   return flagsof(tp) & (PTR|DIM|STRUCT);  }
function    type_is_ptr (tp)    {   return flagsof(tp) & PTR;               }
function    type_is_arr (tp)    {   return flagsof(tp) & DIM;               }
function    type_is_struct (tp) {   return flagsof(tp) & STRUCT;            }

function    type (tp)
{               if (isN(tp)) return tp
    const       t = TP_BUILTIN[tp]
                return t ? t : user_type(tp);
}
function    user_type (tp)
{               if (user_type_map.has(tp))
                    return user_type_map.get(tp)
                throw new Error(`(user) type ${tp} not defined`)
}
function    type_alias(name,tp)
{               return typealias_map.set(name, type(tp))
}
function    type_def(name,tp)
{               user_type_map.set(name, tp)
}
function    tp_alloc(size, flags)
{   const       tp = malloc_align(8,8);
                mem_set_u32(tp,size)
                mem_set_u32(tp+4n,flags)
                return tp
}
            // function tp_list_alloc(arr) {
            //     const len = arr.length
            //     const tp = malloc(len<<3,8);
            //     mem_place_i64(tp,arr)
            //     return tp
            // }
function    _tp_alloc_primary(flags)
{   const       tp = malloc_align(8,8);
    const       size = flags & TP_ALIGN_MASK;
                mem_set_u32(tp, size)
                mem_set_u32(tp+4n, flags)
                return tp
}
function    isN (num)
{               return typeof num === 'bigint'
}
function    N (num)
{               return BigInt(num)
}
function    n (num)
{               return Number(num)
}
            /*
            function type_head_enc(size, flags) { return N(size) | (N(flags) << T_HEAD_FLAGS_SHIFT) }
            function type_head_(size, flags) { return N(size) | (N(flags) << T_HEAD_FLAGS_SHIFT) }
            function primary_type(flags,names) {
                const t_head = type_head_enc(0, flags)
                return t_head
            }
            function type_is_primary(type) {
                return (type & (T_HEAD_SIZE_MASK|T_HEAD_DER_FLAGS_MASK)) == 0
                // return (type & T_HEAD_SIZE_MASK) == 0
            }
            */

const       complex_map = map_new()

function    offsetin(tp,i)
{               return complex_map.get(tp)(i)[1]
}
function    typein(tp,i)
{               return complex_map.get(tp)(i)[0]
}
function    ptr(el_type)
{   const       el_tp = type(el_type);
    const       tp_id = '*'+el_tp;
    return      user_type_map.has(tp_id)
        ?       user_type_map.get(tp_id)
        :       user_type_map.set(tp_id, _ptr_tp_alloc(el_tp))
}
function    _ptr_tp_alloc(el_tp)
{   const       tp = tp_alloc(8,8|PTR)
                complex_map.set(    // offset == -1 ==> ref jump
                    tp, function (i)
                        {  return [el_tp, -1]
                        }
                )
    return      tp
}
function    arr(el_type,dims)
{   const       el_tp = type(el_type)
    const       dimc = dims.length
    const       tp_id = ''+el_tp+'['+dimc+']'
    return      user_type_map.has(tp_id)
        ?       user_type_map.get(tp_id)
        :       user_type_map.set(tp_id,
                                  _arr_tp_alloc(el_tp,dims,dimc))
}
function    _arr_tp_alloc(el_tp,dims,dimc)
{
    const       el_size = sizeof(el_tp)
    const       size = el_size * array_product( dims )
    const       tp = tp_alloc(size, alignof(el_tp) | ARR(dimc))
                complex_map.set(
                    tp, function (i)
                        {   const   offset = i*el_size
                            return  [el_tp, offset]
                        }
                )
                return tp
}
function    padding(offset, align)
{               return (-offset & (align - 1))
}
function    is_aligned(offset, align)
{               return (offset + (align - 1)) & -align
}
function    struct(name, entries)
{   const       tp_id = "struct " + name
    const       l = entries.length
    const       keys = []
    const       offsets = new Uint32Array(l)
    const       types = new BigInt64Array(l)
    let         align = 0;
    let         offset = 0
    let         i = 0
    do          {   const   [tp, key] = entries[i]
                            types[i] = tp
                            keys[i] = key
                    const   mbr_al = alignof(tp)
                            if (mbr_al > align)
                                align = mbr_al
                    const   mbr_sz = sizeof(tp)
                    const   pad = padding(offset, mbr_al)
                            offset += pad
                            offsets[i] = offset
                            // console.log({i, pad, offset, sz:mbr_sz, mbr_al,al})
                            offset += mbr_sz
                }
                while (++i < l)

    const       size = offset + padding(offset, align)
    const       tp = tp_alloc(size, align|STRUCT)

                type_def(tp_id, tp)
                complex_map.set(
                    tp,
                    function (x)
                        {   const i = (x instanceof Array
                                ? x : indexof(x) );
                            return [types[i],offsets[i]]
                        }
                    )
                return tp

    function    indexof(key)
                {   return keys.indexOf(key)
                }
}


/*          fn frame calling convention
============================================================*================*/
const       FRAME_RET_I = 0
const       FRAME_CFG_I = 1<<3      // size... TODO:
const       FRAME_SYM_I = 2<<3
const       FRAME_PAD_LEN = 3<<3

function    frame_new()             {   return buf_from_size((8<<3) + FRAME_PAD_LEN); }
function    frame_mount(f,sym,argc) {   frame_set_sym(f,sym);  frame_set_argc(f,N(argc)); }
function    frame_set_sym(f,sym)    {   buf_set_i64(f, FRAME_SYM_I, sym); }
function    frame_set_argc(f, argc) {   buf_set_i64(f, FRAME_CFG_I, argc); }
function    frame_ret(f)            {   return buf_get_i64(f, FRAME_RET_I);}
function    frame_apply(f)          {   cframe_apply(f.buffer);}
function    frame_return(f)         {   cframe_return(f.buffer);}
function    framep_apply(ptr)       {   cframe_return(ptr);}
function    framep_return(ptr)      {   cframep_return(ptr);}
function    frame_set(f,typ,i,val)
{               return module.exports["buf_set_"+typ](
                    f, FRAME_PAD_LEN + (i<<3), val);        }
function    frame_get(f,typ,i,val)
{               return module.exports["buf_get_"+typ](
                    f, FRAME_PAD_LEN + (i<<3), val);        }
function    frame_slice(f,typ,i,l)
{               return module.exports["buf_slice_"+typ](
                    f, typ, FRAME_PAD_LEN + (i<<3), l );    }
function    frame_place(f,typ,i,l,val)
{               return module.exports["buf_place_"+typ](
                    f, typ, FRAME_PAD_LEN + (i<<3), l, val);}

        // const SUM = n.sym("sum")
        // const sum = n.frame(SUM, n.arg88_ret8)
        // sum.a0(100)
        // sum.a1(200)
        // sum.call()
        // val = sum.ret()
        // console.log(Symbol.for("d4") == Symbol.for("d4"))

        /*
        let code = 256, char2code = {}, code2char = [];
        while (code--) {
            char = String.fromCharCode(code);
            char2code[char] = code;
            code2char[code] = char;
        }
        console.log(`const char2code = ${JSON.stringify(char2code)};`);
        console.log(`const code2char = ${JSON.stringify(code2char)};`);
        */



// EXPORT()/*============================================================*======*/

module.exports = {

    NULL,

    bindings,
    c_malloc,
    c_malloc_align,
    c_malloc_str, c_deref_str,
    c_free,
    strcpy, strncpy,
    memcpy,
    xxhash64, xxhash32,
    crypto_hash256,
    crypto_box_enc, crypto_box_dec, crypto_box_verify,
    sym,
    call1,          call0,          callr,

    jit_alloc,
    jit_alloc_wdefaults,
    jit_free,
    jit_sym,
    jit_string,
    jit_file,
    jit_outfile,
    jit_compile,
    jit_run,

    map_new,
    map_aliased_new,
    map_aliased_new_with,


    frame_new,
    frame_mount,
    frame_set_sym,
    frame_set_argc,
    frame_set,
    frame_get,
    frame_slice,
    frame_place,
    frame_ret,
    frame_apply,            framep_apply,
    frame_return,           framep_return,


    malloc_tp,
    mem_top,
    mem_get_tp,     buf_get_tp,
    mem_set_tp,     buf_set_tp,
    mem_slice_tp,   buf_slice_tp,
    mem_place_tp,   buf_place_tp,
    mem_getin,      buf_getin,
    mem_setin,      buf_setin,
    mem_slice_u8,   //buf_slice_u8,
    N,              n,

    type,
    typein,
    type_alias,
    type_is_complex,
    type_is_ptr,

    sizeof,
    alignof,
    flagsof,
    offsetin,

    ptr,
    arr,
    struct,
    char,
    bool,
    u8,         i8,
    u16,        i16,
    u32,        i32,
    u64,        i64,
    f32,
    f64,


    malloc,
    malloc_align,
    page_idx_from_ptr,
    page_idx_from_offset,



    mem_offset,
    mem_grow,
    mem_grow_handler,
    mem_grow_if_needed,
    mem_get_u64,        mem_set_u64,
    mem_get_i64,        mem_set_i64,
    mem_get_f64,        mem_set_f64,
    mem_get_f32,        mem_set_f32,
    mem_get_u32,        mem_set_u32,
    mem_get_i32,        mem_set_i32,
    mem_get_u16,        mem_set_u16,
    mem_get_i16,        mem_set_i16,
    mem_get_i8,         mem_set_i8,
    mem_get_u8,         mem_set_u8,
    mem_get_char,       mem_set_char,

    
    buf_from_size,
    buf_from_bytes,
    buf_from_buffer,
    buf_attach,

    buf_ref,            arrbuf_ref,

    buf_get_u64,        buf_set_u64,
    buf_get_i64,        buf_set_i64,
    buf_get_f64,        buf_set_f64,
    buf_get_f32,        buf_set_f32,
    buf_get_u32,        buf_set_u32,
    buf_get_i32,        buf_set_i32,
    buf_get_u16,        buf_set_u16,
    buf_get_i16,        buf_set_i16,
    buf_get_i8,         buf_set_i8,
    buf_get_u8,         buf_set_u8,
    buf_get_char,       buf_set_char,

    buf_set_string,     bytes_set_string,   mem_set_string,
    buf_get_string,     bytes_get_string,   mem_get_string,
mem_get_hex,
    buf_slice_char,     bytes_slice_char,
    buf_slice_hex,      bytes_slice_hex,
}


/*          utils
============================================================*================*/
function    ASSERT(x,msg)
{               if (!x)         throw new Error(`ERROR: ${msg}`)
}
function    LOG(...msgs)
{               console.log(...msgs)
}
function    TEST_skip () {}
function    TEST(n,cb)
{   LOG         ("\bTEST:",n)
    let         count = 0
    let         errcount = 0
                cb(YAY, NAY)
    LOG         ("--->",count - errcount,"/",count,"\n")

                function YAY(x,msg)
                {   ++count
                    if (!x)     ERR('!YAY',x,msg)
                    // else     LOG('    ', x)
                }
                function NAY(x,msg)
                {   ++count
                    if (x)      ERR(`${count} !NAY: ${x}, ${msg}`)
                }
                function ERR(t,x,m)
                {   ++errcount
                    console.error(`${count} !YAY ${x}, ${msg}`)
                }
}




            global.__test_call_from_c = __test_call_from_c;

function    __test_call_from_c(num)
{               console.log("   __test_call_from_c <-", num);
                return num + 2;
}