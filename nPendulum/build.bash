%bashrc
# g++ main.cpp -o a.out -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./a.out
emcc -o index.html main.cpp -Os -Wall ~/Installations/raylib/src/libraylib.a -I. -I/usr/include -L/ -L/usr/lib -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB