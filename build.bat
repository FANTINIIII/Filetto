@echo off
cd obj
g++ -c -I../ext/ ../src/main.c 
g++ -c -I../ext/ ../src/game.c
cd ..
g++ -o main ./obj/main.o ./obj/game.o -LC:\raylib\w64devkit\x86_64-w64-mingw32\lib\ -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32