```
     _____ _____            _       _____
    |  ___|  ___|          (_)     |  ___|
    | |__ | |__ _ __   __ _ _ _ __ | |__
    |  __||  __| '_ \ / _` | | '_ \|  __|
    | |___| |__| | | | (_| | | | | | |___
    \____/\____/_| |_|\__, |_|_| |_\____/
                       __/ |
                      |___/
```

# EEnginE [![Build Status](<https://travis-ci.org/EEnginE/engine.svg?branch=master>)](<https://travis-ci.org/EEnginE/engine>)

EEnginE is (or will be) a simple - but fullfeatured - OpenGL 3D engine.
Providing modules for networking, physic and 2D/3D graphic.

# Denpendencies

-   CMake v2.8.11 or newer [<http://www.cmake.org/>]
-   The OpenGL headers (often in a mesa package)
-   Linux/UNIX (at least one of those):
    -   X11 and it's headers (including RandR)
    -   Wayland and it's headers (not yet supported)
    -   Mir and it's headers (not yet supported)
-   Boost v1.46.1 or newer [<http://www.boost.org/>]
-   gawk - needed for some additional utils (mawk wont work!)

-   GLEW - downloaded and built automatically [<http://glew.sourceforge.net/>]

# Installation

```
$ git clone https://github.com/EEnginE/engine
$ ./generate.sh
$ mkdir build
$ cd build
$ cmake <Additional CMake options> ..
$ make
$ make install
```

Alternatively, you can use ninja instead of make by adding `-G Ninja` to your cmake options.

# CMake options

## ENGINE

-   `-DCMAKE_INSTALL_PREFIX=/install/prefix`

-   `-DENGINE_VERBOSE=<0/1>`

    Special cmake output and verbose compiler messages:
      - 0: disabled - default
      - 1: enabled

-   `-DDISPLAY_SERVER=<which>`

    Possible options:
      - UNIX_X11: stable - default
      - WINDOWS: stable
      - UNIX_WAYLAND: not yet supported
      - UNIX_MIR: not yet supported


-   `-DENGINE_LINK_SHARED=<0/1>`

    Link the engine:
      - 0: static
      - 1: shared - default


-   `-DENGINE_BUILD_SHARED=<0/1>`

    Build the engine:
      - 0: static
      - 1: shared - default


-   `-DEXTRA_CXX_FLAGS=<CXX compiler options>`

-   `-DSANITIZERS=<sanitizers>`

    Select sanitizers to use. Only works with clang.

-   `-DSANITIZER_OPTIONS=<options>`

## GLEW

-   `-DGLEW_USE_DEFAULT=<0/1>`

    Use system GLEW:
      - 0: disabled - default
      - 1: enabled


-   `-DGLEW_ROOT=/path/to/GLEW`

## BOOST

-   `-DBoost_NO_SYSTEM_PATHS=<0/1>`

    To tell CMake to only search boost in `BOOST_ROOT`
      - 0: disabled - default
      - 1: enabled


-   `-DBOOST_ROOT=/path/to/Boost`

# Troubleshooting

### CMake can not find Boost

Try to set:
-   `-DBOOST_ROOT=/path/to/Boost`

### CMake complains about an outdated Boost version even though the newest Version has been compiled by myself

You can either remove your native boost installation or set
-    `-DBoost_NO_SYSTEM_PATHS=1`

and

-    `-DBOOST_ROOT=/path/to/Boost`

### CMake can not find GLEW

Try to set:
-    `-DGLEW_ROOT=/path/to/GLEW`

### I crosscompiled the project on Linux and want to test the result with WINE but WINE crashes immediately.

Wine doesnt like `boost::wregex` because a function was unimplemented:

-    [BUG-Report](http://bugs.winehq.org/show_bug.cgi?id=36617)

Try to update your WINE version.
Or set:

-    `-DEXTRA_CXX_FLAGS=-DUGLY_WINE_WORKAROUND`
