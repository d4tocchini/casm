
typedef sx_hashtbl map32_t;

void map32_create(const void **map, int capacity)
{
    *map = sx_hashtbl_create(SX_ALLOCATOR, capacity);
}

void map32_destroy(map32_t *map)
{
    sx_hashtbl_destroy(map, SX_ALLOCATOR);
}

void map32_capacity(const map32_t *map, int *out)
{
    *out = map->capacity;
}

void map32_size(const map32_t *map, int *out)
{
    *out = map->count;
}

void map32_is_full(const map32_t *map, bool *out)
{
    *out = map->capacity == map->count;
}

void map32_set(map32_t *map, uint32_t key, int val, int *idx)
{
    *idx = sx_hashtbl_add(map, key, val);
}

void map32_get(map32_t *map, uint32_t key, int *out)
{
    *out = sx_hashtbl_find(map, key);
}

void map32_at(const map32_t *map, int idx, int *out)
{
    *out = sx_hashtbl_get(map, idx);
}

void map32_clear(map32_t *map)
{
    sx_hashtbl_clear(map);
}

// void map32(int64_t **p)
// {
//     static const in64_t SYM[] = {
//         &map32_create,
//         &map32_destroy,
//         &map32_capacity,
//         &map32_size,
//         &map32_is_full,
//         &map32_set,
//         &map32_get,
//         &map32_at,
//         &map32_clear
//     };
//     void (*) SYM[p[0]]
// }

// void gmap32_grow(map32_t *map)
// {
//     sx_hashtbl_destroy(map, SX_ALLOCATOR);
// }