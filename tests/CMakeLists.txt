cmake_minimum_required( VERSION 3.5.2 )
project( SafeChat_Tests )

# Disable MSVC warnings
if( MSVC )
	add_definitions(
		-D_CRT_SECURE_NO_WARNINGS
		-D_SCL_SECURE_NO_WARNINGS
	)
endif()

# Sanitizers
if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" )
	set( CMAKE_C_FLAGS	"${CMAKE_C_FLAGS} -fsanitize=address,undefined" )
	set( CMAKE_C_FLAGS	"${CMAKE_C_FLAGS} -fsanitize=address,undefined" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined" )
endif()

# libsafechat
set( LIBSAFECHAT_HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/../BigNumber.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Vector.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../rsa.h"
)

set( LIBSAFECHAT_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/../BigNumber.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../rsa.cpp"
)

add_library( safechat STATIC
	${LIBSAFECHAT_HEADER_FILES}
	${LIBSAFECHAT_SOURCE_FILES}
)

target_include_directories( safechat PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.." )


# Tests
add_executable( safechat_tests
	src/main.cpp
	src/catch.hpp
	src/string.cpp
	src/bigsmoke.cpp
	src/vector.cpp
	src/rsa.cpp
)

target_link_libraries( safechat_tests
	safechat
)
