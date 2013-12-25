#包含的源文件目录
INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
  
  /usr/local/include
  /usr/include
)

#包含的库文件目录
LINK_DIRECTORIES(
  ${PROJECT_BINARY_DIR}/lib

  /usr/local/lib
  /use/lib
)

IF(WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF()

#添加文件
FILE(GLOB SRC_LIST simplesocket/*.c 
				   simplesocket/*.cpp 
				   simplesocket/*.h 
				   simplesocket/*hpp)

#执行生成  
ADD_LIBRARY(SimpleSocket STATIC ${SRC_LIST})

IF(NOT WIN32)
  TARGET_LINK_LIBRARIES(SimpleSocket pthread nsl)
ENDIF()

IF(SOLARIS)
  TARGET_LINK_LIBRARIES(SimpleSocket socket)
ENDIF()