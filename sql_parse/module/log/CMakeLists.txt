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

#设置工程输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#指定编译类型
SET(CMAKE_BUILE_TYPE DEBUG) 

IF(WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -m32") 
	SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g -m32")
ENDIF()

#添加文件
FILE(GLOB SRC_LIST ./*.c *.cpp *.h *.hpp)

#执行生成  
ADD_LIBRARY(Log STATIC ${SRC_LIST})
