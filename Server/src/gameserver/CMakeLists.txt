cmake_minimum_required(VERSION 3.9)

PROJECT(GAMESERVER)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
AUX_SOURCE_DIRECTORY(../base SRC_LIST)
AUX_SOURCE_DIRECTORY(../netproto SRC_LIST)

SET(EXECUTABLE_OUTPUT_PATH ../../bin)

INCLUDE_DIRECTORIES(
    ../base/
    ../third_party/
    ../third_party/libevent/
    ../meplay/include/
    ../netproto/
    )

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    ADD_DEFINITIONS(
        -std=c++17
        -fsanitize=address
        -fsanitize=leak
        -g
        -rdynamic
        -O0
        -Wall
		-Wno-unused-function
        -fPIC
        -ggdb
		-DMONGOCXX_STATIC
	   	-DBSONCXX_STATIC
	   	-DMONGOC_STATIC
	   	-DBSON_STATIC
        -D_DEBUG
        )

    LINK_DIRECTORIES(
        ../../lib/linux/Debug
        )

    LINK_LIBRARIES(
        asan
        ubsan
        libmeplay.a
        libevent.a
        libevent_core.a
        libmongocxx-static.a
        libbsoncxx-static.a
        libmongoc-1.0.a
        libbson-1.0.a
        libprotobuf.a
        pthread
        rt
		sasl2
		ssl
		crypto
        )

    ADD_EXECUTABLE(GameServer_Debug ${SRC_LIST})
else()
    ADD_DEFINITIONS(
        -std=c++17
        -g
        -rdynamic
        -O3
        -Wall
		-Wno-unused-function
        -fPIC
        -DMONGOCXX_STATIC
        -DBSONCXX_STATIC
		-DMONGOC_STATIC
		-DBSON_STATIC
        )

    LINK_DIRECTORIES(
        ../../lib/linux/Release
        )

    LINK_LIBRARIES(
        libmeplay.a
        libevent.a
        libevent_core.a
        libmongocxx-static.a
        libbsoncxx-static.a
        libmongoc-1.0.a
        libbson-1.0.a
        libprotobuf.a
        pthread
        rt
		sasl2
		ssl
		crypto
        )

    ADD_EXECUTABLE(GameServer_Release ${SRC_LIST})
endif()

