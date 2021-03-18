SDL_DEP = `sdl2-config --libs --cflags`
OPENGL_DEP = -framework opengl
GLEW_DEP = `pkg-config --libs --cflags glew`
SRC = $(wildcard src/*.cpp)
TESTS = $(patsubst %.cpp, %.out, $(wildcard test/*.cpp))
DEBUG = -g

all:${TESTS}

%.out:%.cpp test/app.hpp
	$(CXX) $< ${SRC} -o $@ -Iinclude -Itest ${SDL_DEP} ${OPENGL_DEP} ${GLEW_DEP} ${DEBUG} -std=c++17

.PHONY:clean
clean:
	-rm test/*.out

rebuild: clean all
