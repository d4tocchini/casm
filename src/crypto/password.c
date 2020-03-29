

// https://github.com/jedisct1/libhydrogen/wiki/Password-hashing

sc_ch*  SPECIALS = "!@$*_+-?";
#define SPECIALS_MAX    7
#define SPECIALS_LEN    8
#define LOWERCASE_MIN   'a'
#define LOWERCASE_MAX   'z'
#define LOWERCASE_LEN   26
#define LOWERCASE_0     (SPECIALS_LEN)
#define UPPERCASE_MIN   'A'
#define UPPERCASE_MAX   'Z'
#define UPPERCASE_LEN   26
#define UPPERCASE_0     (SPECIALS_LEN + LOWERCASE_LEN)
#define DIGIT_MIN       '0'
#define DIGIT_MAX       '9'
#define DIGIT_LEN       10
#define DIGIT_0         (SPECIALS_LEN + LOWERCASE_LEN + UPPERCASE_LEN)
#define PASSCH_LEN      (SPECIALS_LEN + LOWERCASE_LEN + UPPERCASE_LEN + DIGIT_LEN)
#define PASSCH_MAX      (PASSCH_LEN - 1)

ch crypto_pass_gen_char()
{
    int idx = CRYPTO_RANDOM_U32_MAX(PASSCH_MAX);
    if (idx >= DIGIT_0)
        return DIGIT_MIN + (idx - DIGIT_0);
    if (idx >= UPPERCASE_0)
        return UPPERCASE_MIN + (idx - UPPERCASE_0);
    if (idx >= LOWERCASE_0)
        return LOWERCASE_MIN + (idx - LOWERCASE_0);
    return SPECIALS[idx];
}
void crypto_pass_gen(u8 *buf, int len)
{
    if (len < 8)
        len = 8;
    buf[0] = CRYPTO_RANDOM_U32_MINMAX(LOWERCASE_MIN, LOWERCASE_MAX);
    buf[1] = CRYPTO_RANDOM_U32_MINMAX(LOWERCASE_MIN, LOWERCASE_MAX);
    buf[2] = CRYPTO_RANDOM_U32_MINMAX(UPPERCASE_MIN, UPPERCASE_MAX);
    buf[3] = CRYPTO_RANDOM_U32_MINMAX(UPPERCASE_MIN, UPPERCASE_MAX);
    buf[4] = CRYPTO_RANDOM_U32_MINMAX(DIGIT_MIN, DIGIT_MAX);
    buf[5] = CRYPTO_RANDOM_U32_MINMAX(DIGIT_MIN, DIGIT_MAX);
    buf[6] = SPECIALS[CRYPTO_RANDOM_U32_MAX(SPECIALS_MAX)];
    buf[7] = SPECIALS[CRYPTO_RANDOM_U32_MAX(SPECIALS_MAX)];
    int i = 8;
    while (i < len)
        buf[i++] = crypto_pass_gen_char();
    crypto_shuffle8(buf, len);
}
// buf = new Uint8Array(10); casm.c.bindings.crypto_pass_gen(casm.c.bindings.arrbuf_ref(buf.buffer), 10)

