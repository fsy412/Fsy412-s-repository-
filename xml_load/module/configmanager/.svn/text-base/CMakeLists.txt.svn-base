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
FILE(GLOB SRC_LIST ./*.c *.cpp *.h *.hpp)

#执行生成  
ADD_LIBRARY(ConfigManager STATIC ${SRC_LIST})

TARGET_LINK_LIBRARIES(ConfigManager Tinyxml Common)
