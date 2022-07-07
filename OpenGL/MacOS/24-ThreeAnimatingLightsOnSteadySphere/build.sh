mkdir -p OGL.app/Contents/MacOS
clang++ -Wno-deprecated-declarations -o OGL.app/Contents/MacOS/OGL OGL.mm Sphere.cpp -framework Cocoa -framework QuartzCore -framework OpenGL
OGL.app/Contents/MacOS/OGL