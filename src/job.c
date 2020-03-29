
#include        "sx/atomic.h"
#include        "sx/os.h"
#include        "sx/threads.h"

// Mutex, for long-time data locks, for short-time locks use sx_lock_t in atomics.h
typedef sx_mutex            mtx;

#define mtx_init(mtx)           sx_mutex_init((mtx)) // void sx_mutex_init(sx_mutex* mutex);
#define mtx_release(mtx)        sx_mutex_release((mtx)) // void sx_mutex_release(sx_mutex* mutex);
#define mtx_lock(mtx)           sx_mutex_lock((mtx)) // void sx_mutex_lock(sx_mutex* mutex);
#define mtx_unlock(mtx)         sx_mutex_unlock(mtx) // void sx_mutex_unlock(sx_mutex* mutex);
#define mtx_trylock(mtx)        sx_mutex_trylock((mtx)) //bool sx_mutex_trylock(sx_mutex* mutex);


// Semaphore
typedef sx_sem              mtsem;

#define mtsem_init(sem_p)               sx_semaphore_init((sem_p))    // -> void sx_semaphore_init(sx_sem* sem);
#define mtsem_release(sem_p)            sx_semaphore_release((sem_p))    // -> void sx_semaphore_release(sx_sem* sem);
#define mtsem_post(sem_p, int_count)    sx_semaphore_post((sem_p), int_count)    // -> void sx_semaphore_post(sx_sem* sem, int count sx_default(1));
#define mtsem_wait(sem_p, int_msecs)    sx_semaphore_wait((sem_p), int_msecs)    // -> bool sx_semaphore_wait(sx_sem* sem, int msecs sx_default(-1));


// Lock-Free single-producer/single-consumer self-contained-data queue
// Reference:
//      http://www.drdobbs.com/parallel/writing-lock-free-code-a-corrected-queue/210604448?pgno=1

#define SPSC_Q_ALLOCATOR SX_ALLOCATOR

typedef sx_queue_spsc       spsc_q;

spsc_q* spsc_q_create (int item_sz, int capacity)             { return sx_queue_spsc_create(SPSC_Q_ALLOCATOR, item_sz, capacity); }
void    spsc_q_destroy    (spsc_q* q)                         { sx_queue_spsc_destroy(q, SPSC_Q_ALLOCATOR); }
bool    spsc_q_produce    (spsc_q* q, const void* data)       { return sx_queue_spsc_produce(q,data); }
bool    spsc_q_consume    (spsc_q* q, void* data)             { return sx_queue_spsc_consume(q,data); }
bool    spsc_q_grow       (spsc_q* q, const sx_alloc* alloc)  { return sx_queue_spsc_grow(q,SPSC_Q_ALLOCATOR); }
bool    spsc_q_full (const spsc_q* q)                   { return sx_queue_spsc_full(q); }


// jobs
#include        "sx/jobs.h"


#define JOB_ALLOCATOR SX_ALLOCATOR

// Job priority, higher priority jobs will run sooner
#define JOB_PRIO_HIGH               SX_JOB_PRIORITY_HIGH // = 0
#define JOB_PRIO_MED                SX_JOB_PRIORITY_NORMAL
#define JOB_PRIO_LOW                SX_JOB_PRIORITY_LOW
#define JOB_PRIO_COUNT              SX_JOB_PRIORITY_COUNT

typedef sx_job_t                    job_h;
typedef sx_job_cb                   job_cb_t;
typedef sx_job_thread_init_cb       job_ctor;
typedef sx_job_thread_shutdown_cb   job_dtor;
typedef sx_job_context              job_ctx_t;
typedef sx_job_context_desc         job_desc_t;
typedef sx_job_priority             job_prio_t;
                    // Job description, required for submitting jobs
                // int num_threads;
                    // number of worker threads to spawn (default: num_cpu_cores-1)
                // int max_fibers;
                    // maximum fibers that are allowed to be active at the same time (default: 64)
                // int fiber_stack_sz;
                    // fiber stack size (default: 1mb)
                // sx_job_thread_init_cb* thread_init_cb;
                    // callback function that will be called on initiaslization of each worker thread
                // sx_job_thread_shutdown_cb* thread_shutdown_cb;
                    // callback functions that will be called on the shutdown of each worker thread
                // void* thread_user_data;
                    // user-data to be passed to callback functions above

job_ctx_t *     job_ctx_create(const job_desc_t* desc) { return sx_job_create_context(JOB_ALLOCATOR, desc); }
void            job_ctx_destroy(job_ctx_t* ctx) { sx_job_destroy_context(ctx, JOB_ALLOCATOR); }
job_h           job_dispatch(
                    job_ctx_t* ctx,
                    i32 count,
                    job_cb_t* job_cb,
                    void* user,
                    job_prio_t prio, // sx_default(SX_JOB_PRIORITY_NORMAL),
                    u32 tags) //sx_default(0)
                {   return sx_job_dispatch(ctx,count,job_cb,user,prio,tags); }
void            job_await(job_ctx_t* ctx, job_h h) { sx_job_wait_and_del(ctx, h); }
                    // (Thread-Safe) Blocks, waits on dispatched job and then deletes
bool            job_check(job_ctx_t* ctx, job_h h) { return sx_job_test_and_del(ctx, h); }
                    // (Thread-Safe) non-blocking function, checks if job is finished
int             job_worker_count(job_ctx_t* ctx) { return sx_job_num_worker_threads(ctx); }
                    // number of worker threads running (does not include main thread)
                    //  If job is finished, returns `true` and deletes the job handle
                    //  Else, returns `false` immediately
int             job_this_idx(job_ctx_t* ctx) { return sx_job_thread_index(ctx); }
                    // Get current working thread's index (0..num_workers)
int             job_this_id(job_ctx_t* ctx) { return sx_job_thread_id(ctx); }
                    // Get current working thread's Os Id
void            job_this_tag(job_ctx_t* ctx, u32 tags) { sx_job_set_current_thread_tags(ctx, tags); }



#include        <signal.h>
#include        <unistd.h>

static char volatile * volatile   _js_eval_src[1];
static char volatile * volatile   _js_eval_ret;

void            js_eval(char * src, size_t len)
{
                    printf("++ %s\n",src);
                    _js_eval_src[0] = malloc(len);
                    memcpy(_js_eval_src[0], src, len);
                    kill(getpid(), SIGUSR2);
}

static js_val_t _js_eval_flush(napi_env env, napi_callback_info ctx)
{
                    // kill(getpid(), SIGUSR2);
                    char src[] = "console.log('!!!!!!!!')";
                    js_eval(src, strlen(src) + 1);
                    return NULL;
}

static js_val_t _js_eval_run(napi_env env, napi_callback_info ctx)
{
                    printf("** %s\n",_js_eval_src[0]);
                    if (_js_eval_src[0] != NULL) {
                        js_val_t result;
                        JS_VAL_UTF8(src, _js_eval_src[0], strlen(_js_eval_src[0]))
                        NAPI_CALL(env, napi_run_script(env, src, &result));
                        free(_js_eval_src[0]);
                        _js_eval_src[0] = NULL;
                    }
                    // _js_eval_src = NULL;

                    return NULL;
}

void            __job_setup(napi_env env, js_val_t exports)
{
                    JS_EXPORT_FN_AS(_js_eval_run,       _eval_run)
                    JS_EXPORT_FN_AS(_js_eval_flush,     _eval_flush)
}

