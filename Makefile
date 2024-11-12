# Non-volatile memory simulator

target := destiny

# define tool chain
CXX := g++
RM := rm -rf

# Directory structure
SRCDIR := src
INCDIR := include
BUILDDIR := build

# Source directories
SRCDIRS := $(SRCDIR) \
           $(SRCDIR)/core \
           $(SRCDIR)/decoders \
           $(SRCDIR)/peripherals \
           $(SRCDIR)/tsv \
           $(SRCDIR)/utils

# Include directories - add all subdirectories
INCDIRS := $(INCDIR) \
           $(INCDIR)/core \
           $(INCDIR)/decoders \
           $(INCDIR)/peripherals \
           $(INCDIR)/tsv \
           $(INCDIR)/utils

# define build options
# compile options
CXXFLAGS := -Wall $(foreach d, $(INCDIRS), -I$d)
# link options
LDFLAGS :=
# link libraries
LDLIBS :=

# Find all source files
SRC := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))
# Generate object file names
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRC))
# Generate dependency files
DEP := $(OBJ:.o=.d)

# Create subdirectories in build directory
BUILDDIRS := $(patsubst $(SRCDIR)%,$(BUILDDIR)%,$(SRCDIRS))

# Default target
.PHONY: all clean dbg dir
all: CXXFLAGS += -g -O3 -mtune=native
all: dir $(target)

# Debug build
dbg: CXXFLAGS += -ggdb -g
dbg: dir $(target)

# Create build directories
dir:
	@mkdir -p $(BUILDDIRS)

# Link the target
$(target): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compile source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Clean build files
clean:
	$(RM) $(target) $(BUILDDIR)

# Include dependency files
-include $(DEP)