# this is example-file: configure.ac
m4_define([v_maj], [m4_esyscmd_s([cat VERSION | cut -d '.' -f 1])])
m4_define([v_min], [m4_esyscmd_s([cat VERSION | cut -d '.' -f 2])])
m4_define([v_mic], [m4_esyscmd_s([cat VERSION | cut -d '.' -f 3])])

m4_define([project_version], [v_maj.v_min.v_mic])
 
m4_define([lt_cur], m4_eval(v_maj + v_min))
m4_define([lt_rev], [v_mic])
m4_define([lt_age], [v_min])
# initial information about the project
AC_INIT([scisdk], [project_version],[abba@nuclearinstruments.eu])

# check if the source folder is correct
AC_CONFIG_SRCDIR([src/SciSDK_DLL.cpp])

# Checks for programs

# check for C++ preprocessor and compiler and the library compiler
AC_PROG_CXXCPP
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_LIBTOOL


# automake initialisation and check for minimal automake API version 1.9

AM_INIT_AUTOMAKE([1.9])

# use the C++ compiler for the following checks
AC_LANG([C++])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# distribute additional compiler and linker flags
# --> set these variables instead of CXXFLAGS or LDFLAGS
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
VERSION_INFO="lt_cur:lt_rev:lt_age"
AC_SUBST([VERSION_INFO])#, [$LIBSCISDK_MAJOR_VERSION:$LIBSCISDK_MINOR_VERSION:$LIBSCISDK_MICRO_VERSION])



# files to generate via autotools (.am or .in source files)
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])


# generate the final Makefile etc.
AC_OUTPUT
