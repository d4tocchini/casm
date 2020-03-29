
let _spawn
function spawn(cmd, args, opts) {
    (_spawn || (_spawn = require('child_process').spawn))(
        cmd, args, opts
    )
}

module.exports = SUITE('e.js',async () => {

    let e
    TEST("require", function(Y){
        Y(!!(e = require('../../lib/casm.js')))
    })

    const name = 'tester';
    let agent
    const self = {AGENT_TEST: 'NAY'}
    TEST("mount", function(Y){
        agent = e.mount(name, self)
        Y(!!agent)
    })


    TEST("JS.in().src()", function(Y){
        agent.JS().in(name).src(`this.AGENT_TEST = 'YAY';`).$();

        Y(self.AGENT_TEST == 'NAY');
        agent.tick();
        Y(self.AGENT_TEST == 'YAY');
        agent.tick();
    });


    let loop = setInterval(agent.tick, 32);

    TEST("JS.in().async()", async function(Y){
        let res = await agent.JS().in(name).async(`return this.AGENT_TEST = 'HEY';`).$();
        Y(res == 'HEY');
        Y(res == 'HEY');
        return
    });
    TEST("JS.in().async()II", async function(Y){
        let res = await agent.JS().in(name).async(`return this.AGENT_TEST = 'YOU';`).$();
        Y(res == 'YOU');
        clearInterval(loop)
        return
    });


    // const children = new Map();
    // function spawn_if_needed(child_name) {
    //     if (children.has(child_name)) return children.get(child_name)
    // }
    // TEST("routing", function(Y){
    //     agent.route('child_proc', function(child_name) {

    //     })
    // });

    // setInterval(agent.tick, 100);
    return true

})