{   "targets": [
        {
            "target_name":
                "casm",
            'dependencies': [
                'deps',
            ],
            'libraries': [
                '-L/usr/lib',
                '-L/usr/local/lib',
                '-ldl',

                "-L/usr/local/Cellar/tcc/0.9.27_1/lib", "-ltcc",

                "-L<(module_root_dir)/pkg/sx/build", "-lsx"
            ],
            "include_dirs": [
                "./pkg/ems/src",
                "./include",
                "<(module_root_dir)/pkg/sx/include",
                "<(module_root_dir)/pkg/napi-macros",

                "/usr/local/Cellar/tcc/0.9.27_1/include"
            ],
            "sources": [
                "./binding.c",
            ],
            "xcode_settings":
                {
                    '__CLANG_CXX_LANGUAGE_STANDARD': 'gnu++11',
                    'MACOSX_DEPLOYMENT_TARGET':
                        '10.14',
                    "OTHER_CFLAGS": [
                        "-O3",
                        "-std=gnu18",
                        "-D_GNU_SOURCE",
                          '-pthread',
                         '-Wno-incompatible-pointer-types',
                         '-Wno-int-conversion',
                         '-w',
                    ],
                    "OTHER_CPLUSPLUSFLAGS": [
                        "-mmacosx-version-min=10.14"
                    ],
                    "OTHER_LDFLAGS__":["-static"],
                    "OTHER_LDFLAGS____": [ '-std=gnu++11', '-stdlib=libc' ]
                },
            "cflags": [

                "-mmacosx-version-min=10.14",
                "-O3",
                "-D_GNU_SOURCE",
                "-std=gnu18",
                '-pthread',
                '-g',
                '-zdefs',

                '-Wno-error=deprecated-declarations'
            ],
            'cflags!': ['-stdlib=libc++'],
            'ldflags': [
              '-pthread'
            ]
        },
        {   "target_name":
                "deps",
            'type':
                'static_library',
            'direct_dependent_settings': {
                'defines': [],
                'include_dirs': [
                    "./pkg/libhydrogen"
                ],
            },
            "include_dirs": [
                "./pkg/libhydrogen"
            ],
            "sources": [
                "./pkg/libhydrogen/hydrogen.c",

            ],

            "xcode_settings":
                {
                    'CLANG_CXX_LANGUAGE_STANDARD': 'gnu++11',
                    'MACOSX_DEPLOYMENT_TARGET':
                        '10.14',
                    "OTHER_CFLAGS": [
                        "-std=gnu18",
                        "-D_GNU_SOURCE"
                    ],
                    "OTHER_CPLUSPLUSFLAGS": [
                        "-mmacosx-version-min=10.14"
                    ]
                },
            "cflags": [
                "-mmacosx-version-min=10.14",
                "-O3",
                "-D_GNU_SOURCE",
                "-std=gnu18",
                '-w',
                '-pthread',
                '-zdefs',
                '-Werror',
                '-Wno-error=deprecated-declarations'
            ]
        },

    ],




    "__NOTES__": [
        '<(module_root_dir)/build/Release/deps.a',
        {
            "OTHER_LDFLAGS__":["-rpath"],
            "OTHER_LDFLAGS____": [ '-std=gnu++11', '-stdlib=libc' ],
        },
        '-Werror','-Wall',
        "https://gyp.gsrc.io/docs/UserDocumentation.md#Dependencies-between-targets",
        "https://github.com/nodejs/node-gyp/wiki/%22binding.gyp%22-files-out-in-the-wild",
        "?  -flto",
        '-std=gnu++11',
        "-std=gnu18",
        '-g',
        "-std=c99",
        '-std=c++11',
        "-std=c18",
        "<(module_root_dir)/crypto/libhydrogen/libhydrogen.a",


                "./src/ems/src/ems.h",
                "./src/ems/src/ems_alloc.h",

                "./src/xm/collectives.c",
                "./src/xm/ems_alloc.c",
                "./src/xm/ems.c",
                "./src/xm/loops.c",
                "./src/xm/primitives.c",
                "./src/xm/rmw.c",

        ""
    ]
}

