set(GCC_AND_CLANG_WARNINGS
	-Werror
	-Wall
	-Wextra
	-pedantic
	-pedantic-errors
	-Wswitch-enum
	-Wuninitialized
	-Wold-style-cast
	-Wsign-promo
	-Wunused
	-Wunused-parameter
	-Wunused-result
	-Wshadow
	-Wctor-dtor-privacy
	-Wdelete-non-virtual-dtor
	-Wnarrowing
	-Wnon-virtual-dtor
	-Woverloaded-virtual
	-Wformat
	-Wundef
	-Wcast-qual
	-Wcast-align
	-Wwrite-strings
	-Wconversion-null
	-Wempty-body
	-Wenum-compare
	-Wredundant-decls
	-Winvalid-pch
	-Wvla
	-Wdisabled-optimization
	-Wno-error=unused-function
	-Wmissing-field-initializers
	-Werror=float-conversion)

set(GCC_ONLY_WARNINGS
    -Wno-error=maybe-uninitialized
    -Wno-error=implicit-fallthrough
    -Wunused-local-typedefs
    -Wlogical-op
    -Wsign-conversion
    -Wconversion
)
set(CLANG_ONLY_WARNINGS
    -Wunused-private-field
    -Wdocumentation)

set(MSVC_ONLY_WARNINGS
    /W4
    # Включаем ворнинги которых нет в /W4:
    /we4242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
    /we4254 # 'operator': conversion from 'type1' to 'type2', possible loss of data
    /we4263 # 'function': member function does not override any base class virtual member function
    /we4264 # 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
    /we4266 # 'function': no override available for virtual member function from base 'type'; function is hidden
    /we4287 # 'operator': unsigned/negative constant mismatch
    /we4296 # 'operator': expression is always false
    /we4302 # 'conversion': truncation from 'type1' to 'type2'
    /we4311 # 'variable' : pointer truncation from 'type' to 'type'
    /we4312 # 'operation' : conversion from 'type1' to 'type2' of greater size
    /we4342 # behavior change: 'function' called, but a member operator was called in previous versions
    /we4388 # signed/unsigned mismatch
    /we4412 # 'function': function signature contains type 'type'; C++ objects are unsafe to pass between pure code and mixed or native
    /we4437 # dynamic_cast from virtual base 'class1' to 'class2' could fail in some contexts
    /we4536 # 'type name': type-name exceeds meta-data limit of 'limit' characters
    /we4545 # expression before comma evaluates to a function which is missing an argument list
    /we4546 # function call before comma missing argument list
    /we4547 # 'operator': operator before comma has no effect; expected operator with side-effect
    # \todo: Это предупреждение ломает компиляцию из-за повсеместного включения malloc.h.
    # /we4548 # expression before comma has no effect; expected expression with side-effect
    /we4549 # 'operator': operator before comma has no effect; did you intend 'operator'?
    /we4555 # expression has no effect; expected expression with side-effect
    /we4608 # 'symbol1' has already been initialized by another union member in the initializer list, 'symbol2'
    /we4928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
    /we4988 # 'symbol': variable declared outside class/function scope
    /we4706 # assignment within conditional expression
    # Отключаем ненужные ворнинги.
    /wd4512 # 'class' : assignment operator could not be generated
    /wd4510 # 'class' : default constructor could not be generated
    /wd4623 # 'derived class': default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
    /wd4503 # 'identifier' : decorated name length exceeded, name was truncated
    /wd4201 # nonstandard extension used : nameless struct/union
    /wd4505 # 'function' : unreferenced local function has been removed.
)

add_library(warnings INTERFACE)
target_compile_options(warnings INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:${MSVC_ONLY_WARNINGS}>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang,AppleClang>>:${GCC_AND_CLANG_WARNINGS}>
    $<$<CXX_COMPILER_ID:GNU>:${GCC_ONLY_WARNINGS}>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:${CLANG_ONLY_WARNINGS}>
)
