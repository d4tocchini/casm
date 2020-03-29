
let RUN_SIZE      = 1000;
let WARMUP_SIZE   = 1;
const NOOP = function(){}
const NOOP_TRUE = function(){return true;}

const _errs = new Set()
const _names = new Set()

let size = WARMUP_SIZE;
let _setup = NOOP;
let _validate = NOOP_TRUE;
let _i = 0|0;

global.bench = bench;
global.bench_report = bench_report;
global.test = test;
global._ = _;
global.loop_i = function () {return _i;}
global.loop_l = function () {return size;}

function bench_report() {
    console.log(`    ✓ ${_names.size}`)
    if (_errs.size)
        console.log(`    ❌ ${_errs.size} ${[..._errs].join(', ')}`)
    console.log()
}

function test(SPECS, loops) {
    size = WARMUP_SIZE = loops
    for (const spec of SPECS) {
        const name = spec.name;
        if (name[0] === '_') continue
        _setup();
        for (let i = 0; i < WARMUP_SIZE; ++i) spec(_i=(i|0));
        if (WARMUP_SIZE && !_validate())
            _errs.add(name)
    }
}

function bench(SPECS, loops) {
    size = RUN_SIZE = loops;
    for (const spec of SPECS) {
        const name = spec.name;
        if (name[0] === '_') {
            spec()
            continue
        }
        _names.add(name);
        const label = `    ${name.padEnd(16)}`
        if (_errs.has(name)) {
            _err(label);
            continue;
        }

        _setup();
        console.time(label);
        for (let i = 0; i < size; ++i) spec(_i=(i|0));
        console.timeEnd(label);
        if (!_validate()) {
            _errs.add(name)
            _err(' '.repeat(label.length))
        }
    }
    console.log('')
}

function _(name, setup, validate) {
    _setup = setup || NOOP;
    if (validate) {
        _validate = function () {
            try {
                return validate()
            }
            catch(e) {
                console.warn(e)
                return false
            }
        }
    }
    else
        _validate = NOOP_TRUE;
    return function _ () {
        console.log(`    ${'_'.repeat(17)} ${name} x ${RUN_SIZE}`)
    }
}
function _err(label) {
    console.log(`${label} ❌`)
}