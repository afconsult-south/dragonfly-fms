CXX=arm-linux-gnueabihf-g++
#CXX=g++
CXXFLAGS= -c -g -Wall -pthread -std=c++11 
LDFLAGS=
VPATH = ../dragonfly/tools/nanopb-0.3.5-windows-x86/ ../dragonfly/sw/comms/protobuf/
CPPSOURCES = main.cpp charger.cpp flightcontrol.cpp com.cpp
CSOURCES = pb_decode.c pb_common.c dragonfly_fcb.pb.c
SOURCES = $(CPPSOURCES) $(CSOURCES)

INCLUDE_DIR=-I../dragonfly/sw/comms/protobuf/ -I../dragonfly/tools/nanopb-0.3.5-windows-x86/
 
OBJECTS=$(CPPSOURCES:.cpp=.o) $(CSOURCES:.c=.o) 
 
EXECUTABLE=flightmanager

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $< -o $@

%.o : %.c
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $< -o $@
	
.PHONY: clean
clean:
	rm -f *.o
	rm -f $(EXECUTABLE)

print-%  : ; @echo $* = $($*)