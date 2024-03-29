AC_PREREQ([2.72])
AC_INIT([lightwrite], [0.1.0], [])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_INSTALL

PKG_CHECK_MODULES([SDL2], sdl2, [], AC_MSG_ERROR([Failed to find sdl2]))
AC_CHECK_LIB([sdl2_ttf], [TTF_Init], AC_MSG_ERROR([Failed to find TTF_Init in sdl2_ttf]))
AC_CHECK_LIB([sdl2_ttf], [TTF_GetError], AC_MSG_ERROR([Failed to find TTF_Init in sdl2_ttf]))
AC_CHECK_LIB([sdl2_ttf], [TTF_Quit], AC_MSG_ERROR([Failed to find TTF_Quit in sdl2_ttf]))
AC_CHECK_LIB([sdl2_ttf], [TTF_SizeText], AC_MSG_ERROR([Failed to find TTF_SizeText in sdl2_ttf]))
AC_CHECK_LIB([sdl2_ttf], [TTF_RenderTextBlended], AC_MSG_ERROR([Failed to find TTF_RenderTextBlended in sdl2_ttf]))
AC_CHECK_LIB([sdl2_ttf], [TTF_OpenFont], AC_MSG_ERROR([Failed to find TTF_OpenFont in sdl2_ttf]))
AC_CHECK_LIB([sdl2_ttf], [TTF_CloseFont], AC_MSG_ERROR([Failed to find TTF_CloseFont in sdl2_ttf]))

AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset])

AC_CONFIG_FILES([Makefile
src/Makefile
data/Makefile
])
AC_OUTPUT
