PROJECT(MEPLAY)

AUX_SOURCE_DIRECTORY(./src SRC_LIST)
AUX_SOURCE_DIRECTORY(./include SRC_LIST)
AUX_SOURCE_DIRECTORY(./evpp SRC_LIST)

INCLUDE_DIRECTORIES(
    ./include
    ../third_party
    ../third_party/libevent
	./evpp
    )

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    ADD_DEFINITIONS(
        -std=c++14
        -fsanitize=address
        -fsanitize=leak
        -g
        -rdynamic
        -O0
        -Wall
		-Wno-unused-function
        -fPIC
        -DMONGOCXX_STATIC
        -DBSONCXX_STATIC
        -DELPP_OS_UNIX
        -D_DEBUG
        )
    SET(LIBRARY_OUTPUT_PATH ../../lib/linux/Debug)
else()
    ADD_DEFINITIONS(
        -std=c++14
        -g
        -rdynamic
        -O3
        -Wall
		-Wno-unused-function
        -fPIC
        -DMONGOCXX_STATIC
        -DBSONCXX_STATIC
        -DELPP_OS_UNIX
        )
    SET(LIBRARY_OUTPUT_PATH ../../lib/linux/Release)
endif()


ADD_LIBRARY(meplay ${SRC_LIST})
