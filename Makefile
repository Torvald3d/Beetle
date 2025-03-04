#############################################################################
# Makefile for building: beetle.app/Contents/MacOS/beetle
# Generated by qmake (2.01a) (Qt 4.8.4) on: Thu May 8 12:03:33 2014
# Project:  beetle.pro
# Template: app
# Command: /Developer/Tools/Qt/qmake -spec /usr/local/Qt4.8/mkspecs/unsupported/macx-clang CONFIG+=release CONFIG+=x86_64 -o Makefile beetle.pro
#############################################################################

####### Compiler, tools and options

CC            = clang
CXX           = clang++
DEFINES       = -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -arch x86_64 -Wall -W $(DEFINES)
CXXFLAGS      = -std=c++0x -stdlib=libc++ -mmacosx-version-min=10.7 -O2 -arch x86_64 -Wall -W $(DEFINES)
INCPATH       = -I/usr/local/Qt4.8/mkspecs/unsupported/macx-clang -I. -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui -I/Library/Frameworks/QtOpenGL.framework/Versions/4/Headers -I/usr/include/QtOpenGL -I/usr/include -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/AGL.framework/Headers -I../build_files/moc -F/Library/Frameworks
LINK          = clang++
LFLAGS        = -headerpad_max_install_names -arch x86_64
LIBS          = $(SUBLIBS) -F/Library/Frameworks -L/Library/Frameworks -framework QtOpenGL -framework QtGui -framework QtCore -framework OpenGL -framework AGL 
AR            = ar cq
RANLIB        = ranlib -s
QMAKE         = /Developer/Tools/Qt/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
export MACOSX_DEPLOYMENT_TARGET = 10.4

####### Output directory

OBJECTS_DIR   = ../build_files/release/

####### Files

SOURCES       = main.cpp \
		widget.cpp \
		materials/material.cpp \
		utils/fps.cpp \
		models/beetle.cpp \
		models/stick.cpp \
		gui/gui.cpp \
		utils/hdrimage.cpp \
		models/mesh.cpp \
		materials/envmaterial.cpp ../build_files/moc/moc_widget.cpp
OBJECTS       = ../build_files/release/main.o \
		../build_files/release/widget.o \
		../build_files/release/material.o \
		../build_files/release/fps.o \
		../build_files/release/beetle.o \
		../build_files/release/stick.o \
		../build_files/release/gui.o \
		../build_files/release/hdrimage.o \
		../build_files/release/mesh.o \
		../build_files/release/envmaterial.o \
		../build_files/release/moc_widget.o
DIST          = /usr/local/Qt4.8/mkspecs/common/unix.conf \
		/usr/local/Qt4.8/mkspecs/common/mac.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base-macx.conf \
		/usr/local/Qt4.8/mkspecs/common/clang.conf \
		/usr/local/Qt4.8/mkspecs/qconfig.pri \
		/usr/local/Qt4.8/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt4.8/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.8/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.8/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.8/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/release.prf \
		/usr/local/Qt4.8/mkspecs/features/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/x86_64.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.8/mkspecs/features/warn_on.prf \
		/usr/local/Qt4.8/mkspecs/features/qt.prf \
		/usr/local/Qt4.8/mkspecs/features/unix/opengl.prf \
		/usr/local/Qt4.8/mkspecs/features/unix/thread.prf \
		/usr/local/Qt4.8/mkspecs/features/moc.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/rez.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/sdk.prf \
		/usr/local/Qt4.8/mkspecs/features/resources.prf \
		/usr/local/Qt4.8/mkspecs/features/uic.prf \
		/usr/local/Qt4.8/mkspecs/features/yacc.prf \
		/usr/local/Qt4.8/mkspecs/features/lex.prf \
		beetle.pro
QMAKE_TARGET  = beetle
DESTDIR       = 
TARGET        = beetle.app/Contents/MacOS/beetle

