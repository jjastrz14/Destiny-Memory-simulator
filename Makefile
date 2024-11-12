# Non-volatile memory simulator

target := destiny

# define tool chain
CXX := g++
RM := rm -rf

# Directory structure
SRCDIR := src
INCDIR := include
BUILDDIR := build

# Debug and optimization flags
DEBUG_FLAGS := -ggdb -g -DDEBUG
SANITIZE_FLAGS := -fsanitize=address -fsanitize=undefined
EXTRA_DEBUG_FLAGS := -Wall -Wextra -Wpedantic -Werror
OPTIMIZE_FLAGS := -O3 -mtune=native

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

# Base compile options
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

# Build types
.PHONY: all clean cleanall debug debugsanitize debugstrict dir

# Default release build (optimized)
all: CXXFLAGS += $(OPTIMIZE_FLAGS)
all: dir $(target)

# Basic debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: dir $(target)

# Debug build with sanitizers
debugsanitize: CXXFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
debugsanitize: LDFLAGS += $(SANITIZE_FLAGS)
debugsanitize: dir $(target)

# Strict debug build with all warnings as errors
debugstrict: CXXFLAGS += $(DEBUG_FLAGS) $(EXTRA_DEBUG_FLAGS)
debugstrict: dir $(target)

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

# Clean only build directory
clean:
	@echo "Cleaning build files..."
	$(RM) $(target) $(BUILDDIR)
	@echo "Cleaning Makefile dependency file..."
	$(RM) Makefile.dep

# Clean everything including old obj directory
cleanall: clean
	@echo "Cleaning old obj directory..."
	$(RM) obj
	@echo "Cleaning all temporary and object files..."
	find . -name '*~' -delete
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	@echo "Clean complete!"

# Print variables for debugging
print-%:
	@echo $* = $($*)

# Include dependency files
-include $(DEP)