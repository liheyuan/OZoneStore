# Compiler and flags
CC = gcc
LD = gcc
CXXFLAGS := -g -D_FILE_OFFSET_BITS=64
LINKFLAGS :=  


# Objects for test
OBJS_STORE := $(patsubst %.c, %.o, $(wildcard ozcore/*.c))
STORE = ../bin/OZoneStore


# Objects for sort
OBJS_SORT := $(patsubst %.c, %.o, $(wildcard ozsort/*.c))
SORT = ../bin/OZoneSort 

# For all binary files
all: $(STORE) $(SORT)
$(STORE): $(OBJS_STORE)
	$(LD)  $(LINKFLAGS) $(OBJS_STORE) -o $(STORE)
$(SORT): $(OBJS_SORT)
	$(LD)  $(LINKFLAGS) $(OBJS_SORT) -o $(SORT)

%.o:%.c
	$(CC) $(CXXFLAGS) -c -o $@ $<

# Clear
.PHONY: all clean
clean:
	rm -rf $(OBJS_SORT) $(OJBS_SORT:.o=.d)  $(OBJS_STORE) $(OJBS_STORE:.o=.d) $(STORE) $(SORT)

