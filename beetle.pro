QT += core gui

QMAKE_CXXFLAGS = -std=c++0x #-mavx

win32: {
	INCLUDEPATH += src $$PWD/glew-1.10.0/include
	LIBS += -lglew32 -L$$PWD/glew-1.10.0/lib/Release/Win32
}

unix:!macx {
	LIBS += -lGLEW
}

macx {
	QMAKE_CXXFLAGS += -stdlib=libc++ -mmacosx-version-min=10.7
}

TARGET = beetle
TEMPLATE = app

include (src/src.pri)

OTHER_FILES += \
	shaders/environment.frag \
	shaders/quad.frag \
	shaders/quad.vert \
	shaders/blur.frag \
	shaders/gui.frag \
	shaders/gui.vert \
	shaders/quadAlpha.frag \
	shaders/iridescence.frag \
	shaders/color.vert \
	shaders/color.frag \
	shaders/colornm.vert \
	shaders/colornm.frag \
	shaders/environment.vert \
	shaders/downsample.frag \
	shaders/fxaa.frag
