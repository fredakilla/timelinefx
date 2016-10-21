#--------------------------------------------------------------------
# 3rdparty libraries path
#--------------------------------------------------------------------

SFML = C:\Dev\SFML-2.4.0

#--------------------------------------------------------------------
# target
#--------------------------------------------------------------------

TEMPLATE = app
CONFIG -= console
CONFIG += windows
QT -= qt
CONFIG += c++11
CONFIG -= debug_and_release
CONFIG -= debug_and_release_target

#--------------------------------------------------------------------
# output directory
#--------------------------------------------------------------------

CONFIG(debug,debug|release) {
    DESTDIR = $$PWD/binary
} else {
    DESTDIR = $$PWD/binary
}

QMAKE_CLEAN += $$DESTDIR/$$TARGET

#--------------------------------------------------------------------
# compilation flags
#--------------------------------------------------------------------

unix:!macx: QMAKE_CFLAGS_WARN_ON -= -Wall
unix:!macx: QMAKE_CXXFLAGS_WARN_ON -= -Wall
unix:!macx: QMAKE_CXXFLAGS += -Wall
unix:!macx: QMAKE_CXXFLAGS += -Wno-comment
unix:!macx: QMAKE_CXXFLAGS += -Wno-ignored-qualifiers
unix:!macx: QMAKE_CXXFLAGS += -Wno-unused-parameter
unix:!macx: QMAKE_CXXFLAGS += -std=c++11


CONFIG(debug,debug|release) {
#message( debug )
} else {
#message( release )
unix:!macx: QMAKE_CXXFLAGS += -Wno-strict-aliasing

win32:QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3
}

#--------------------------------------------------------------------
# pre-processor definitions
#--------------------------------------------------------------------

CONFIG(debug,debug|release) {
#debug
DEFINES +=  \
        #WIN32 \
        _USE_MATH_DEFINES \
} else {
# release
DEFINES +=  \
        #WIN32 \
        _USE_MATH_DEFINES \
}

#--------------------------------------------------------------------
# libraries includes
#--------------------------------------------------------------------

INCLUDEPATH += $${SFML}/include
INCLUDEPATH += ../timelinefx/source
INCLUDEPATH += ../pugixml/include

#--------------------------------------------------------------------
# check libraries dependencies
#--------------------------------------------------------------------

CONFIG(debug,debug|release) {
# debug
win32: PRE_TARGETDEPS += $${SFML}/lib/sfml-graphics-d.lib
} else {
# release
win32: PRE_TARGETDEPS += $${SFML}/lib/sfml-graphics.lib
}



#--------------------------------------------------------------------
# libraries link
#--------------------------------------------------------------------

CONFIG(debug,debug|release) {
# debug
win32:LIBS += -L$${SFML}/lib -lsfml-graphics-d
win32:LIBS += -L$${SFML}/lib -lsfml-system-d
win32:LIBS += -L$${SFML}/lib -lsfml-window-d
win32:LIBS += -L$${SFML}/lib -lsfml-main-d
} else {
# release
win32:LIBS += -L$${SFML}/lib -lsfml-graphics
win32:LIBS += -L$${SFML}/lib -lsfml-system
win32:LIBS += -L$${SFML}/lib -lsfml-window
win32:LIBS += -L$${SFML}/lib -lsfml-main
}



#LIBS += -L$${DESTDIR} -lwz4lib

win32:LIBS +=   kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib \
                ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib version.lib

unix:!macx: LIBS += -lXinerama -lXcursor -lXrandr -lXi -ldl -lXxf86vm  -lpthread -lGL -lGLU -lX11


#--------------------------------------------------------------------
# project sources
#--------------------------------------------------------------------

SOURCES += \
    source/main.cpp \
    source/SfmlEffectsLibrary.cpp \
    ../timelinefx/source/TLFXAnimImage.cpp \
    ../timelinefx/source/TLFXAttributeNode.cpp \
    ../timelinefx/source/TLFXEffect.cpp \
    ../timelinefx/source/TLFXEffectsLibrary.cpp \
    ../timelinefx/source/TLFXEmitter.cpp \
    ../timelinefx/source/TLFXEmitterArray.cpp \
    ../timelinefx/source/TLFXEntity.cpp \
    ../timelinefx/source/TLFXMatrix2.cpp \
    ../timelinefx/source/TLFXParticle.cpp \
    ../timelinefx/source/TLFXParticleManager.cpp \
    ../timelinefx/source/TLFXPugiXMLLoader.cpp \
    ../timelinefx/source/TLFXVector2.cpp \
    ../timelinefx/source/TLFXXMLLoader.cpp \
    ../pugixml/src/pugixml.cpp

HEADERS += \
    source/SfmlEffectsLibrary.h \
    ../timelinefx/source/TLFXAnimImage.h \
    ../timelinefx/source/TLFXAttributeNode.h \
    ../timelinefx/source/TLFXEffect.h \
    ../timelinefx/source/TLFXEffectsLibrary.h \
    ../timelinefx/source/TLFXEmitter.h \
    ../timelinefx/source/TLFXEmitterArray.h \
    ../timelinefx/source/TLFXEntity.h \
    ../timelinefx/source/TLFXMatrix2.h \
    ../timelinefx/source/TLFXParticle.h \
    ../timelinefx/source/TLFXParticleManager.h \
    ../timelinefx/source/TLFXPugiXMLLoader.h \
    ../timelinefx/source/TLFXVector2.h \
    ../timelinefx/source/TLFXXMLLoader.h \
    ../pugixml/include/pugiconfig.hpp \
    ../pugixml/include/pugixml.hpp
