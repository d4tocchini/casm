module.exports =
{
                T0, T1,
                LOG,
                SUITE, TEST, TEST_skip,
                BENCH
}

function        T0(x)
{                   console.time(x)
}
function        T1(x)
{                   console.timeEnd(x)
}
// const _CODE = ' '.charCodeAt(0)

let path = []
let marks = ''
let passes = 0
let errs = 0
let depth = 0
let prefix = ''
let total_sync_time = 0
let total_time = 0

const PROMISES = []
const ARRAY_EMPTY = []

// console._log = console.log
let report = ''
console.print = function(...a) {
    const msg = a.join(' ')
    report += msg + '\n'
    return msg
}
// console.log = function() {}
function        LOG(...msgs)
{

                    console.log(prefix + msgs.join(' '))
}
function        _LOG(...msgs)
{

                    console.log(prefix + msgs.join(' '))
}

async function        _enter(name, cb, cbargs, state)
{
                    const {is_suite} = state;

                    let _depth = depth
                    // passes = 0
                    // errs = 0
                    // marks = ''

                    let line = '| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | '.substr(0,((_depth)<<1)
                    )
                    console.log(line + name)


                    path[depth++] = name

                    let _prefix = prefix
                    prefix = ''
                    for (let i =0; i<depth; i++)
                        prefix += '  '

                    let t0;
                    let is_async = cb.toString().startsWith('async')
                    if (is_async) {
                        console.print( line + name + ' ...' )
                        line =line + '... '+ name ;
                        t0= process.hrtime()
                        PROMISES.push(cb.apply(null, cbargs).then(done))
                    }
                    else {
                        line = line + name;
                        t0= process.hrtime()
                        cb.apply(null, cbargs)
                        done();
                    }

                    path.length = --depth
                    // if (!async) {
                        // marks = _marks
                        // passes += _passes
                        // errs += _errs
                        // }

                    function done() {

                        const timediff = process.hrtime(t0)
                        const duration =  (timediff[0]*1e3 + timediff[1]/(1e6));
                        total_sync_time += (!is_suite) * (!is_async) * duration
                        total_time += (!is_suite) * duration

                        console.print(line );

                        prefix = _prefix
                        if (state.passes | state.errs) {
                            // " " + name + " | " +
                            let postfix = (state.errs?"🔴 ": "✓") + " " + state.passes +
                                ( state.errs
                                    ? "/" + (state.errs + state.passes) + " "
                                    : ""
                                )
                                + "  \t\t\t[" + (duration) +"ms]"
                            state.marks && (state.marks+=' ');
                            console.print(
                                (
                                    '| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | '.substr(0,((_depth)<<1))
                                ) + postfix
                                + (state.errs?state.marks:'')
                            )

                            passes += state.passes
                            errs += state.errs
                        }
                        // else {
                        //     console.log(  '||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||'.substr(0,
                        //         (depth)
                        //     ) + '- ' + name)
                        // }
                    }

}


let suites_active = 0
let suite_report = ''
async function        SUITE(name,cb)
{
                    ++suites_active
                    let t0= process.hrtime();

                    let state = {
                        passes: 0,
                        errs: 0,
                        marks: '',
                        is_suite:1,
                    }
                    await _enter(name, cb, [], state)
                    if (PROMISES.length){
                        await Promise.all(PROMISES)
                        PROMISES.length = 0;
                    }
                    const timediff = process.hrtime(t0)
                    const duration =  (timediff[0]*1e3 + timediff[1]/(1e6));
                    --suites_active


                    suite_report +=
                        '\n' + name
                        + "  [" + (duration) +"ms]"

                    if (suites_active == 0) {

                        console.log(
                            '\n----------------------------------------')
                        console.log(
                            '\n'+report + '\n' + suite_report)
                        console.log(
                            '\n'+passes +" / "+ (passes + errs)
                            + "  [" + (total_sync_time) +"ms]"
                            + "  [" + (total_time) +"ms]"
                            )
                        console.log(
                            '----------------------------------------\n')
                    }
}
const optmap = new Map()
function        TESTOPT_SET(k, v)
{
                    optmap.set(k,v)
}
function        TEST_skip () {}

const BENCH_STATE = {
    passes: 0,
    errs: 0,
    marks: '',
    is_suite:0,
}

async function  BENCH(name,cb) {
    let t0= process.hrtime();
    cb();
    const timediff = process.hrtime(t0)
    const duration =  (timediff[0]*1e3 + timediff[1]/(1e6));
    console.log(name, duration + 'ms')
}
async function  TEST(name,cb)
{
                    let state = {
                        passes: 0,
                        errs: 0,
                        marks: '',
                        is_suite:0,
                    }
                    const args = [YAY, NAY]
                    await _enter(name, cb, args, state)
                    // if (PROMISES.length){
                    //     await Promise.all(PROMISES)
                    //     // PROMISES.length = 0;
                    // }

    function        YAY(x,msg)
    {
                        if (!x)
                            ERR(msg)
                        else
                            PASS(msg)
                        // else console.log('    ', x)
                        return x
    }
    function        NAY(x,msg)
    {
                        if (x)
                            ERR(msg)
                        else {
                            ++state.passes
                            state.marks+="✓"
                            console.log(path.join('/') + " " + state.marks + " " + state.passes +"/" + (state.passes+state.errs) +"\n")
                        }
                        return x
    }
    function        ERR(msg)
    {
                        state.marks+="x"
                        console.log("\n🔴 " + (state.errs+state.passes)
                            + " " + msg
                            + " :: " + path.join('/') + '\n')
                        ++state.errs
                        // console.error(m)
    }
    function        PASS(m)
    {
                        ++state.passes
                        state.marks+="✓"
                        if (optmap.get('log-level'))
                            console.log(path.join('/') + " " + state.marks + " " + state.passes +"/" + (state.passes+state.errs) +"\n")
    }
}