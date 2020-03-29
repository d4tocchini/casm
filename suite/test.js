

const {
    SUITE, TEST, TEST_skip, LOG, T0,T1
} = require('../lib/dev.js')
global.SUITE = SUITE
global.TEST = TEST
global.TEST_skip = TEST_skip
global.LOG = LOG
global.T0 = T0
global.T1 = T1

require('./casm/test.js')
require('./dd.test.js');
