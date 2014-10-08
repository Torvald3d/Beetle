QT += opengl

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += src/main.cpp\
	src/widget.cpp \
	src/core/material.cpp \
	src/models/beetle.cpp \
	src/models/stick.cpp \
	src/utils/hdrimage.cpp \
	src/materials/envmaterial.cpp \
	src/core/camera.cpp \
	src/core/texture.cpp \
	src/core/framebuffer.cpp \
    src/materials/quadmaterial.cpp \
    src/gui/layout.cpp \
    src/materials/blurmaterial.cpp \
    src/core/model.cpp \
    src/models/quadmodel.cpp \
    src/gui/plate.cpp \
    src/gui/guiitem.cpp \
    src/gui/button.cpp \
    src/gui/window.cpp \
    src/materials/guimaterial.cpp \
    src/materials/quadalphamaterial.cpp \
    src/gui/label.cpp \
    src/materials/colormaterial.cpp \
    src/materials/downsamplematerial.cpp \
    src/materials/fxaamaterial.cpp

HEADERS  += src/widget.h \
	src/core/material.h \
	src/utils/fps.h \
	src/models/beetle.h \
	src/models/stick.h \
	src/utils/hdrimage.h \
	src/utils/color.h \
	src/materials/envmaterial.h \
	src/debug.h \
	src/core/camera.h \
	src/core/texture.h \
	src/core/framebuffer.h \
    src/materials/quadmaterial.h \
	src/gui/layout.h \
    src/materials/blurmaterial.h \
    src/core/model.h \
    src/models/quadmodel.h \
    src/gui/plate.h \
    src/core/mesh.h \
    src/utils/geometry.h \
    src/gui/guiitem.h \
    src/gui/button.h \
    src/gui/window.h \
    src/materials/guimaterial.h \
    src/materials/quadalphamaterial.h \
    src/utils/math.h \
    src/utils/time.h \
    src/gui/label.h \
    src/materials/colormaterial.h \
    src/materials/downsamplematerial.h \
    src/materials/fxaamaterial.h
