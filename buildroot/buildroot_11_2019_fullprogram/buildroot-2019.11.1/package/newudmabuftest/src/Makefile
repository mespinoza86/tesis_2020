APP = newudmabuftest
CXX=arm-linux-gnueabihf-g++

# Add any other object files to this list below
APP_OBJS = newudmabuftest.o


#all:
#	g++ -o StreamTest StreamTest.cpp -lm -std=c++17 -Wall -O3 -march=native -Wpedantic -Wshadow -ftree-vectorize

#debug:
#	g++ -o StreamTest StreamTest.cpp -lm -std=c++17 -Wall -g -Wpedantic -fsanitize=address -fno-omit-frame-pointer -Wshadow

#clean:
#	rm  -f StreamTest.o StreamTest


all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CXX) -lm -std=c++17 -Wall -O3 -Wpedantic -Wshadow -ftree-vectorize $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

clean:
