######################################################################
# Automatically generated by qmake (3.0) Wed Jul 10 15:21:06 2013
######################################################################

TEMPLATE = app
TARGET   = TextEditor
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += debug_and_release warn_on

OBJECTS_DIR = .obj/
MOC_DIR     = .moc/

QMAKE_CXXFLAGS_DEBUG += -Og

# уменьшаем размер исполняемого файла
CONFIG(release, debug|release) {
    linux-g++ {
        QMAKE_CXXFLAGS_RELEASE += -Os -Wl,--strip-all # --gc-sections
    }
}

# Uncomment for profiler
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

RESOURCES = images.qrc

SOURCES = src/main.cpp      \
    src/mainwindow.cpp      \
    src/tabwidget.cpp       \
    src/texteditor.cpp      \
    src/findwidget.cpp      \
    src/application.cpp     \
    src/gotodialog.cpp      \
    src/findandreplace.cpp  \
    src/appsettings.cpp     \
    src/locationwidget.cpp  \
    src/overwritewidget.cpp

HEADERS = src/mainwindow.h  \
    src/tabwidget.h         \
    src/texteditor.h        \
    src/findwidget.h        \
    src/application.h       \
    src/gotodialog.h        \
    src/findandreplace.h    \
    src/appsettings.h       \
    src/locationwidget.h    \
    src/overwritewidget.h

