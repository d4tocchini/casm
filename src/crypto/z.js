
module.exports = function (c,mod) {

/*          import core deps
============================================================*================*/
const       {   sym,
                malloc_align,
                frame_new,frame_mount,frame_set,frame_apply
            } = c;

/*          libhydrogen crypto
============================================================*================*/
PUB`
            hydro_enc_sizeof, hydro_set_ctx,
`
const       hydro_secretbox_CONTEXTBYTES = 8
const       hydro_secretbox_HEADERBYTES = (20 + 16)
const       hydro_secretbox_KEYBYTES = 32
const       hydro_secretbox_PROBEBYTES = 16
const       hydro_keygen_p = sym("hydro_secretbox_keygen")
const       hydro_encrypt_p = sym("hydro_secretbox_encrypt")
const       hydro_decrypt_p = sym("hydro_secretbox_decrypt")

const       hydro_f = frame_new();

            // frame("hydro_secretbox_keygen", [
            //     arr(u8, [32])
            // ])

function    secretbox_set_ctx ()
{

}
function    secretbox_sizeof_encrypt(msg, mlen)
{               return hydro_secretbox_HEADERBYTES + mlen
}

pub`        secretbox_key_malloc,
            secretbox_keygen,
`
function    secretbox_key_malloc()
{   const       ptr = malloc_align(
                    hydro_secretbox_KEYBYTES, hydro_secretbox_KEYBYTES);
                return ptr
}
function    secretbox_keygen(u8x32)
{   const       key_p = malloc_align(
                    hydro_secretbox_KEYBYTES, hydro_secretbox_KEYBYTES);
    const       key_i = mem_offset(key_p);

                frame_mount(f, hydro_keygen_p, 1);
                frame_set(f,'i64',0,key_p);
                frame_apply(f)

                console.log("!!!! " +mem_get_hex(key_i, hydro_secretbox_KEYBYTES));
                process.exit(0);
                return key_p
}
function    secretbox_encrypt(msg, mlen)
{   const       key_p = malloc_align(hydro_secretbox_KEYBYTES, 32);
                frame_mount(f, encrypt_p, 4);
                frame_set(f,'i64',0,key_p);
                frame_apply(f)
                return key_p
}
function    secretbox_decrypt(msg, msg_len)
{   const       key_p = malloc_align(hydro_secretbox_KEYBYTES, 32);
                frame_mount(f, encrypt_p, 4);
                frame_set(f,'i64',0,key_p);
                frame_apply(f)
                return key_p
}

TEST        ("hydro crypto", function(Y,N)
{   let         key_p = secretbox_key_malloc()
                Y(key_p)
                console.log(hydro_keygen_p)
                process.exit(0)
}           )
    // function    probe_create(
    //                 cipher,
    //                 message)
    //             {
    //             }
//     const           seckey_b = buf_from_size(hydro_secretbox_KEYBYTES)

// function        crypto_seckey_new() {
//     const               buf =
//                 }

//         int
//         hydro_secretbox_encrypt(
//             uint8_t *c, // hydro_secretbox_HEADERBYTES + mlen
//             const void *m_, size_t mlen,
//             uint64_t msg_id,
//             const char ctx[8],//hydro_secretbox_CONTEXTBYTES],
//             const uint8_t key[32])//hydro_secretbox_KEYBYTES]);
//         ;
//         int
//         hydro_secretbox_decrypt(
//             void *m_,         // out message buffer
//             const uint8_t *c,  size_t clen,  // in cipher len
//             uint64_t msg_id,  // message counter, if not needed use 0
//             const char ctx[8],//hydro_secretbox_CONTEXTBYTES],
//             const uint8_t key[32])//hydro_secretbox_KEYBYTES])
//         ;   //__attribute__((warn_unused_result));
//         void
//         hydro_secretbox_probe_create(
//             uint8_t probe[16],//hydro_secretbox_PROBEBYTES],
//             const uint8_t *c, size_t c_len,
//             const char    ctx[8],//hydro_secretbox_CONTEXTBYTES],
//             const uint8_t key[32])//hydro_secretbox_KEYBYTES])
//         ;
//         int
//         hydro_secretbox_probe_verify( // returns 0 on success, and -1 if the probe didn't pass verification.
//             uint8_t probe[16],//hydro_secretbox_PROBEBYTES],
//             const uint8_t *c, size_t c_len,
//             const char    ctx[8],//hydro_secretbox_CONTEXTBYTES],
//             const uint8_t key[32])//hydro_secretbox_KEYBYTES])
//         ;   //__attribute__((warn_unused_result));

//         const
//         secretbox_HEADERBYTES = 20 + 16
//         const
//         key32 = Buffer.allocUnsafe(32);
//         const
//         keygen = c.frame(
//             c.void,
//             "hydro_secretbox_keygen",
//                     function(a)
//                 {   a.array.u8(32)
//                 }
//         )
//         const
//         encrypt = c.frame(
//             c.int,
//             "hydro_secretbox_encrypt",
//             [ c.array.u8(32) ]
//         )
//         const
//         encrypt = c.frame(`
//             int
//             hydro_secretbox_encrypt(
//                 u8   * cipher,
//                 void * msg,
//                 size_t msg_len,
//                 u64    msg_id,
//                 char * ctx,
//                 u8     key[32]
//             );
//             struct things {
//                 uint64_t a_number;
//                 char a_char;
//                 char input[61];
//                 char output[61];
//                 int operations;
//             };
//         `)
//         const
//         encrypt = c.frame(
//             c.sym("hydro_secretbox_encrypt"),
//                     function (arg, ret)
//                 {   arg("cipher", u8_ptr())
//                     arg("msg", void_ptr())
//                     arg("msg_len", size_t())
//                     arg("msg_id", u64())
//                     arg("ctx", str(8))
//                     arg("key", u8arr(32))
//                 }
//         )

//         keygen.key.malloc()
//         keygen.call()


//         c.call_1(keygen, key32);

//                 function
//         encrypt_str(msg)
//             {   const
//                 mlen = msg.length
//                 const
//                 cipher = Buffer.allocUnsafe();
//                 c
//             }
//     }

/*          publish public api
============================================================*================*/
return  {
            secretbox_key_malloc,secretbox_keygen,
            secretbox_encrypt,secretbox_sizeof_encrypt,
            secretbox_decrypt,
            secretbox_set_ctx,
}


/*          utils
============================================================*================*/
function    ASSERT(x,msg)
{               if (!x) throw new Error(`ERROR: ${msg}`)
}
function    LOG(...msgs)
{               console.log(...msgs)
}
function    TEST_skip ()
{
}
function    TEST(n,cb)
{   LOG         ("\bTEST:",n)
    let         count = 0
    let         errcount = 0
                cb(YAY, NAY)
    LOG         ("--->",count - errcount,"/",count,"\n")

    function    YAY(x,msg)
    {
                    ++count
                    if (!x) ERR('!YAY',x,msg)
                    // else LOG('    ', x)
    }
    function    NAY(x,msg)
    {
                    ++count
                    if (x) ERR(`${count} !NAY: ${x}, ${msg}`)
    }
    function    ERR(t,x,m)
    {
                    ++errcount
                    console.error(`${count} !YAY ${x}, ${msg}`)
    }
}


}

