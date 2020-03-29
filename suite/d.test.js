const {
    SUITE, TEST, TEST_skip, LOG, T0,T1
} = require('../lib/dev.js')

module.exports = SUITE('d.js',() => {



    let d
    let bindings
    TEST("require", function(Y){
        Y(!!(d = require('../lib/d.js/index.js')))
        Y(!!(bindings = require('../lib/c.js/index.js').bindings))
    })

    TEST('hash64',function(Y,N){
        Y(d.hash64("hello")>0n)
        Y(d.hash64("hello")==d.hash64("hello"))
        Y(d.hash64("hello")!=d.hash64("hello!"))
    })
    TEST('hash32',function(Y,N){
        Y(d.hash32("hello")!=0)
        Y(d.hash32("hello")==d.hash32("hello"))
        Y(d.hash32("hello")!=d.hash32("hello!"))
    })
    TEST('secret_hash64',function(Y,N){
        d.secret_set_phrase("password123")
        let h1 = d.secret_hash64("hello")
        Y(h1!=0n)
        Y(h1==d.secret_hash64("hello"))
        Y(h1!=d.secret_hash64("hello!"))

        d.secret_set_phrase("passwordabc")
        let h2 = d.secret_hash64("hello")
        Y(h2!=0n)
        Y(h2!=h1)
        Y(h2==d.secret_hash64("hello"))
        Y(h2!=d.secret_hash64("hello!"))
    })
    TEST('secret_hash32',function(Y,N){
        d.secret_set_phrase("password123")
        let h1 = d.secret_hash32("hello")
        Y(h1!=0)
        Y(h1==d.secret_hash32("hello"))
        Y(h1!=d.secret_hash32("hello!"))

        d.secret_set_phrase("passwordabc")
        let h2 = d.secret_hash32("hello")
        Y(h2>0)
        Y(h2!=h1)
        Y(h2==d.secret_hash32("hello"))
        Y(h2!=d.secret_hash32("hello!"))
    })
    TEST('secret_hash32',function(Y,N){
        d.secret_set_phrase("password123")
        let h1 = d.secret_hash32("hello")
        Y(h1!=0)
        Y(h1==d.secret_hash32("hello"))
        Y(h1!=d.secret_hash32("hello!"))

        d.secret_set_phrase("passwordabc")
        let h2 = d.secret_hash32("hello")
        Y(h2>0)
        Y(h2!=h1)
        Y(h2==d.secret_hash32("hello"))
        Y(h2!=d.secret_hash32("hello!"))
    })
    TEST('secret_get_hex', function(Y,N){
        d.secret_set_phrase("password")
        let key1 = d.secret_get_hex()
        d.secret_set_phrase("1234")
        let key2 = d.secret_get_hex()
        d.secret_set_phrase("password")
        let key3 = d.secret_get_hex()
        Y(key1.length === 64)
        Y(key2.length === 64)
        Y(key1 != key2)
        Y(key1 == key3)
    })

    TEST('io', function(Y,N){
        let xid = 0;
        let xstk_l = d.xstack_length()
        let xids_l = d.xids_length()
        xid = d.mkx('/tmp/d.test.x',1024,1024<<3,d.PERSIST|d.CLEAR);
        d.pushx(xid);

            Y(d.xstack_length() == xstk_l+1)
            Y(d.xids_length() == xids_l+1)
            TEST('read/write', function(Y,N){
                Y(d.read('name') === null);
                  d.write('name','joe smith');
                Y(d.read('name') === 'joe smith')
                  d.write('name','joe smith jr');
                Y(d.read('name') === 'joe smith jr')
            })
            TEST('get/set', function(Y,N){                
                d.key('username');
                d.REQ();
                Y(d.get_str() === null, d.get_str())
                  d.set('joe smith');
                  d.PUT();
                d.REQ();
                Y(d.get_str() === 'joe smith',d.get_str())
                  d.set('joe smith jr');
                  d.PUT();
                d.REQ();
                Y(d.get() === 'joe smith jr',d.get())
            })
            TEST('get/set|json', function(Y,N){
                d.key('creds');
                d.set({u:'billy',p:'love'});
                d.PUT();

                d.REQ();
                let {u,p} = d.get_json()
                Y(u == 'billy',u)
                Y(p == 'love',p)
            })
            TEST('get/set|i64', function(Y,N){
                let v;
                d.key_str('64');
                d.set_i64(164n);
                d.PUT();

                d.REQ();
                Y((v = d.get_i64()) == 164n,v);
            })
            TEST('get/set|i32', function(Y,N){
                let v;
                d.key_str('32');
                d.set_i32(132);
                d.PUT();

                d.REQ();
                Y((v = d.get_i32()) == 132,v);
            })

            TEST('secret:get/set|json|fun', function(Y,N){
                d.secure_key('creds');
                d.secure_set_json({u:'billy',p:'love'});
                d.PUT();

                d.REQ();
                let {u,p} = d.secure_get_json();
                Y(u == 'billy',u)
                Y(p == 'love',p)
            })
            TEST('secret read / write', function(Y,N){
                Y(d.secret_read('name') === null)

                d.secret_set_phrase('password')
                Y(d.secret_read('name') === null)
                d.secret_write('name','joe smith')
                Y(d.secret_read('name') === 'joe smith')
                d.secret_write('name','joe smith jr')
                Y(d.secret_read('name') === 'joe smith jr')

                d.secret_set_phrase('qwerqwer')
                Y(d.secret_read('name') === null)
                d.secret_write('name','jane doe')
                Y(d.secret_read('name') === 'jane doe')
                d.secret_write('name','ms jane doe')
                Y(d.secret_read('name') === 'ms jane doe')

                d.secret_set_phrase('password')
                Y(d.secret_read('name') === 'joe smith jr')

                d.secret_set_phrase('qwerqwer')
                Y(d.secret_read('name') === 'ms jane doe')
            })
            TEST('secret functional api', function(Y,N){
                let v;
                function SECRET_READ() {
                    d.secure_key('username')
                    d.REQ();
                    return d.secure_get_str();
                }
                function SECRET_WRITE(val) {
                    d.secure_key('username')
                    d.secure_set_str(val);
                    d.PUT();
                }
                    Y(SECRET_READ() === null,SECRET_READ())
                d.secret_set_phrase('password')
                    Y(SECRET_READ() === null,SECRET_READ())
                      SECRET_WRITE('joe smith')
                    Y(v=SECRET_READ() === 'joe smith',v)

                      SECRET_WRITE('joe smith jr')
                    Y(SECRET_READ() === 'joe smith jr')
                d.secret_set_phrase('qwerqwer')
                    Y(SECRET_READ() === null)
                      SECRET_WRITE('jane doe')
                    Y(SECRET_READ() === 'jane doe')
                      SECRET_WRITE('ms jane doe')
                    Y(SECRET_READ() === 'ms jane doe')
                d.secret_set_phrase('password')
                    Y(SECRET_READ() === 'joe smith jr')
                d.secret_set_phrase('qwerqwer')
                    Y(SECRET_READ() === 'ms jane doe')
            })
            let xid_sub = d.mkx('/tmp/d.test-sub.x',1024,1024<<3,d.PERSIST|d.CLEAR)
            d.pushx(xid_sub);

                TEST('pushx', function(Y,N){
                    Y(d.read('name') === null)
                    d.write('name','billy bob')
                    Y(d.read('name') === 'billy bob')
                })

            let poppedx = d.popx();
            TEST('popx', function(Y,N){
                let wx = d.cwx();
                let v;
                Y(wx == xid, `${wx} != ${xid}`)
                Y(v=d.read('name') !== 'billy bob',v)
            })
            d.pushx(xid_sub);

                TEST('pushx 2', function(Y,N){
                    Y(d.read('name') === 'billy bob')
                })

            xid = d.popx();

        xid = d.popx();
        Y(d.xstack_length() == xstk_l)
        Y(d.xids_length() == xids_l + 2)


        TEST('stack', function(Y,N){
            d.pushx(
                d.mkxa('/tmp/d.test.stack.x',128,128<<3,d.PERSIST|d.CLEAR) );
                Y(d.xstack_length() == xstk_l + 1)
                Y(d.xids_length() == xids_l + 3)
                let v
                Y((d.stk_consume()) === null)
                d.stk_produce('hello')
                Y((v= d.stk_consume()) === 'hello', v)
                Y((d.stk_consume()) === null)
                Y((d.stk_consume()) === null)
                d.stk_produce('abc')
                d.stk_produce('xyz')
                Y((d.stk_consume()) === 'xyz')
                Y((d.stk_consume()) === 'abc')
                Y((d.stk_consume()) === null)
                d.stk_produce('1')
                d.stk_produce('2')
                Y((d.stk_consume()) === '2')
                Y((d.stk_consume()) === '1')
                Y((d.stk_consume()) === null)
            d.popx()
        });

        TEST('stack|fun', function(Y,N){
            d.pushx(
                d.mkxa('/tmp/d.test.stackfun.x',128,128<<3,d.PERSIST|d.CLEAR) )
                let v
                let len

                d.POP();
                Y((d.get()) === null)
                Y((d.get_str()) === null)

                d.set_str('hello');
                d.PUSH();
                len = d.get_status();
                // len = d.push('hello').$();
                Y(len === 1, len);
                Y(d.stk_get_top() == len);

                d.key_i64(0n);
                d.REQ();
                Y((v= d.get()) === 'hello', v)
                Y((v= d.get_str()) === 'hello', v)

                d.POP();
                Y((v= d.get()) === 'hello', v)
                Y((v= d.get_str()) === 'hello', v)
                // Y((v= d.GET().i(0).$()) === 'hello', v)
                // Y((v= d.POP().$()) === 'hello', v)

                d.POP();
                Y((d.get()) === null)
                Y((d.get_str()) === null)

                d.POP();
                Y((d.get()) === null)
                Y((d.get_str()) === null)

                // Y((d.POP().$()) === null)
                // Y((d.POP().$()) === null)

                d.set_str('abc');
                d.PUSH();

                d.set('xyz');
                d.PUSH();

                len = d.get_status();
                Y(len === 2,{len});
                Y(d.stk_get_top() == len);

                d.key_i32(1);
                d.REQ();
                Y(d.get() === 'xyz');

                d.key_i32(0);
                d.REQ();
                Y(d.get() === 'abc');

                d.POP();
                Y(d.get() === 'xyz');

                d.POP();
                Y(d.get() === 'abc');

                d.POP();
                Y(d.get() === null);

                Y(d.stk_get_top() == 0);
            d.popx()
        })

        TEST('queue|fun', function(Y,N){
            d.pushx(
                d.mkxa('/tmp/d.test.queuefun.x',128,128<<3,d.PERSIST|d.CLEAR) );
                let v
                d.DEQ();
                Y((d.get()) === null);
                Y((d.get_str()) === null);

                d.set_str('hello');
                d.ENQ();

                d.DEQ();
                Y((d.get_str()) === 'hello');
                Y((d.get()) === 'hello');

                d.DEQ();
                Y((d.get_str()) === null);

                d.DEQ();
                Y((d.get_str()) === null);

                d.set('abc');
                d.ENQ();
                d.set_str('xyz');
                d.ENQ();

                d.DEQ();
                Y((d.get()) === 'abc');
                d.DEQ();
                Y((d.get_str()) === 'xyz');

                d.DEQ();
                Y((d.get_str()) === null);
            d.popx()
        })

        TEST('FE read / write', function(Y,N){
            d.pushx(
                d.mkx('/tmp/d.test.fe.x',1024,1024<<3,d.PERSIST|d.CLEAR)
            );
            d.key('TIMEOUT');
            d.set_str('x');
            d.PUT_XF();

            // Y(d.GET_EF().k('TIMEOUT').timeout(1).$() === null)
            // Y(d.GET_EF().k('TIMEOUT').timeout(1).$() === null)
            d.set_timeout(1);

                d.REQ_EF();
                Y(d.get_str() === null);

                d.REQ_EF();
                Y(d.get_str() === null);

            d.set_timeout(0);

            d.REQ_FF();
            Y(d.get_str() === 'x');

            d.REQ_XX();
            Y(d.get_str() === 'x');

            d.REQ();
            Y(d.get_str() === 'x');

            d.popx();
        });

        TEST('smoke-loops', function(){
            TEST('read/write|funx1000 I', function(Y,N){
                let v
                let i = 1000
                while(i--) {
                    d.key_str('xxxx')
                    d.set_str('joe smith' + i)
                    d.PUT();
                    d.REQ();
                    v = d.get_str();
                }
                Y(v === 'joe smith0',v)
            })
            TEST('read/write|funx1000 II', function(Y,N){
                let v
                let i = 1000
                d.key_str('xxxx')
                while(i--) {
                    d.set_str('joe smith' + i)
                    d.PUT();
                    d.REQ();
                    v = d.get_str();
                }
                Y(v === 'joe smith0',v)
            })
            TEST('read/write|funx1000 III', function(Y,N){
                let v
                let i = 1000
                d.key_str('xxxx')
                while(i--) {
                    d.set_i32(i)
                    d.PUT();
                    d.REQ();
                    v = d.get_i32();
                }
                Y(v === 0,v)
            })
            TEST('js-map.get/set|funx1000', function(Y,N){
                let m = new Map()
                let v
                let i = 1000
                while(i--) {
                    m.set('xxxx','joe smith' +i);
                    v = m.get('xxxx');
                }
                Y((v) === 'joe smith0')
            });
        })

        TEST_skip('autogrow', function(Y,N){
            d.pushx(
                d.mkxa('/tmp/d.test.autogrow.x',2,16,d.CLEAR) );
                let v
                Y((d.DEQ().$()) === null)
                d.ENQ().v('12345678').$()
                Y((v= d.DEQ().$()) === '12345678',v)
                Y((v= d.DEQ().$()) === null,v)
                Y((v= d.DEQ().$()) === null,v)
                d.ENQ().v('12345678').$()
                d.ENQ().v('12345678').$()
                d.ENQ().v('12345678').$()
                Y((v= d.DEQ().$()) === '12345678',v)
            d.popx()
        })

    })
    // TEST('x-usage', function(Y,N){
    //     function xbot() {
    //         const xid = d.mkx('/.xbot',1024,1024<<3,d.PERSIST|d.CLEAR);
    //         const conf = {}
    //         return
    //         function
    //         function signup() {

    //         }
    //         function login() {

    //         }

    //     }
    // })
})