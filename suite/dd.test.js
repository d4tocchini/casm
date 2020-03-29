



SUITE('dd.js',() => {
    let dd

    TEST("require", function(Y){
        Y(!!(dd = require('../lib/dd.js')))
        dd
            .set_dir("/tmp")
            .touch("dd-specs", dd.PERSIST|dd.CLEAR);
        ;
    })
    let tbl_allocs = 0
    const sbar = dd.ent("sbar");
    const screen = dd.ent("screen");

    const x = dd.ent("x");
    const w = dd.ent("w");
    const x_spring = dd.ent("x_spring");


    TEST("tbl basics", function(Y){

        let N = 10
        let i = 0;
        while (i++ < N) {

        console.time('tbl')

            const layout = dd
                .TBL("app","layout")
                .cols([x, w, x_spring])
            ;

        console.timeEnd('tbl')

            Y(dd.tbl_alloc_count() == ++tbl_allocs)

        console.time('w')

            layout.set_i32(w, screen, 1280);
            layout.set_i32(x, sbar, 100);

            // Y(layout.updated_comp_count() == 2)
            Y(layout.get_i32(w, screen) == 1280)
            Y(layout.get_i32(x, sbar) == 100)

            if (i > 1)
                Y(!layout.needs_commit())
            else
                Y(layout.needs_commit())

            if (i===N-1) {
                layout.set_i32(x, sbar, 100);
            }
            if (i===N) {
                layout.set_i32(x, sbar, 10);
                Y(layout.needs_commit())
            }

        console.timeEnd('w')
        console.time('x')

            layout.rectify()

        console.timeEnd('x')
        console.time('r')
            if (i===N) {
                Y(layout.get_i32(w, screen) == 1280)
                Y(layout.get_i32(x, sbar) == 10)
                Y(layout.comp_gen_get(x) === 2)
            }
            else {
                // Y(layout.updated_comp_count() == 0)
                Y(layout.get_i32(w, screen) == 1280)
                Y(layout.get_i32(x, sbar) == 100)
                Y(layout.comp_gen_get(x) === 1)
            }
        console.timeEnd('r')


            layout.free()
        }
    })


    TEST("tbl load", function(Y){
        let N = 10
        let i = 0;
        const layout = dd
                .TBL("app","layout")
                .cols([x, w, x_spring])
            ;
            Y(dd.tbl_alloc_count() == ++tbl_allocs)
        while (i++ < N) {

            // Y(layout.updated_comp_count() == 0)
            // Y(layout.saved_comp_count() == 2)
            // Y(layout.updated_comp_count() == 0)

            Y(layout.comp_gen_get(w) === 1)

            // layout.set_i32(w, screen, 1280);
            // layout.set_i32(x, sbar, 100);

            // Y(layout.comp_gen_get(x) === 1)
            // Y(layout.comp_gen_get(w) === 1, layout.comp_gen_get(w))

            Y(layout.get_i32(w, screen) == 1280)
            Y(layout.get_i32(x, sbar) == 10)
        }
        layout.set_i32(x, 1, 99);
        Y(layout.needs_commit())
        Y(layout.get_i32(x, 1) === 99)
        layout.rectify()

        Y(layout.get_i32(x, 1) === 99)

        layout.free()
    })

    TEST("tbl load again", function(Y){
        console.log('------')
        const layout = dd
            .TBL("app","layout")
            .cols([x, w, x_spring])
        ;

        // dd.DB("app")
        // dd.TBL("layout")
        // dd.COLS(["x","w","w_spring"])
        // dd.col(0)
        // dd.col_get_i32(sbar)
        // dd.col_set_i32(sbar, 1)

        Y(layout.get_i32(x, sbar) == 10)

        Y(layout.get_i32(x, 1) === 99)

        layout.free()
    })

    // TEST("tbl change listeners", function(Y){
    //     const layout = dd
    //         .TBL("app","layout")
    //         .cols([x, w, x_spring])
    //     ;

    //     let val_change_count = 0
    //     let col_change_count = 0

    //     layout.on_i32(x, sbar, function(){
    //         val_change_count++
    //     })
    //     layout.on_col(x, function() {
    //         col_change_count++
    //     })
    //     // layout.on_ent_i32(sbar, function() {
    //     // })

    //     layout.rectify();
    //     Y(val_change_count === 0);
    //     Y(col_change_count === 0);

    //     layout.set_i32(x, sbar, 10101)
    //     layout.set_i32(x, screen, 20202)

    //     layout.rectify();
    //     Y(val_change_count === 1);
    //     Y(col_change_count === 1);

    //     layout.set_i32(x, sbar, 10101)
    //     layout.set_i32(x, screen, 20202)

    //     layout.rectify();
    //     Y(val_change_count === 1);
    //     Y(col_change_count === 1);

    //     layout.set_i32(x, sbar, 101010)
    //     layout.set_i32(x, screen, 202020)

    //     layout.set_i32(x, sbar, 10101)
    //     layout.set_i32(x, screen, 20202)

    //     layout.rectify();
    //     Y(val_change_count === 1);
    //     Y(col_change_count === 1);

    //     layout.set_i32(x, sbar, 101010)
    //     layout.set_i32(x, screen, 202020)

    //     layout.set_i32(x, sbar, 101011)
    //     layout.set_i32(x, screen, 202022)

    //     layout.rectify();
    //     Y(val_change_count === 2);
    //     Y(col_change_count === 2);

    //     layout.free()
    // })

    TEST("smoke (100,000 iter) x (9ops/iter) = 320ms = 0.0003ms/op ", function(Y){
        let layout = dd
            .TBL("app","layout")
            .cols([x, w, x_spring])
        ;
        let N = 100000
        console.time('populating')
        let i = 0;
        while (i++ < N) {
            layout.set_i32_fast(w, i+screen, 1280^i);
            layout.set_i32_fast(w, i, i);
            layout.set_i32_fast(x, i+sbar, 100^i);
            layout.set_i32_fast(x, i, i<<1);
        }
        console.timeEnd('populating')
        console.time('rectifying')

        layout.rectify()

        console.timeEnd('rectifying')
        console.time('validating')
        i = 0;
        while (i++ < N) {
            Y(
                layout.get_i32(w, i+screen) == 1280^i
                && layout.get_i32(x, i+sbar) == 100^i
                && layout.get_i32(w, i) == i
                && layout.get_i32(x, i) == (i << 1)
            )
        }
        console.timeEnd('validating')

        layout.free()
    })

})



















//                 bar, x))
//             return

//         dat.set32(sbar, x, 250);
//         dat.set32(sbar, x_spring, 0);
//         dat.set(sbar, sbar, 256);
//         dat.set(POS_SPRING, sbar, 100);

//         Y(dat.saved_size(SZ) === 0);
//         Y(dat.updated_size(SZ) === 2);

//         Y(dat.get16_2x(SZ, screen) === 1600);
//         Y(dat.get16_2y(SZ, screen) === dat.get16_2y(SZ|dat.UPATED));
//         // Y(layout.get("sbar.w") === 100);
//     });
//     TEST("pod commit", function(Y){

//         dat.sys("", function(){
//             dat
//         })


//         dat.progress(16)

//         // layout.commit();

//         Y(dat.saved_size(SZ) === 2);
//         Y(dat.updated_size(SZ) === 0);

//         // layout.set("screen.w", 1600);
//         // layout.set("screen.h", 900);
//         // layout.set("sbar.w", 100);

//         // Y(layout.updated_size() === 3);
//         // Y(layout.get("screen.w") === 1600);
//         // Y(layout.get("screen.h") === layout.updated_get("screen.h"));
//         // Y(layout.get("sbar.w") === 100);
//     });
// })