CXX=clang++
CXX_FLAGS=-std=c++20 -Iincludes -g -fstandalone-debug -O0 -Wall -Wextra -Werror

exec: bin/exec

bin/exec: ./src/jd.cpp 
	$(CXX) $(CXX_FLAGS) $^ -o $@


.DEFAULT_GOAL := exec
.PHONY: exec tests clean

clean:
	rm -f bin/*
