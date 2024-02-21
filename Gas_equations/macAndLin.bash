%bashrc
g++ main.cpp -o a.out -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./a.out
&& gcc main.cpp -o a.exe -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./a.exe