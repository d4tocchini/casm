#ifndef         _N_TYPES_H_
#define         _N_TYPES_H_

/*
============================================================*==================
TODO:
                - storage qualifers
                - threads tss / atomics . etc
                        https://en.cppreference.com/w/c/thread/tss_create
                        https://en.cppreference.com/w/c/thread
                - byte strings
                        https://en.cppreference.com/w/c/string/byte
*/

#define         __STDC_FORMAT_MACROS
#include        <inttypes.h>
#include        <stdint.h>
                // https://en.cppreference.com/w/c/types/integer

#define         alias()

typedef intptr_t
                iptr_t;
typedef uintptr_t
                uptr_t;
typedef size_t  sz_t;

typedef char    ch;
typedef signed char
                ich;
typedef unsigned char
                uchar;
typedef unsigned char
                uch;;
typedef uint8_t
                u8;
typedef uint16_t
                u16;
typedef uint32_t
                u32;
typedef uint64_t
                u64;
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;



/* floats
============================================================*==================

        FLP07-C. cast returned float
                https://wiki.sei.cmu.edu/confluence/display/c/FLP07-C.+Cast+the+retur+value+of+a+function+that+returns+a+floating-point+type

        Floating-point environment
                https://en.cppreference.com/w/c/numeric/fenv
*/

typedef float   f32;
typedef double  f64;
                // `f32 complex` for imaginary

/* quaifiers
============================================================*==================

        A_ _Atomic (C11)

        S_ static

        C_ const (C89)
                read-only, No writes through this lvalue.

        V_ volatile (C89)
                these values might keep on changing without any explicit assignment by the program

                No cacheing through this lvalue: each operation in the abstract semantics must be performed (that is, no cacheing assumptions may be made, since the location is not guaranteed to contain any previous value). In the absence of this qualifier, the contents of the designated location may be assumed to be unchanged except for possible aliasing.

        RPT *restrict (C99)

                Objects referenced through a restrict-qualified pointer have a special association with that pointer. All references to that object must directly or indirectly use the value of this pointer. In the absence of this qualifier, other pointers can alias this object. Cacheing the value in an object designated through a restrict-qualified pointer is safe at the beginning of the block in which the pointer is declared, because no pre-existing aliases may also be used to reference that object. The cached value must be restored to the object by the end of the block, where pre-existing aliases again become available.
                https://www.oracle.com/technetwork/server-storage/solaris10/cc-restrict-139391.html

void updatePtrs( sz *restrict ptrA, sz *restrict ptrB, sz *restrict val);

void updatePtrs( sz RPT ptrA, sz RPT ptrB, sz RPT val);

                - can be used in pointer declarations
                - a declaration of intent
                - that for lifetime of pointer, **only the pointer** itself or a value directly derived from it (such as pointer + 1) will be used to access the object to which it points. limits effects of "pointer aliasing", aiding optimizations. If the declaration of intent is not followed and the object is accessed by an `independent pointer`, this will result in undefined behavior+
                - The use of the restrict keyword in C, in principle, allows non-obtuse C to achieve the same performance as the same program written in Fortran.
                - the compiler is allowed to assume that ptrA, ptrB, and val point to **different locations** and updating one pointer will not affect the other pointers
                - programmer, not the compiler, is responsible for ensuring that the pointers do not point to identical locations
                https://en.wikipedia.org/wiki/Restrict

*/

#define         v_      volatile
#define         s_      static
#define         c_      const

#define         s_void  static void
#define         c_void  const void

#define         s_ch    static ch
#define         c_ch    const ch
#define        sc_ch    static const ch

#define        s_uch    static uch
#define        c_uch    const uch
#define       sc_uch    static const uch

#define         c_u8    static u8
#define         c_u8    const u8
#define        sc_u8    static const u8

/* memory
===============================================================================
                Function btox converts arbitrary data *bb to an unterminated string *xp of n hexadecimal digits:
*/

void            to_hex(ch* xp, c_ch* bb, i8 n)
{
        sc_ch           xx[]= "0123456789ABCDEF";

                        while (--n >= 0)
                                xp[n] = xx[(bb[n>>1]
                                        >> (( 1 - (n&1))
                                        << 2)) & 0xF];
}

/* tgmath type-generic math
===============================================================================
                https://en.cppreference.com/w/c/numeric/tgmath


#include        <stdio.h>
#include        <tgmath.h>

int             main(void)
{
        int             i = 2;
                                printf("sqrt(2) = %f\n", sqrt(i));
                                // int -> sqrt

        f32             f = 0.5;
                                printf("sin(0.5f) = %f\n", sin(f));
                                // float -> sinf

        f32I            dc = 1 + 0.5*I;
        f32I            z = sqrt(dc);
                                // float complex -> csqrtf

                                printf( "sqrt(1 + 0.5i) = %f+%fi\n",
                                // float complex -> crealf
                                        creal(z),
                                // float complex -> cimagf
                                        cimag(z));
}

/* Generic selection (C11)
===============================================================================

                Provides a way to choose one of several expressions at compile time, based on a type of a controlling expression

                Generic selection is similar to overloading in C++ (where one of several functions is chosen at compile time based on the types of the arguments), except that it makes the selection between arbitrary expressions.

                The controlling-expression & the expressions of the selections that are not chosen are never evaluated.

        *       "abc" ~ `char*` ! char[4]
        *       (int const){0} ~ `int` ! `const int`    (*C17)

                _Generic ( controlling-expression , association-list )

                https://en.cppreference.com/w/c/language/generic



// Possible implementation of the tgmath.h macro cbrt
// https://en.cppreference.com/w/c/language/generic


#include        <stdio.h>
#include        <math.h>

#define         cbrt(X) _Generic( (X), \
                        long double: cbrtl, \
                        default: cbrt, \
                        float: cbrtf \
                )(X)

int             main(void)
{       f64             x = 8.0;
C_      f32            y = 3.375;

                        // -> default
                        printf("cbrt(8.0) = %f\n", cbrt(x));

                        // Cf32 => f32 -> cbrtf
                        printf("cbrtf(3.375) = %f\n", cbrt(y));
}
*/


/* Generic selection (C11)
===============================================================================

        Null-terminated byte strings
                https://en.cppreference.com/w/c/string/byte
*/

#endif