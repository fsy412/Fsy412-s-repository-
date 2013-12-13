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

#添加文件
FILE(GLOB SRC_LIST ./*.c *.cpp *.h *.hpp)

#执行生成  
ADD_LIBRARY(Thread STATIC ${SRC_LIST})

IF(NOT WIN32)
  TARGET_LINK_LIBRARIES(Thread pthread nsl)
ENDIF()