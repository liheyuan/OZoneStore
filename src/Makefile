# Compiler and flags
CC = gcc
LD = gcc
CXXFLAGS := -O3 -D_FILE_OFFSET_BITS=64
LINKFLAGS :=  

# Objects for test
OBJS := $(patsubst %.c, %.o, $(wildcard *.c)) \
		$(patsubst %.c, %.o, $(wildcard ozcore/*.c))
TEST = ../bin/TestOzoneStore

# Objects for sort
OBJS_SORT := $(patsubst %.c, %.o, $(wildcard ozsort/*.c))
SORT = ../bin/OZoneSort 

# For test file
all: $(TEST) $(SORT)
 
$(TEST): $(OBJS)
	$(LD) $(OBJS) $(LINKFLAGS) -o $(TEST)

$(SORT): $(OBJS_SORT)
	$(LD) $(OBJS_SORT) $(LINKFLAGS) -o $(SORT)

%.o:%.c
	$(CC) $(CXXFLAGS) -c -o $@ $<
 
.PHONY: all clean
clean:
	rm -rf $(OBJS) $(OJBS:.o=.d) $(TEST) $(SORT)
