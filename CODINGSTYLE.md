```
     _____           _ _               _____ _         _
    /  __ \         | (_)             /  ___| |       | |
    | /  \/ ___   __| |_ _ __   __ _  \ `--.| |_ _   _| | ___
    | |    / _ \ / _` | | '_ \ / _` |  `--. \ __| | | | |/ _ \
    | \__/\ (_) | (_| | | | | | (_| | /\__/ / |_| |_| | |  __/
     \____/\___/ \__,_|_|_| |_|\__, | \____/ \__|\__, |_|\___|
                                __/ |             __/ |
                               |___/             |___/
```

# Return Values

Functions should return 0 if they succeed and an error code if not.
Alternatively they can also return a boolean value (true or false).

# Classes

## Deleting Constructors

If you want to delete constructors, don't make them private. Instead use the
C++11 key word delete.

Example:

```cpp
class myClass {
   private:
      int vAnInt;

      public:
         myClass() = delete;
         myClass( int _a ) : vAnInt( _a ) {}
};
```

## Constructors

Classes managing OpenGL objects should always have a DELETED copy constructor
and a move constructor.

This will prevent silly errors, when an object is copied (e.g. passed by value)
and then deleted. As a result the destructor will be called before the main object
is deleted and the OpenGL data will be removed more than once!

With a removed copy constructor, the compiler will print a compile time error,
when copying.

You should use something like this:

```cpp
// Forbid copying
myClass( const myClass & ) = delete;
myClass &operator=( const myClass & ) = delete;

// Allow moving
myClass( myClass&& ) {}
myClass &operator=( myClass&& ) {return *this;}
```

# Includes and etc

## ALWAYS include defines.hpp and include it first

You should **ALWAYS** include defines.hpp as the first header file. There are some specific
(system) header files requiring some defines (cmath or math.h and M_PI). All this defines
will be handled in defines.hpp.

## Move all defines and macros to defines.in.hpp

All defines, global enums and constants shoud be located in defines.in.hpp.
