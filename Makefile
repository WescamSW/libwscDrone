# Set the library version
LIB_MAJOR_VER = 0
LIB_MINOR_VER = 0
LIB_PATCH_VER = 0
LIB_VER = $(LIB_MAJOR_VER).$(LIB_MINOR_VER).$(LIB_PATCH_VER)


# Check for environment variables.
ifndef SHELL
SHELL = /usr/bin/sh
endif

ifndef DESTDIR
DESTDIR = 
endif

ifndef PREFIX
PREFIX = /usr/local/
endif

ifndef CROSS_COMPILE
TOOL_PREFIX=
else
TOOL_PREFIX=$(CROSS_COMPILE)
endif

ifndef ARCH
ARCH = $(shell uname -m)
endif

ifndef ARSDK3
ARSDK3=.
endif

ifndef DIST_DIR
DIST_DIR = $(CURDIR)/dist
endif

ifndef DIST_LIBDIR
DIST_LIBDIR = $(DIST_DIR)/lib/
endif

ifndef DIST_INCDIR
DIST_INCDIR = $(DIST_DIR)/include
endif

SYS_INC = -I/usr/local/include

CC  = $(TOOL_PREFIX)gcc
CXX = $(TOOL_PREFIX)g++
AR  = $(TOOL_PREFIX)ar
LD  = $(TOOL_PREFIX)ld
MKDIR_P = mkdir -p

TARGET_NAME = wscDrone

LOCAL_INCDIR = $(CURDIR)/src/
API_INCDIR = $(CURDIR)/inc/
OUTPUT_DIRS = $(CURDIR)/obj/ $(CURDIR)/dist/lib $(CURDIR)/dist/include/$(TARGET_NAME)
SRCDIR = $(CURDIR)/src/
OBJDIR = $(CURDIR)/obj/

SYS_INC_DIRS  = -I=/usr/local/include -I$(ARSDK3)/include
SYS_LIBS_DIRS = -L=/usr/local/lib -L$(ARSDK3)/lib

DYN_TARGET_LIST    = lib$(TARGET_NAME).so.$(LIB_VER)
STATIC_TARGET_LIST = lib$(TARGET_NAME).a

DYN_TARGET    = $(addprefix $(DIST_LIBDIR), $(DYN_TARGET_LIST))
STATIC_TARGET = $(addprefix $(DIST_LIBDIR), $(STATIC_TARGET_LIST))

DEBUGFLAGS     = -ggdb3 -O0 -D _DEBUG
RELEASEFLAGS   = -O3 -D NDEBUG
DEFAULTFLAGS   = -O3 -D NDEBUG

#COMMON_FLAGS += -pedantic -Wall -Wextra -Wno-deprecated-declarations
COMMON_FLAGS += -Wno-deprecated-declarations
# shared library flags
COMMON_FLAGS += -shared -fPIC -DRESTRICTED_ALTITUDE
CPPFLAGS     += -I$(LOCAL_INCDIR) -I$(API_INCDIR) $(SYS_INC_DIRS)
CXXFLAGS     += -std=c++14 $(COMMON_FLAGS)
LDFLAGS      += -shared -fPIC

#for FFMPEG
SYS_DYN_LIBS_LIST += avformat avcodec avutil swscale
# for Bebop2 SDK
SYS_DYN_LIBS_LIST += pthread rtsp sdp mux pomp json-c ulog futils
SYS_DYN_LIBS_LIST = arsal arcommands ardiscovery arcontroller armedia arnetwork arnetworkal arstream arstream2
SYS_STAT_LIBS_LIST += 

API_HEADER_LIST = \
                  Bebop2 \
                  DroneDiscovery \
                  DroneController \
                  CameraControl \
                  Pilot \
                  VideoDecoder \
                  VideoDriver \
                  Utils \
                  Semaphore \
                  VideoFrame

CPP_SRC_LIST = Bebop2 \
               DroneDiscovery \
               DroneController \
               CameraControl \
               Pilot \
               VideoDecoder \
               VideoDriver \
               Utils


#Prepend the path
SYS_DYN_LIBS  = -L$(DIST_LIBDIR) $(SYS_LIBS_DIRS) $(addprefix -l, $(SYS_DYN_LIBS_LIST))
SYS_STAT_LIBS = -L$(DIST_LIBDIR) $(SYS_LIBS_DIRS) $(addprefix -l, $(SYS_STAT_LIBS_LIST))
API_HEADERS = $(addsuffix .h, $(addprefix $(API_INCDIR), $(API_HEADER_LIST)))
#LOCAL_HEADERS += $(addsuffix .h, $(addprefix $(LOCAL_INCDIR), $(H_SRC_LIST)))
SOURCES_CPP = $(addsuffix .cpp, $(addprefix $(SRCDIR), $(CPP_SRC_LIST)))
SOURCES_C   = $(addsuffix .c, $(addprefix $(SRCDIR), $(C_SRC_LIST)))

OBJECTS_CPP = $(addsuffix .o, $(addprefix $(OBJDIR), $(CPP_SRC_LIST)))
OBJECTS_C   = $(addsuffix .o, $(addprefix $(OBJDIR), $(C_SRC_LIST)))


all: directories api_headers $(DYN_TARGET) $(STATIC_TARGET)

directories:
	$(MKDIR_P) $(OUTPUT_DIRS)

symlinks:
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so.$(LIB_MAJOR_VER)
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so.$(LIB_MAJOR_VER).$(LIB_MINOR_VER)

api_headers:
	cp -f $(API_INCDIR)/$(TARGET_NAME).h $(DIST_INCDIR)
	cp -f $(API_HEADERS) $(DIST_INCDIR)/$(TARGET_NAME)

$(DYN_TARGET): $(OBJECTS_C) $(OBJECTS_CPP)
	$(CXX) $(LDFLAGS) -o $(DYN_TARGET) $(OBJECTS_C) $(OBJECTS_CPP) -Wl,-Bstatic $(SYS_STAT_LIBS) -Wl,-Bdynamic $(SYS_DYN_LIBS)
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so.$(LIB_MAJOR_VER)
	ln -f -s lib$(TARGET_NAME).so.$(LIB_VER) $(DIST_LIBDIR)/lib$(TARGET_NAME).so.$(LIB_MAJOR_VER).$(LIB_MINOR_VER)

$(STATIC_TARGET): $(OBJECTS_C) $(OBJECTS_CPP) $(OBJECTS_CU)
	$(AR) $(ARFLAGS) $(STATIC_TARGET) $(OBJECTS_C) $(OBJECTS_CPP)

printvar:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEFAULTFLAGS) -c -o $@ $<

$(OBJDIR)%.o: $(SRCDIR)%.c

install: all
	$(MKDIR_P) $(DESTDIR)/$(PREFIX)/lib/ $(DESTDIR)/$(PREFIX)/include/
	cp -a $(DIST_LIBDIR)/* $(DESTDIR)/$(PREFIX)/lib/
	cp -a $(DIST_INCDIR)/* $(DESTDIR)/$(PREFIX)/include/

clean:
	-rm -f $(OBJECTS_C) $(OBJECTS_CPP)

distclean: clean
	-rm -rf $(DIST_DIR)
	-rm -rf $(OBJDIR)

	
