mkdir -p OGL.app/Contents/MacOS
clang -o OGL.app/Contents/MacOS/OGL OGL.m -framework Cocoa
OGL.app/Contents/MacOS/OGL