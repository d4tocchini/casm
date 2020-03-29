
/*              xm
==============================================================================*/
#include        "ems.h"

int EMSpush(int mmapID, EMSvalueType *value, int64_t* timer);
int EMSpop(int mmapID, EMSvalueType *returnValue, int64_t* timer);
int EMSenqueue(int mmapID, EMSvalueType *value, int64_t* timer);
int EMSdequeue(int mmapID, EMSvalueType *returnValue, int64_t* timer);

// #define         XM_EXPORT()\
//                     SYM_EXPORT(xm_destroy)                                  \
//                     SYM_EXPORT(xm_read)         SYM_EXPORT(xm_write)        \
//                     SYM_EXPORT(xm_read_fe)      SYM_EXPORT(xm_write_xf)     \
//                     SYM_EXPORT(xm_read_ff)      SYM_EXPORT(xm_write_xe)     \
//                     SYM_EXPORT(xm_write_ef)                                 \
//                     SYM_EXPORT(xm_read_swmr)    SYM_EXPORT(xm_release_swmr) \
//                     SYM_EXPORT(xm_key2idx)      SYM_EXPORT(xm_idx2key)      \
//                     SYM_EXPORT(xm_await_file)

                /*
                struct xmobj {
                    const char *path;
                    int         id;
                    uint32_t flags;
                    uint32_t    len;
                    uint32_t    size;
                    // EMSvalueType *fill_val
                };
                */

typedef struct  xmtype_s
{
    sz_t                length; // for JSON, strs + buf
    void *              value;
    uchar               type;
}               xmtype_t;

// xmtype_t        xmtype;

JS_METHOD(      js_ems_open)
{
                    NAPI_ARGV(1)
                    JS_ARGV_I64(filepath,0)
                    int xmid = ems_open(filepath);
                    JS_RET_I32(xmid)
}
JS_METHOD(      js_ems_create)
{
                    NAPI_ARGV(4)
                    JS_ARGV_U32(max_len, 0)
                    JS_ARGV_U32(max_sz, 1)
                    JS_ARGV_I64(filepath,2)
                    JS_ARGV_U32(flags, 3)
                    int xmid = ems_create(max_len, max_sz, filepath, flags);
                    JS_RET_I32(xmid)
}
JS_METHOD(      js_ems_destroy)
{
                    NAPI_ARGV(2)
                    JS_ARGV_I32(xmid, 0)
                    JS_ARGV_I32(do_unlink, 1)
                    int did = ems_destroy(xmid, do_unlink);
                    JS_RET_I32(did)
}

/*              xm :: r/w
                -----------------------------------------------------------------------*/
bool            xm_read(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_ANY, EMS_TAG_ANY, NULL);
}
bool            xm_write(int mmapID, EMSvalueType *key, EMSvalueType *value)
{
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_ANY, NULL);
}
bool            xm_read_fe(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{                   //  read when full and leave empty
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_FULL, EMS_TAG_EMPTY , NULL);
}
bool            xm_read_ff(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{                   //  read when full and leave Full
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_FULL, EMS_TAG_FULL, NULL);
}
bool            xm_write_xf(int mmapID, EMSvalueType *key, EMSvalueType *value)
{                   //  WriteXF
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_FULL, NULL);
}
bool            xm_write_xe(int mmapID, EMSvalueType *key, EMSvalueType *value) {
                    //  WriteXE
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_ANY, EMS_TAG_EMPTY, NULL);
}
bool            xm_write_ef(int mmapID, EMSvalueType *key, EMSvalueType *value)
{                   //  write when empty & leave full
                    return EMSwriteUsingTags(mmapID, key, value, EMS_TAG_EMPTY, EMS_TAG_FULL, NULL);
}

/*              xm :: multiple readers-single writer lock
                -----------------------------------------------------------------------*/
bool            xm_read_swmr(const int mmapID, EMSvalueType *key, EMSvalueType *returnValue)
{                   //  Read under multiple readers-single writer lock
                    return EMSreadUsingTags(mmapID, key, returnValue, EMS_TAG_RW_LOCK, EMS_TAG_RW_LOCK, NULL);
}
int             xm_release_swmr(
                    //  Decrement the reference count of the multiple readers-single writer lock
    const int       mmapID,
    EMSvalueType*   key)
{
}

/*              xm :: utils
                --------------------------------------------------------------*/
i64             xm_key2idx(
    void*           emsBuf,
    EMSvalueType*   key,
    bool            is_mapped)
{                   //  Convert any type of key to an index
}
bool            xm_idx2key(
    int             mmapID,
    i64             idx,
    EMSvalueType*   key)
{
}
// void            xm_await_file(
//     c_ch*           path)
// {
//                     return ems_await_owner(path);
// }



/*              xm :: frame
                --------------------------------------------------------------
spsc_q*         xm_job_q;
typedef struct  xm_job_item {
    int id;
}               xm_job_item;

void            xm_frame()
{
                    while (spsc_q_consume(xm_job_q, &job_itm)) {

                    }
}

void            xm_frame_setup()
{
                    xm_job_q = spsc_q_create(sizeof(xm_job_item), 10);
}

// static int      xm_job_
*/

/*              xm :: setup
                --------------------------------------------------------------*/
void            __xm_setup(napi_env env, napi_value exports)
{
                    JS_EXPORT_FN_AS(js_ems_open,     ems_open)
                    JS_EXPORT_FN_AS(js_ems_create,   ems_create)
                    JS_EXPORT_FN_AS(js_ems_destroy,  ems_destroy)
}


/*============================================================================*/

