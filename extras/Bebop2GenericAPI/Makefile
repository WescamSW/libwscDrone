
###########################################################################
## Useful functions for parsing strings like 'repo.git', 'mybranch!repo.git'
# Returns the .git repo after optional separator [mybranch!]repo.git
repo = $(lastword $(subst !, ,$1))
# Returns the branch in 'mybranch!repo.git' if present, otherwise master
branch = $(if $(word 2,$(subst !, ,$1)), $(firstword $(subst !, ,$1)),master)
###########################################################################
# Set the library version
LIB_MAJOR_VER = 0
LIB_MINOR_VER = 0
LIB_PATCH_VER = 0
LIB_VER = $(LIB_MAJOR_VER).$(LIB_MINOR_VER).$(LIB_PATCH_VER)


# Check for environment variables.
ifndef SHELL
SHELL = /usr/bin/sh
endif

ifndef ARCH
ARCH = $(shell uname -m)
endif

# ifndef ARSDK3
# ARSDK3=.
# endif

ifndef LIBWSCDRONE
LIBWSCDRONE= /home/hackathon/libraries/libwscDrone
endif

OBJDIR = $(CURDIR)/obj/
PREFIX = usr/local/include/
DISTDIR = $(CURDIR)/dist/
DIST_DIR = $(CURDIR)/dist/
DEPDIR = $(CURDIR)/deps/
OUTPUT_DIRS = $(CURDIR)/obj/ $(CURDIR)/dist/lib $(CURDIR)/dist/include/$(TARGET_NAME)
OUTPUT_DIRS += $(CURDIR)/deps $(DISTDIR)
# OUTPUT_DIRS = $(CURDIR)/deps $(DISTDIR)/$(PREFIX)
SYS_INC = -I/usr/local/include
BINDINGS_INCDIR = $(CURDIR)/inc/
BINDINGS_SRCDIR = $(CURDIR)/src/

ifndef DESTDIR
export DESTDIR =$(DISTDIR)
endif

CC = $(TOOL_PREFIX)g++
AR = $(TOOL_PREFIX)ar
LD = $(TOOL_PREFIX)ld


ifndef DIST_LIBDIR
# mkdir $(DIST_DIR)/lib
DIST_LIBDIR = $(DIST_DIR)/lib/
endif

TARGET_NAME = wscDroneBindings

DYN_TARGET_LIST    = lib$(TARGET_NAME).so.$(LIB_VER)
STATIC_TARGET_LIST = lib$(TARGET_NAME).a
DYN_TARGET    = $(addprefix $(DIST_LIBDIR), $(DYN_TARGET_LIST))
STATIC_TARGET = $(addprefix $(DIST_LIBDIR), $(STATIC_TARGET_LIST))

FLAGS += -std=c++14 -Wall -pedantic -O2 -Wno-deprecated-declarations
LOC_INC = -I$(DISTDIR)/$(PREFIX)/include #-I$(ARSDK3)/include
LOC_LIB = -L$(DISTDIR)/$(PREFIX)/lib #-L$(ARSDK3)/lib

SYS_INC = -I/usr/local/include
SYS_LIB = -L/usr/local/lib
SYS_INC = -I$(LIBWSCDRONE)/include

SYS_LIB += -lpthread -lrtsp -lsdp -lmux -lpomp -ljson-c -lulog -lfutils
SYS_LIB += -L$(LIBWSCDRONE)/lib -Wl, --whole-archive -lwscDrone

LOCAL_INCDIR = $(CURDIR)/inc/
SYS_LIBS_DIRS = -L=/usr/local/lib #-L$(ARSDK3)/lib -L/usr/bin/python3.6
#COMMON_FLAGS += -pedantic -Wall -Wextra -Wno-deprecated-declarations
COMMON_FLAGS += -Wno-deprecated-declarations
# shared library flags
COMMON_FLAGS += -shared -fPIC
CPPFLAGS     += -I$(LOCAL_INCDIR) 
CXXFLAGS     += -std=c++14 $(COMMON_FLAGS)
LDFLAGS      += -shared -fPIC

DEBUGFLAGS     = -ggdb3 -O0 -D _DEBUG
RELEASEFLAGS   = -O3 -D NDEBUG
DEFAULTFLAGS   = -O3 -D NDEBUG
#Extra Library Source Files For The Generic API IF
CPP_BINDINGS_HEADER_LIST = Bebop2CtrlIF \
                             Bebop2FrameIF
CPP_BINDINGS_SRC_LIST = Bebop2CtrlIF \
                             Bebop2FrameIF
BINDING_HEADERS = $(addsuffix .h, $(addprefix $(BINDINGS_INCDIR), $(CPP_BINDINGS_HEADER_LIST)))
SOURCES_BINDINGS_CPP = $(addsuffix .cpp, $(addprefix $(BINDINGS_SRCDIR), $(CPP_BINDINGS_SRC_LIST)))
OBJECTS_BINDINGS_CPP = $(addsuffix .o, $(addprefix $(OBJDIR), $(CPP_BINDINGS_SRC_LIST)))
all: directories binding_headers $(DYN_TARGET) $(STATIC_TARGET)  
	@echo $(DYN_TARGET)
directories:
	mkdir -p $(OUTPUT_DIRS)


#no longer in use
# binding_sources:
# 	${CC} ${FLAGS} -o ${TARGET_NAME} ${SOURCES_BINDINGS_CPP} ${SYS_INC} ${LOC_INC} ${LOC_LIB} ${SYS_LIB}

binding_headers:
	cp -f $(BINDING_HEADERS) $(DIST_INCDIR)/$(PREFIX)

$(DYN_TARGET):  $(OBJECTS_BINDINGS_CPP) 
	$(CXX) $(LDFLAGS) -o $(DYN_TARGET)  -Wl,-Bstatic $(SYS_STAT_LIBS) -Wl,-Bdynamic $(SYS_DYN_LIBS)
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so.$(LIB_MAJOR_VER)
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so.$(LIB_MAJOR_VER).$(LIB_MINOR_VER)

$(STATIC_TARGET):  $(OBJECTS_CU) $(OBJECTS_BINDINGS_CPP)
	$(AR) $(ARFLAGS) $(STATIC_TARGET)  $(OBJECTS_BINDINGS_CPP) 

$(OBJDIR)%.o: $(BINDINGS_SRCDIR)%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEFAULTFLAGS) -c -o $@ $<
printvar:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))

clean:
	-rm -f  $(OBJECTS_BINDINGS_CPP) 