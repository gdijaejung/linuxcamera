# makefile
.SUFFIXES : .cpp .o

TARGET = mgcamera

OBJECTS = common/math/vector2.o common/math/vector3.o common/math/vector4.o common/math/matrix44.o \
common/math/quaternion.o common/math/triangle.o common/math/plane.o common/math/ray.o common/etc/stringfunc.o \
common/etc/observer.o common/etc/observer2.o common/etc/genid.o common/etc/config.o \
network/network.o network/session.o network/packetqueue.o network/tcpserver.o network/tcpclient.o \
network/udpclient.o network/udpserver.o network/netlauncher.o \
camcommon/util.o camcommon/linecontour.o  camcommon/config.o camcommon/RecognitionConfig.o \
camcommon/recognitionedgeconfig.o camcommon/detectpoint.o camcommon/rectcontour.o \
camcommon/CameraDistortion.o camcommon/camera.o \
camcommon/squarecontour.o camcommon/roi.o camcommon/screen.o camcommon/pidcontrol.o \
camcommon/skewdetect.o camcommon/streamingsender.o camcommon/streamingreceiver.o \
camcommon/searchpoint.o camcommon/thresholdfinder.o camcommon/LEDCalibrater.o \
camcommon/videowriter.o camcommon/movie.o\
stdafx.o main.o
SRCS = $(OBJECTS:.o=.cpp)


CC = g++
CXXFLAGS = -pipe -g -std=c++11 -Wall -pthread `pkg-config --cflags opencv`
OPENCVFLAGS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_features2d

PCH_FILENAME = stdafx.h
PCH_CREATE = stdafx.cpp
PCH = $(PCH_FILENAME).gch

build : $(TARGET)


$(PCH) : $(PCH_FILENAME)
	@echo "build precompiled header..."
	$(CC) $(CXXFLAGS) -x c++-header  -o $@ -c $(PCH_CREATE) $(OPENCVFLAGS)


$(TARGET) : $(PCH) $(OBJECTS)
	@echo "build execute file..."
	$(CC) $(CXXFLAGS) -o $@  $(OBJECTS) $(OPENCVFLAGS)


.cpp.o :
	$(CC) $(CXXFLAGS) -c $< -o $@ $(OPENCVFLAGS)


clean : 
	$(RM) -f $(OBJECTS) $(TARGET) $(PCH) core

