CC := clang++
DIR := $(pwd)

init: init.cpp log.cpp log.h lua_manager.cpp lua_manager.h lua_value.cpp lua_value.h
	$(CC) --static -std=c++14 -Ilua/include/ init.cpp lua/liblua.a lua/liblualibs.a -o init
