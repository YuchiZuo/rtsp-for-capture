﻿DEUBG = -D_DEBUG

TARGET1 = rtsp_server
TARGET2 = rtsp_pusher
TARGET3 = rtsp_h264_file
TARGET4 = rtsp_jpeg_file

OBJS_PATH = objs

CROSS_COMPILE =
CXX   = $(CROSS_COMPILE)g++
CC    = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

INC  = -I$(shell pwd)/src/ -I$(shell pwd)/src/net -I$(shell pwd)/src/xop -I$(shell pwd)/src/3rdpart
LIB  =

OPENCV_INCLUDE = -I/usr/local/include/opencv4/opencv2 -I/usr/local/include/opencv4
OPENCV_LIBS = -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio

LD_FLAGS  = -lrt -pthread -lpthread -ldl -lm $(DEBUG) $(OPENCV_LIBS)
CXX_FLAGS = -std=c++11 $(OPENCV_INCLUDE)

O_FLAG = -O2

SRC1  = $(notdir $(wildcard ./src/net/*.cpp))
OBJS1 = $(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRC1))

SRC2  = $(notdir $(wildcard ./src/xop/*.cpp))
OBJS2 = $(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRC2))

SRC3  = $(notdir $(wildcard ./example/rtsp_server.cpp))
OBJS3 = $(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRC3))

SRC4  = $(notdir $(wildcard ./example/rtsp_pusher.cpp))
OBJS4 = $(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRC4))

SRC5  = $(notdir $(wildcard ./example/rtsp_h264_file.cpp))
OBJS5 = $(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRC5))

SRC6  = $(notdir $(wildcard ./example/rtsp_jpeg_file.cpp))
OBJS6 = $(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRC6))

all: BUILD_DIR $(TARGET4)

BUILD_DIR:
	@-mkdir -p $(OBJS_PATH)

$(TARGET1) : $(OBJS1) $(OBJS2) $(OBJS3)
	$(CXX) $^ -o $@ $(CFLAGS) $(LD_FLAGS) $(CXX_FLAGS)

$(TARGET2) : $(OBJS1) $(OBJS2) $(OBJS4)
	$(CXX) $^ -o $@ $(CFLAGS) $(LD_FLAGS) $(CXX_FLAGS)

$(TARGET3) : $(OBJS1) $(OBJS2) $(OBJS5)
	$(CXX) $^ -o $@ $(CFLAGS) $(LD_FLAGS) $(CXX_FLAGS)

$(TARGET4) : $(OBJS1) $(OBJS2) $(OBJS6) 
	$(CXX) $^ -o $@ $(CFLAGS) $(LD_FLAGS) $(CXX_FLAGS) $(OPENCV_LIBS)

$(OBJS_PATH)/%.o : ./example/%.cpp
	$(CXX) -c  $< -o  $@  $(CXX_FLAGS) $(INC)
$(OBJS_PATH)/%.o : ./src/net/%.cpp
	$(CXX) -c  $< -o  $@  $(CXX_FLAGS) $(INC)
$(OBJS_PATH)/%.o : ./src/xop/%.cpp
	$(CXX) -c  $< -o  $@  $(CXX_FLAGS) $(INC)



clean:
	-rm -rf $(OBJS_PATH) $(TARGET4)
