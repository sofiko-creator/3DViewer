QT       += core gui
include(qtgifimage/src/gifimage/qtgifimage.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customqopenglwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    ../Core/Viewer.c

HEADERS += \
    /usr/include/GL/gl.h \
    /usr/include/GL/glcorearb.h \
    /usr/include/GL/glext.h \
    /usr/include/GL/glu.h \
    /usr/include/GL/glx.h \
    /usr/include/GL/glxext.h \
    /usr/include/GL/glxint.h \
    /usr/include/GL/glxmd.h \
    /usr/include/GL/glxproto.h \
    /usr/include/GL/glxtokens.h \
    /usr/include/GL/internal/glcore.h \
    ../Core/Viewer.h \
    customqopenglwidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Qt_3D_Viewer_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/x86_64-linux-gnu/release/ -lglut
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/x86_64-linux-gnu/debug/ -lglut
else:unix: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lglut

INCLUDEPATH += /usr/lib/x86_64-linux-gnu
DEPENDPATH += /usr/lib/x86_64-linux-gnu

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/release/libglut.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/debug/libglut.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/release/glut.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/debug/glut.lib
else:unix: PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libglut.a

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/x86_64-linux-gnu/release/ -lglut
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/x86_64-linux-gnu/debug/ -lglut
else:unix: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lglut -lGLU

INCLUDEPATH += /usr/lib/x86_64-linux-gnu
DEPENDPATH += /usr/lib/x86_64-linux-gnu
