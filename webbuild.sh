emcc -o ./buildweb/game.html ./src/main.cpp ./src/utils.cpp -Os -Wall ./build/external/raylib-master/src/libraylib.a -I./src/ -I./build/external/raylib-master/src/ -L. -L./build/external/raylib-master/src/libraylib.a -s USE_GLFW=3 --shell-file ./build/external/raylib-master/src/shell.html -DPLATFORM_WEB