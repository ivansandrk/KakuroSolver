#!/bin/bash
# g++ gui.cpp main.cpp sol.cpp FreeType.cpp -o kak -lGL -lGLU -lSDL -I/usr/include/freetype2/ -lftgl
g++ gui.cpp main.cpp sol.cpp -o kakuro -std=c++11 \
  -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_ttf.framework/Headers \
  -framework SDL2 -framework Cocoa -framework SDL2_ttf
