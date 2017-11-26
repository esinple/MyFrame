PROJECT(STATSSERVER)

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
		-D_DEBUG
		-DBOOST_NO_CXX11_SCOPED_ENUMS
		-DBOOST_ALL_DYN_LINK
		)

	LINK_DIRECTORIES(
		../../lib/linux/Debug
		/usr/local/boost_1_56_0/lib
		/usr/local/libevent_2_0_22/lib
		)

	LINK_LIBRARIES(
		asan
		ubsan
		libmeplay.a
		librocketmq.a
		libevent.a
		libevent_core.a
		libevent_pthreads.a
		libevent_openssl.a
		libevent_extra.a
		libSignature.a
		libbsoncxx.a
		libmongocxx.a
		libprotobuf.a
		libjsoncpp.a
		pthread
		rt
		z
		libboost_filesystem.a
		libboost_log.a
		libboost_log_setup.a
		)

	ADD_EXECUTABLE(StatsServer_Debug ${SRC_LIST})

	TARGET_LINK_LIBRARIES(StatsServer_Debug
		boost_chrono
		boost_filesystem
		boost_iostreams
		boost_log
		boost_log_setup
		boost_serialization
		boost_system
		boost_thread
		)
else()
	ADD_DEFINITIONS(
		-std=c++17
		-g
		-rdynamic
		-O3
		-Wall
		-fPIC
		-DMONGOCXX_STATIC
		-DBSONCXX_STATIC
		-DBOOST_NO_CXX11_SCOPED_ENUMS
		-DBOOST_ALL_DYN_LINK
		)

	LINK_DIRECTORIES(
		../../lib/linux/Release
		)

	LINK_LIBRARIES(
		libmeplay.a
		libbsoncxx.a
		libevent.a
		libevent_core.a
		libmongocxx.a
		libprotobuf.a
		librocketmq.a
		libjsoncpp.a
		pthread
		rt
		z
		libboost_filesystem.a
		libboost_log.a
		libboost_log_setup.a
		)

	ADD_EXECUTABLE(StatsServer_Release ${SRC_LIST})

	TARGET_LINK_LIBRARIES(StatsServer_Release
		boost_chrono
		boost_filesystem
		boost_iostreams
		boost_log
		boost_log_setup
		boost_serialization
		boost_system
		boost_thread
		)
endif()

