cmake_minimum_required( VERSION 3.5.2 )
project( SafeChat_Tests )

# libsafechat
set( LIBSAFECHAT_HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/../BigNumber.h"
)

set( LIBSAFECHAT_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/../BigNumber.cpp"
)

add_library( safechat STATIC
	${LIBSAFECHAT_HEADER_FILES}
	${LIBSAFECHAT_SOURCE_FILES}
)

target_include_directories( safechat PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.." )


# Tests
add_executable( safechat_tests
	src/main.cpp
	src/string.cpp
)

target_link_libraries( safechat_tests
	safechat
)
