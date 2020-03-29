
// https://github.com/jedisct1/libhydrogen/wiki/Random-numbers

#define hydro_random_SEEDBYTES 32

#define CRYPTO_RANDOM_U32_MINMAX(min, max) \
    (min + hydro_random_uniform(max + 1 - min))

#define CRYPTO_RANDOM_U32_MAX(max) \
    (hydro_random_uniform(max + 1))

void crypto_u32(u32 *out)
{
    *out = hydro_random_u32();
}

void crypto_u32_max(u32 *out, const u32 max)
{
    *out = CRYPTO_RANDOM_U32_MAX(max);
}

void crypto_u32_minmax(u32 *out, const u32 min, const u32 max)
{
    *out = CRYPTO_RANDOM_U32_MINMAX(min, max);
}

// void crypto_sample_u32(u32 *out, u32 *buf, const u32 min, const u32 max)
// {
//     *out = buf[CRYPTO_RANDOM_U32_MINMAX(min, max)];
// }


void crypto_memset_seed256(void *buf, size_t len, const u8 seed[hydro_random_SEEDBYTES])
    // The hydro_random_buf_deterministic() function stores len bytes into buf indistinguishable from random bytes without knowing seed. For a given seed, this function will always output the same sequence. This function is mainly useful for writing tests.
{
    hydro_random_buf_deterministic(buf, len, seed);
}

#define _CRYPTO_SWAP(TYPE, a, b)\
    TYPE temp = buf[i];\
    buf[i] = buf[j];\
    buf[j] = temp;

void crypto_shuffle32(u32 *buf, int len)
    // Fisher–Yates shuffle Algorithm
    // Start from the last element and swap one by one. We don't need to run for the first element that's why i > 0
{
    for (int i = len-1; i > 0; i--)
    {
        int j = CRYPTO_RANDOM_U32_MAX(i);
        _CRYPTO_SWAP(u32, i, j)
    }
}

void crypto_shuffle8(u8 *buf, int len)
    // Fisher–Yates
{
    for (int i = len-1; i > 0; i--)
    {
        int j = CRYPTO_RANDOM_U32_MAX(i);
        _CRYPTO_SWAP(u8, i, j)
    }
}

void crypto_range32(i32 *buf, i32 min, i32 max)
    // Fisher–Yates shuffle Algorithm
{
    i32 i = 0;
    i32 len = max - min + 1;
    while (i < len) {
        buf[i] = min + i++;
    }
    for (i = len-1; i > 0; i--)
    {
        i32 j = CRYPTO_RANDOM_U32_MAX(i);
        // Swap arr[i] with the element at random index
        _CRYPTO_SWAP(u32, i, j)
    }
}
