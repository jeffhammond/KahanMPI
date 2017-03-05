AC_DEFUN([ACX_QUADRUPLE_PRECISION],[
  # Check for quadruple precision types
  # GCC __float128, Intel _Quad

  AC_LANG_PUSH([C])

  AC_CHECK_HEADERS([quadmath.h])

  # Check for GCC __float128
  AC_COMPILE_IFELSE(
    [
      AC_LANG_PROGRAM(
        [[__float128 x = 1.0Q/7.0Q;]],
        [[x *= 7.0Q;]]
      )
    ],
    [
      acx_quadruple_precision="__float128"
      AC_DEFINE(HAVE_GCC_FLOAT128,1,[GCC quadruple precision (__float128) is supported.])
      AC_DEFINE([quad],[__float128],[Define the quad keyword.])
    ]
  )

  # Check for Intel _Quad
  if test "$acx_quadruple_precision" = no; then
    AC_COMPILE_IFELSE(
      [
        AC_LANG_PROGRAM(
          [[_Quad x = 1.0Q/7.0Q;]],
          [[x *= 7.0Q;]]
        )
      ],
      [
        acx_quadruple_precision="_Quad"
        AC_DEFINE(HAVE_INTEL_QUAD,1,[Intel quadruple precision (_Quad) is supported.])
        AC_DEFINE([quad],[_Quad],[Define the quad keyword.])
      ]
    )
  fi

  if test "$acx_quadruple_precision" = no; then
    AC_MSG_ERROR([Unable to detect mandatory support for quadruple precision.])
  fi


  AC_MSG_RESULT([$acx_quadruple_precision])

  AC_LANG_POP
])

