
CXX = g++
TARGET_DIR =../bin
UNGINE_SDK_PATH = $(SDK_PATH)
CXXFLAGS = -std=c++14 -msse -msse2 -msse4.2 -Wall -I"$(UNGINE_SDK_PATH)include" -I./include $(EXTRA_DEFINES) -D_LINUX
LDFLAGS = -Wl,-rpath,'$$ORIGIN' -L$(UNGINE_SDK_PATH)bin
SOURCES:= $(shell find . -name "*.cpp")
OBJS:=$(patsubst %.cpp,%.o,$(SOURCES))

ARCH:=x64

TARGET=$(TARGET_DIR)/$(PROJECT_NAME)_$(ARCH)
UNIGINE_LIB=Unigine$(DOUBLE)_$(ARCH)

#
# Debug build settings
#
DBGDIR = debug
DBGAPP = $(TARGET)d
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCXXFLAGS=-g -DDEBUG
DBGLIB=-l$(UNIGINE_LIB)d

#
# Release build settings
#
RELDIR = release
RELAPP = $(TARGET)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCXXFLAGS=-O2 -DNDEBUG
RELLIB=-l$(UNIGINE_LIB)

.PHONY: all clean debug release rebuild

# Default build
all: release

#
# Debug rules
#
debug: $(DBGAPP)

$(DBGAPP): $(DBGOBJS)
	$(CXX) $^ $(LDFLAGS) $(DBGLIB) -o $@

$(DBGOBJS): $(DBGDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DBGCXXFLAGS) -c -o $@ $<

#
# Release rules
#
release: $(RELAPP)

$(RELAPP): $(RELOBJS)
	$(CXX) $^ $(LDFLAGS) $(RELLIB) -o $@

$(RELOBJS): $(RELDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(RELCXXFLAGS) -c -o $@ $<

#
# Other rules
#

rebuild: clean all

clean:
	rm -f $(RELAPP) $(DBGAPP)
	rm -rf $(RELDIR) $(DBGDIR)

