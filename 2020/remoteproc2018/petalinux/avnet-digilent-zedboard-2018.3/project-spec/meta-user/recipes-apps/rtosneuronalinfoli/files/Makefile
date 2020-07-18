APP = rtosneuronalinfoli
# Add any other object files to this list below
APP_OBJS = rtosneuronalinfoli.o


#all:
#	g++ -O3 -pthread -march=native -std=c++17 -Wall src/infoli_sim_impl.cpp -o infoli_sim_impl -lm -ffast-math -fsanitize=address
#
#cluster:
#	icpc -O3 -inline-max-size=350 -qopt-report=5 -march=knl -std=c++17 -Wall src/infoli.cpp src/infoli_sim_impl.cpp -o infoli_sim_impl -lm -qopenmp -ffast-math


all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CXX) -lm -std=c++17 -Wall -O3 -pthread -lm -ffast-math  -Wpedantic -Wshadow -ftree-vectorize $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

clean:
