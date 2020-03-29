
/*              crypto
==============================================================================*/
#include        "hydrogen.h"

#define         CRYPTO_KEY_SIZE hydro_hash_KEYBYTES
                    // == hydro_secretbox_KEYBYTES == 32
sc_ch*          CRYPTO_CTX = "ÆXILE";

int             crypto_hash256(
    u8*             out,
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    return hydro_hash_hash(out, 32, bin, bin_sz, CRYPTO_CTX, _key);
}
int             crypto_hash128(
    u8*             out,
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    return hydro_hash_hash(out, 16, bin, bin_sz, CRYPTO_CTX, _key);
}
u64             crypto_hash64(
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    u64 h[2];
                    hydro_hash_hash(h, 16, bin, bin_sz, CRYPTO_CTX, _key);
                    return h[0]^h[1];
                    // return *((u64 *)h);
}
u32             crypto_hash32(
    c_void*         bin,
    sz_t            bin_sz,
    c_u8*           _key) // NULL
{
                    u32 h[4];
                    hydro_hash_hash(h, 16, bin, bin_sz, CRYPTO_CTX, _key);
                    return h[0]^h[1]^h[2]^h[3];
}
bool            crypto_eq(
    c_void*         b1,
    c_void*         b2,
    sz_t            sz)
{                   //  - returns 1 if the len bytes pointed to by b1_ match the len bytes pointed to by b2_. Otherwise, it returns 0.
                    //  - returns 0 if b1_ and b2_ are the exact same address, as this is likely to only happen on misuse.
                    return hydro_equal(b1, b2, sz);
}

int             crypto_box_enc(
    u8*             out,
    c_void*         msg,
    sz_t            msg_sz,
    c_u8            key[CRYPTO_KEY_SIZE],
    u64             _id) // 0
{                   //  * The hydro_secretbox_encrypt() function encrypts a message m_ of length mlen bytes using a context ctx, a secret key key and a message counter msg_id.
                    //  * It puts the ciphertext whose length is hydro_secretbox_HEADERBYTES + mlen into c.
                    //  * The header includes an automatically-generated 160-bit nonce as well as a 128-bit authentication tag.
                    //  * msg_id * is an optional message tag. For example, if 3 messages are sent to the same recipient using the same key, these messages can sequentially use 0, 1 and 2 as identifiers.
                    //      * If this mechanism is not required by an application, using a constant msg_id such as 0 is also totally fine. Message identifiers are optional and do not have to be unique.
                    return hydro_secretbox_encrypt(out, msg, msg_sz, _id, CRYPTO_CTX, key);
}
int             crypto_box_dec(
    u8*             out,
    c_void*         box,
    sz_t            box_sz,
    c_u8            key[CRYPTO_KEY_SIZE],
    u64             _id)
{                   //  --> yay -> 0, nay -> -1
                    return hydro_secretbox_decrypt(out, box, box_sz, _id, CRYPTO_CTX, key);
}
int             crypto_box_verify(
    c_u8*           box,
    sz_t            box_sz,
    c_u8            key[CRYPTO_KEY_SIZE])
{                   //  --> yay -> 0, nay -> -1
    u8              probe[hydro_secretbox_PROBEBYTES];
                    hydro_secretbox_probe_create(probe, box, box_sz, CRYPTO_CTX, key);
                    return hydro_secretbox_probe_verify(probe, box, box_sz, CRYPTO_CTX, key);
}

int             hex2bin_parse(
    u8*             bin,
    sz_t            bin_maxlen,
    c_ch*           hex,
    sz_t            hex_len,
    c_ch*           _ignore,    // NULL
    c_ch**          _hex_end_p) // NULL
{                   /*
                    - parses a hexadecimal string hex & converts it to a byte sequence.
                    - The parser stops when a non-hexadecimal, non-ignored character is found or when bin_maxlen bytes have been written.
                    -> -1
                        if more than bin_maxlen bytes would be required to store the parsed string.
                    -> number of written bytes
                        on success and sets hex_end, if it is not NULL, to a pointer to the character following the last parsed character.
                    :hex
                        - does not have to be nul terminated, as the number of characters to parse is supplied via the hex_len parameter.
                    :ignore
                        is a string of characters to skip. For example, the string ": " allows columns and spaces to be present at any locations in the hexadecimal string. These characters will just be ignored. As a result, "69:FC", "69 FC", "69 : FC" and "69FC" will be valid inputs, and will produce the same output.
                        - can be set to NULL in order to disallow any non-hexadecimal character.
                    :bin_maxlen
                        is the maximum number of bytes to put into bin.

                    hydro_bin2hex: hex_maxlen
                        - the maximum number of bytes that the function is allowed to write starting at hex. It should be at least bin_len * 2 + 1.
                    */
                    return hydro_hex2bin( bin, (hex_len << 1U) + 1U, hex, hex_len,
                        _ignore, _hex_end_p);

}
ch*             hex2bin(
    u8*             out,
    c_ch*           hex,
    sz_t            hex_sz)
{
                    return hex2bin_parse(out, (hex_sz >> 1U), hex, hex_sz,
                    NULL, NULL);
}
ch*             bin2hex(
    ch*             hex,
    c_u8*           bin,
    sz_t            bin_sz)
{                   //  - converts bin_len bytes stored at bin into a hexadecimal string w/ nul byte (\0) terminator.
                    //  - returns hex on success, or NULL on overflow. It evaluates in constant time for a given size.
                    //  : hex_maxlen
                    //      - the maximum number of bytes that the function is allowed to write starting at hex. It should be at least bin_len * 2 + 1.
                    return hydro_bin2hex(hex, (bin_sz << 1U) + 1U, bin, bin_sz);

}

/*============================================================================*/
