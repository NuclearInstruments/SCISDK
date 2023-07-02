# this is example-file: src/Makefile.am

# additional include paths necessary to compile the C++ library
AUTOMAKE_OPTIONS = subdir-objects
AM_CXXFLAGS = -I$(top_srcdir)/src @AM_CXXFLAGS@ -Wno-narrowing -fpermissive -DSCISDK_DLL_EXPORTS

###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

# # the library names to build (note we are building static libs only)
# lib_LIBRARIES = libscisdk.a

# # where to install the headers on the system
# libscisdk_adir = $(includedir)/scisdk

# # the list of header files that belong to the library (to be installed later)
# libscisdk_a_HEADERS =    \
#                         *.h

# # the sources to add to the library and to add to the source distribution
# libscisdk_a_SOURCES =    \
#                         $(libmylib_a_HEADERS) \
#                         SciSDK_DLL.cpp \                 
# 						scisdk_DT5550W_petirocframe.cpp \
# 						scisdk_core.cpp \
# 						scisdk_digitizer.cpp  \
# 						scisdk_hal.cpp  \  
# 						scisdk_oscilloscope_dual.cpp  \
# 						scisdk_registers.cpp  \
# 						scisdk_DT5550W_citirocconfig.cpp  \
# 						scisdk_SciElement.cpp  \
# 						scisdk_custom_packet.cpp  \  
# 						scisdk_fftmon.cpp  \
# 						scisdk_list.cpp  \     
# 						scisdk_rate_meter.cpp \
# 						scisdk_scinode.cpp \
# 						scisdk_DT5550W_citirocframe.cpp \
# 						scisdk_TOFspectrum.cpp \
# 						scisdk_device.cpp \
# 						scisdk_frame.cpp \
# 						scisdk_oscilloscope.cpp \
# 						scisdk_register.cpp \
# 						scisdk_spectrum.cpp \
# 						boards_driver/bd_dt1260.cpp \
# 						boards_driver/bd_feelib.cpp \
# 						boards_driver/bd_dt5560se.cpp

lib_LTLIBRARIES = libscisdk.la

libscisdk_la_LDFLAGS = -version-info @VERSION_INFO@

# where to install the headers on the system
libscisdk_ladir = $(includedir)/scisdk

# the list of header files that belong to the library (to be installed later)
libscisdk_la_HEADERS =    \
                        SciSDK_DLL.h \
						NIErrorCode.h \
						scisdk_defines.h

# the sources to add to the library and to add to the source distribution
libscisdk_la_SOURCES =    \
                        $(libmylib_a_HEADERS) \               
						scisdk_DT5550W_petirocframe.cpp \
						scisdk_core.cpp \
						scisdk_digitizer.cpp  \
						scisdk_hal.cpp  \  
						scisdk_oscilloscope_dual.cpp  \
						scisdk_registers.cpp  \
						scisdk_DT5550W_citirocconfig.cpp  \
						scisdk_SciElement.cpp  \
						scisdk_custom_packet.cpp  \  
						scisdk_fftmon.cpp  \
						scisdk_list.cpp  \     
						scisdk_rate_meter.cpp \
						scisdk_scinode.cpp \
						scisdk_DT5550W_citirocframe.cpp \
						scisdk_TOFspectrum.cpp \
						scisdk_device.cpp \
						scisdk_frame.cpp \
						scisdk_oscilloscope.cpp \
						scisdk_register.cpp \
						scisdk_spectrum.cpp \
						boards_driver/bd_dt1260.cpp \
						boards_driver/bd_feelib.cpp \
						boards_driver/bd_dt5560se.cpp \
						SciSDK_DLL.cpp 
						
###############################################################################
