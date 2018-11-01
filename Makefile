all:
	./build_shader.sh
	clang++ -v -x objective-c++ -framework Foundation -framework AppKit -framework Metal -framework MetalKit -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/ -I/usr/local/include -L/usr/local/lib -I/opt/X11/include -lSDL2 main.cpp
