# Script adapted from https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
CC := /usr/local/classes/eecs/fall2018/cs472/public/gcc/bin/g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/cache_sim
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
#CFLAGS := -g -Wall -Wl,-rpath,/usr/local/classes/eecs/fall2018/cs472/public/gcc/lib64
CFLAGS := -g -Wall
LDFLAGS := -Wl,-rpath,/usr/local/classes/eecs/fall2018/cs472/public/gcc/lib64
#LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
LIB := -pthread
INC := -I $(SRCDIR)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB) $(LDFLAGS)"; $(CC) $^ -o $(TARGET) $(LIB) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean