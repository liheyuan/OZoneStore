# OS Type
OS = 64
# Compiler and flags
CXX = g++
LD = g++ 
CXXFLAGS := -D_FILE_OFFSET_BITS=64 -I../..//include_deps/thrift -L../lib_deps/lib/$(OS)
LINKFLAGS := -lthrift 

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

# For all binary files
all: $(WRITE_SERVER) $(READ_SERVER)

$(WRITE_SERVER): $(OBJS_WRITE)
	$(LD) $(CXXFLAGS) $(LINKFLAGS) $(OBJS_WRITE) -o $(WRITE_SERVER)

$(READ_SERVER): $(OBJS_READ)
	$(LD) $(CXXFLAGS) $(LINKFLAGS) $(OBJS_READ) -o $(READ_SERVER)

%.o:%.c
	$(CXX) $(CFLAGS) -c -o $@ $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clear
.PHONY: all clean
clean:
	rm -rf $(OBJS_WRITE) $(OJBS_WRITE:.o=.d) $(OBJS_READ) $(OJBS_READ:.o=.d) $(WRITE_SERVER) $(READ_SERVER)

