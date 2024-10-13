aux_source_directory(. SRC_LIST)
# 生成一个动态库
add_library(mprpc SHARED ${SRC_LIST})