
`space pkg` # print "++++ $SPACENAME : $DIRNAME ++++"

# xcode-select --install
# https://github.com/Homebrew/homebrew-core/issues/2502
# brew config
#       if => CLT: N/A
#          xcode-select --install
#       ! CONFIRMED WORKING

# brew gcc

function () {
    `mod casm`

    # print "++++ $mod_name : $this : $fn_prefix : $mod_dir ++++"

    # --dev: () -d
    # --production: () -p
    # -d: () {
    #     NODE_ENV=development
    # }
    # -p: () {
    #     export NODE_ENV=production
    # }

    `fn hello` () {
        print 'hello!'
    }
    `fn i install` () {
        npm i
    }
    `fn clean` () {
        rm -fR ./build ./bin $@
        rm -f ./binding.c
    }
    `fn fresh` () {
        clean ./node_modules
    }
    `fn build-sx`() {
        pushd "$mod_dir"/src/sx
            mkdir build
            pushd build
                CC=gcc CXX=g++ MAKE=gmake cmake ..
                gmake
            popd
        popd
    }
    `fn build-malloc`() {
        pushd "$mod_dir"/src/malloc/mimalloc
            rm -rf out/release
            mkdir -p out/release
            pushd out/release
                CC=gcc CXX=g++ MAKE=gmake cmake -D__wasm_mem__=1 ../..
                gmake
            popd
        popd
    }
    `fn test` () {
        node ./suite/test.js
    }
    `fn bench` () {
        node ./suite/bench.js
    }
    `fn build` () {
        pushd "$mod_dir"
            mkdir -p ./build

            pushd "src"
                amalg ../binding.c `cat ./__amalg`
            popd

            export MACOSX_DEPLOYMENT_TARGET=10.14
            export CXX=aecc
            export CC=aecc

            PYTHON=/usr/bin/python MACOSX_DEPLOYMENT_TARGET=10.14 HOME=~/.electron-gyp node-gyp  --make="gmake" -jobs=max rebuild ;
            # --loglevel=silly
        popd
    }
    `fn rebuild` () {
        print "++++ $SPACENAME : $DIRNAME ++++"
        print "++++ $xx : $mod_name : $mod_path : $fn_prefix : $mod_dir ++++"
        print "mod_path = ${mod_path}"
        ${mod_path} clean
        ${mod_path} build
    }
    `fn run-e` () {
        # -e: ()
        # {
            ./node_modules/.bin/electron ./main.js;
        # }
    }
    `fn run-v8` () {
        # {
            node ./main.js;
        # }
        # --: || -v8:
        # @:
        # ((!$#)) &&
    }
}

# `@mod c-api` () {
#     print c-api

#     `@pub hello` () {
#         print 'hello!'
#     }

#     --dev: () -d
#     --production: () -p

#     -d: () {
#         NODE_ENV=development
#     }
#     -p: () {
#         export NODE_ENV=production
#     }

#     # --: || -p:

#     `@fn
#     install i` () {
#         npm i
#     }
#     `@fn
#     clean` () {
#         rm -fR ./build ./bin $@
#     }
#     fresh: () {
#         clean ./node_modules
#     }
#     build: () {
#         CC=gcc-9 HOME=~/.electron-gyp node-gyp rebuild ;
#     }
#     rebuild: () {
#         clean
#         build
#     }
#     run: () {
#         -e: ()
#         {
#             ./node_modules/.bin/electron ./main.js;
#         }
#         -v8: ()
#         {
#             node ./main.js;
#         }
#         # --: || -v8:
#         # @:
#         ((!$#)) &&
#     }
#     @:
#     # ${argv}
#     # @: || __HELP__
# }




