CC := clang++
DIR := $(pwd)

init: init.cpp lua_manager.cpp log.cpp
	$(CC) --static -std=c++14 -Ilua/include/ init.cpp lua/liblua.a lua/liblualibs.a -o init