####### Custom Compiler Variables
QMAKE_COMP_QMAKE_OBJECTIVE_CFLAGS = -pipe \
		-O2 \
		-arch \
		x86_64 \
		-Wall \
		-W


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile beetle.app/Contents/PkgInfo beetle.app/Contents/Resources/empty.lproj beetle.app/Contents/Info.plist $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) beetle.app/Contents/MacOS/ || $(MKDIR) beetle.app/Contents/MacOS/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: beetle.pro  /usr/local/Qt4.8/mkspecs/unsupported/macx-clang/qmake.conf /usr/local/Qt4.8/mkspecs/common/unix.conf \
		/usr/local/Qt4.8/mkspecs/common/mac.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base-macx.conf \
		/usr/local/Qt4.8/mkspecs/common/clang.conf \
		/usr/local/Qt4.8/mkspecs/qconfig.pri \
		/usr/local/Qt4.8/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt4.8/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.8/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.8/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.8/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/release.prf \
		/usr/local/Qt4.8/mkspecs/features/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/x86_64.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.8/mkspecs/features/warn_on.prf \
		/usr/local/Qt4.8/mkspecs/features/qt.prf \
		/usr/local/Qt4.8/mkspecs/features/unix/opengl.prf \
		/usr/local/Qt4.8/mkspecs/features/unix/thread.prf \
		/usr/local/Qt4.8/mkspecs/features/moc.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/rez.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/sdk.prf \
		/usr/local/Qt4.8/mkspecs/features/resources.prf \
		/usr/local/Qt4.8/mkspecs/features/uic.prf \
		/usr/local/Qt4.8/mkspecs/features/yacc.prf \
		/usr/local/Qt4.8/mkspecs/features/lex.prf \
		/Library/Frameworks/QtOpenGL.framework/QtOpenGL.prl \
		/Library/Frameworks/QtGui.framework/QtGui.prl \
		/Library/Frameworks/QtCore.framework/QtCore.prl
	$(QMAKE) -spec /usr/local/Qt4.8/mkspecs/unsupported/macx-clang CONFIG+=release CONFIG+=x86_64 -o Makefile beetle.pro
/usr/local/Qt4.8/mkspecs/common/unix.conf:
/usr/local/Qt4.8/mkspecs/common/mac.conf:
/usr/local/Qt4.8/mkspecs/common/gcc-base.conf:
/usr/local/Qt4.8/mkspecs/common/gcc-base-macx.conf:
/usr/local/Qt4.8/mkspecs/common/clang.conf:
/usr/local/Qt4.8/mkspecs/qconfig.pri:
/usr/local/Qt4.8/mkspecs/modules/qt_webkit_version.pri:
/usr/local/Qt4.8/mkspecs/features/qt_functions.prf:
/usr/local/Qt4.8/mkspecs/features/qt_config.prf:
/usr/local/Qt4.8/mkspecs/features/exclusive_builds.prf:
/usr/local/Qt4.8/mkspecs/features/default_pre.prf:
/usr/local/Qt4.8/mkspecs/features/mac/default_pre.prf:
/usr/local/Qt4.8/mkspecs/features/release.prf:
/usr/local/Qt4.8/mkspecs/features/default_post.prf:
/usr/local/Qt4.8/mkspecs/features/mac/default_post.prf:
/usr/local/Qt4.8/mkspecs/features/mac/x86_64.prf:
/usr/local/Qt4.8/mkspecs/features/mac/objective_c.prf:
/usr/local/Qt4.8/mkspecs/features/warn_on.prf:
/usr/local/Qt4.8/mkspecs/features/qt.prf:
/usr/local/Qt4.8/mkspecs/features/unix/opengl.prf:
/usr/local/Qt4.8/mkspecs/features/unix/thread.prf:
/usr/local/Qt4.8/mkspecs/features/moc.prf:
/usr/local/Qt4.8/mkspecs/features/mac/rez.prf:
/usr/local/Qt4.8/mkspecs/features/mac/sdk.prf:
/usr/local/Qt4.8/mkspecs/features/resources.prf:
/usr/local/Qt4.8/mkspecs/features/uic.prf:
/usr/local/Qt4.8/mkspecs/features/yacc.prf:
/usr/local/Qt4.8/mkspecs/features/lex.prf:
/Library/Frameworks/QtOpenGL.framework/QtOpenGL.prl:
/Library/Frameworks/QtGui.framework/QtGui.prl:
/Library/Frameworks/QtCore.framework/QtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/local/Qt4.8/mkspecs/unsupported/macx-clang CONFIG+=release CONFIG+=x86_64 -o Makefile beetle.pro

beetle.app/Contents/PkgInfo: 
	@$(CHK_DIR_EXISTS) beetle.app/Contents || $(MKDIR) beetle.app/Contents 
	@$(DEL_FILE) beetle.app/Contents/PkgInfo
	@echo "APPL????" >beetle.app/Contents/PkgInfo
