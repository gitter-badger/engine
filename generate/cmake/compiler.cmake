
########################################################################################
############################################################################################################################
#####                ################################################################################################################
###  Compiler Options  #################################################################################################################
#####                ################################################################################################################
############################################################################################################################
########################################################################################


if( NOT ENGINE_LINK_SHARED )
   set( DEF_GLEW_STATIC_GCC   "-DGLEW_STATIC" )
   set( DEF_GLEW_STATIC_CLANG "-DGLEW_STATIC" )
   set( DEF_GLEW_STATIC_INTEL "<insert here>" ) # TODO
   set( DEF_GLEW_STATIC_MSVC  "<insert here>" ) # TODO
endif( NOT ENGINE_LINK_SHARED )

if( ENGINE_VERBOSE )
   set( GCC_VERBOSE   "--verbose" )
   set( CLANG_VERBOSE "-v" )
   set( INTEL_VERBOSE "<verbose flag>" ) # TODO
   set( MSVC_VERBOSE  "<verbose flag>" ) # TODO
endif( ENGINE_VERBOSE )


set( ENGINE_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION} )



# Options GCC

# CMAKE_CXX_COMPILER_VERSION doesent work always defined, so this is needed
if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
   if( NOT DEFINED CMAKE_CXX_COMPILER_VERSION )
      execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_COMPILER_VERSION)
   else( NOT DEFINED CMAKE_CXX_COMPILER_VERSION )
      set( GCC_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION} )
   endif( NOT DEFINED CMAKE_CXX_COMPILER_VERSION )
   
   set( ENGINE_COMPILER_VERSION ${GCC_COMPILER_VERSION} )
endif( CMAKE_CXX_COMPILER_ID MATCHES GNU )

set( GCC_ENABLE_CXX_11 "-std=c++11" )
if( CMAKE_CXX_COMPILER_ID MATCHES GNU AND GCC_COMPILER_VERSION LESS 4.7 )
   if( GCC_COMPILER_VERSION LESS 4.5 )
      message( FATAL_ERROR "Your GCC compiler version is to old! You need at least GCC v4.5" )
   endif( GCC_COMPILER_VERSION LESS 4.5 )
   set( GCC_ENABLE_CXX_11 "-std=c++0x" )
endif( CMAKE_CXX_COMPILER_ID MATCHES GNU AND GCC_COMPILER_VERSION LESS 4.7 )

set( GNU_COMPILER_OPTIONS_DEBUG   "-Wall ${GCC_ENABLE_CXX_11} ${DEF_GLEW_STATIC_GCC} ${GCC_VERBOSE}" )
set( GNU_COMPILER_OPTIONS_RELEASE "-Wall ${GCC_ENABLE_CXX_11} ${DEF_GLEW_STATIC_GCC} ${GCC_VERBOSE} -flto -O3" )

if( DEFINED EXTRA_WARNINGS )
   set( GNU_COMPILER_OPTIONS_DEBUG   "-Winline -Werror ${GNU_COMPILER_OPTIONS_DEBUG}" )
   set( GNU_COMPILER_OPTIONS_RELEASE "-Winline -Werror ${GNU_COMPILER_OPTIONS_RELEASE}" )
endif( DEFINED EXTRA_WARNINGS )




# Options LLVM
set( CLANG_ENABLE_CXX_11 "-std=c++11")

set( CLANG_COMPILER_OPTIONS_DEBUG   "${CLANG_ENABLE_CXX_11} ${DEF_GLEW_STATIC_CLANG} ${CLANG_VERBOSE}" )
set( CLANG_COMPILER_OPTIONS_RELEASE "${CLANG_ENABLE_CXX_11} ${DEF_GLEW_STATIC_CLANG} ${CLANG_VERBOSE}" )

if( DEFINED EXTRA_WARNINGS )
   set( CLANG_COMPILER_OPTIONS_DEBUG   "-Werror -Wthread-safety ${CLANG_COMPILER_OPTIONS_DEBUG}" )
   set( CLANG_COMPILER_OPTIONS_RELEASE "-Werror -Wthread-safety ${CLANG_COMPILER_OPTIONS_RELEASE}" )
endif( DEFINED EXTRA_WARNINGS )





# Options ICC  - TODO

set( INTEL_COMPILER_OPTIONS_DEBUG   "<insert here> ${DEF_GLEW_STATIC_INTEL} ${INTEL_VERBOSE}" )
set( INTEL_COMPILER_OPTIONS_RELEASE "<insert here> ${DEF_GLEW_STATIC_INTEL} ${INTEL_VERBOSE}" )

if( DEFINED EXTRA_WARNINGS )
   set( INTEL_COMPILER_OPTIONS_DEBUG   "<insert here> ${INTEL_COMPILER_OPTIONS_DEBUG}" )
   set( INTEL_COMPILER_OPTIONS_RELEASE "<insert here> ${INTEL_COMPILER_OPTIONS_RELEASE}" )
endif( DEFINED EXTRA_WARNINGS )




# Options MSVC - TODO

set( MSVC_COMPILER_OPTIONS_DEBUG   "<insert here> ${DEF_GLEW_STATIC_MSVC} ${MSVC_VERBOSE}" )
set( MSVC_COMPILER_OPTIONS_RELEASE "<insert here> ${DEF_GLEW_STATIC_MSVC} ${MSVC_VERBOSE}" )

if( DEFINED EXTRA_WARNINGS )
   set( MSVC_COMPILER_OPTIONS_DEBUG   "<insert here> ${MSVC_COMPILER_OPTIONS_DEBUG}" )
   set( MSVC_COMPILER_OPTIONS_RELEASE "<insert here> ${MSVC_COMPILER_OPTIONS_RELEASE}" )
endif( DEFINED EXTRA_WARNINGS )






# Detect Compiler

if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
   set( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${GNU_COMPILER_OPTIONS_DEBUG}" )
   set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${GNU_COMPILER_OPTIONS_RELEASE}" )

elseif( CMAKE_CXX_COMPILER_ID MATCHES Clang )
   set( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${CLANG_COMPILER_OPTIONS_DEBUG}" )   # Uncomment this when done with LLVM/Clang
   set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CLANG_COMPILER_OPTIONS_RELEASE}" ) # Uncomment this when done with LLVM/Clang

elseif( CMAKE_CXX_COMPILER_ID MATCHES Intel )
   #set( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${INTEL_COMPILER_OPTIONS_DEBUG}" )   # Uncomment this when done with Intel
   #set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${INTEL_COMPILER_OPTIONS_RELEASE}" ) # Uncomment this when done with Intel
   message( WARNING "Compiler flags for ${CMAKE_CXX_COMPILER_ID} not (yet) supported" )           # Remove this when done with Intel

elseif( CMAKE_CXX_COMPILER_ID MATCHES MSVC )
   #set( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${MSVC_COMPILER_OPTIONS_DEBUG}" )    # Uncomment this when done with MSVC
   #set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_COMPILER_OPTIONS_RELEASE}" )  # Uncomment this when done with MSVC
   message( WARNING "Compiler flags for ${CMAKE_CXX_COMPILER_ID} not (yet) supported" )           # Remove this when done with MSVC

else()
   message( WARNING "Unknown compiler ${CMAKE_CXX_COMPILER_ID}" )
endif()

