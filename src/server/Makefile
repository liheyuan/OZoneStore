# OS Type
OS = $(shell getconf LONG_BIT)
# Compiler and flags
CXX = g++
LD = g++ 
CXXFLAGS := -D_FILE_OFFSET_BITS=64 -I../../include_deps/thrift -I../../include_deps/libevent
LINKFLAGS := -lpthread
LIBS_THRIFT := ../../lib_deps/$(OS)/libthriftnb.a
LIBS_THRIFT += ../../lib_deps/$(OS)/libthrift.a
LIBS_THRIFT += ../../lib_deps/$(OS)/librt.a
LIBS_THRIFT += ../../lib_deps/$(OS)/libevent_core.a


# Objects for WriteServer
OBJS_WRITE := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJS_WRITE += ../ozcore/ozwrite.o
OBJS_WRITE += OZWriteService_server.skeleton.o
WRITE_SERVER = ../../bin/OZoneWriteServer

# Objects for WriteServer
OBJS_READ := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJS_READ += ../ozcore/ozread.o
OBJS_READ += OZReadService_server.skeleton.o
READ_SERVER = ../../bin/OZoneReadServer

# Objects for TravServer
OBJS_TRAV := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJS_TRAV += ../ozcore/oztrav.o
OBJS_TRAV += OZTravService_server.skeleton.o
TRAV_SERVER = ../../bin/OZoneTravServer

# For all binary files
all: $(WRITE_SERVER) $(READ_SERVER) $(TRAV_SERVER)

$(WRITE_SERVER): $(OBJS_WRITE)
	$(LD) $(CXXFLAGS) $(OBJS_WRITE) $(LINKFLAGS) $(LIBS_THRIFT) -o $(WRITE_SERVER)

$(READ_SERVER): $(OBJS_READ)
	$(LD) $(CXXFLAGS) $(LINKFLAGS) $(OBJS_READ) $(LIBS_THRIFT) -o $(READ_SERVER)

$(TRAV_SERVER): $(OBJS_TRAV)
	$(LD) $(CXXFLAGS) $(LINKFLAGS) $(OBJS_TRAV) $(LIBS_THRIFT) -o $(TRAV_SERVER)

%.o:%.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clear
.PHONY: all clean
clean:
	rm -rf $(OBJS_WRITE) $(OJBS_WRITE:.o=.d) $(OBJS_READ) $(OJBS_READ:.o=.d) $(OBJS_TRAV) $(OJBS_TRAV:.o=.d) $(WRITE_SERVER) $(READ_SERVER) $(TRAV_SERVER)