beetle.app/Contents/Resources/empty.lproj: 
	@$(CHK_DIR_EXISTS) beetle.app/Contents/Resources || $(MKDIR) beetle.app/Contents/Resources 
	@touch beetle.app/Contents/Resources/empty.lproj
	
beetle.app/Contents/Info.plist: 
	@$(CHK_DIR_EXISTS) beetle.app/Contents || $(MKDIR) beetle.app/Contents 
	@$(DEL_FILE) beetle.app/Contents/Info.plist
	@sed -e "s,@SHORT_VERSION@,1.0,g" -e "s,@TYPEINFO@,????,g" -e "s,@ICON@,,g" -e "s,@EXECUTABLE@,beetle,g" -e "s,@TYPEINFO@,????,g" /usr/local/Qt4.8/mkspecs/unsupported/macx-clang/Info.plist.app >beetle.app/Contents/Info.plist
dist: 
	@$(CHK_DIR_EXISTS) ../build_files/release/beetle1.0.0 || $(MKDIR) ../build_files/release/beetle1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) ../build_files/release/beetle1.0.0/ && $(COPY_FILE) --parents widget.h materials/material.h utils/fps.h models/beetle.h models/stick.h gui/gui.h utils/hdrimage.h utils/vec.h utils/vec/vec2.h utils/vec/vec3.h utils/vec/vec2i.h utils/vec/vec.h utils/color.h defines.h models/mesh.h materials/envmaterial.h ../build_files/release/beetle1.0.0/ && $(COPY_FILE) --parents main.cpp widget.cpp materials/material.cpp utils/fps.cpp models/beetle.cpp models/stick.cpp gui/gui.cpp utils/hdrimage.cpp models/mesh.cpp materials/envmaterial.cpp ../build_files/release/beetle1.0.0/ && (cd `dirname ../build_files/release/beetle1.0.0` && $(TAR) beetle1.0.0.tar beetle1.0.0 && $(COMPRESS) beetle1.0.0.tar) && $(MOVE) `dirname ../build_files/release/beetle1.0.0`/beetle1.0.0.tar.gz . && $(DEL_FILE) -r ../build_files/release/beetle1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) -r beetle.app
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: ../build_files/moc/moc_widget.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) ../build_files/moc/moc_widget.cpp
../build_files/moc/moc_widget.cpp: utils/fps.h \
		materials/envmaterial.h \
		materials/material.h \
		widget.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ widget.h -o ../build_files/moc/moc_widget.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: ../build_files/uic/qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) ../build_files/uic/qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

../build_files/release/main.o: main.cpp widget.h \
		utils/fps.h \
		materials/envmaterial.h \
		materials/material.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/main.o main.cpp

../build_files/release/widget.o: widget.cpp widget.h \
		utils/fps.h \
		materials/envmaterial.h \
		materials/material.h \
		defines.h \
		utils/hdrimage.h \
		utils/vec/vec.h \
		utils/vec/vec2.h \
		utils/vec/vec2i.h \
		utils/vec/vec3.h \
		utils/Color.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/widget.o widget.cpp

../build_files/release/material.o: materials/material.cpp materials/material.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/material.o materials/material.cpp

../build_files/release/fps.o: utils/fps.cpp utils/fps.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/fps.o utils/fps.cpp

../build_files/release/beetle.o: models/beetle.cpp models/beetle.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/beetle.o models/beetle.cpp

../build_files/release/stick.o: models/stick.cpp models/stick.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/stick.o models/stick.cpp

../build_files/release/gui.o: gui/gui.cpp gui/gui.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/gui.o gui/gui.cpp

../build_files/release/hdrimage.o: utils/hdrimage.cpp utils/hdrimage.h \
		utils/vec/vec.h \
		utils/vec/vec2.h \
		utils/vec/vec2i.h \
		utils/vec/vec3.h \
		utils/Color.h \
		defines.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/hdrimage.o utils/hdrimage.cpp

../build_files/release/mesh.o: models/mesh.cpp models/mesh.h \
		utils/vec/vec.h \
		utils/vec/vec2.h \
		utils/vec/vec2i.h \
		utils/vec/vec3.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/mesh.o models/mesh.cpp

../build_files/release/envmaterial.o: materials/envmaterial.cpp materials/envmaterial.h \
		materials/material.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/envmaterial.o materials/envmaterial.cpp

../build_files/release/moc_widget.o: ../build_files/moc/moc_widget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build_files/release/moc_widget.o ../build_files/moc/moc_widget.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

