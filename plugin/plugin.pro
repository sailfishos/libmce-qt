TARGET = qmcedeclarative
TEMPLATE = lib
CONFIG += plugin link_pkgconfig
PKGCONFIG += mce
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-psabi

QT += qml
QT -= gui
LIBS += -L../lib -lmce-qt5
INCLUDEPATH +=  ../lib/include

SOURCES =  qmcedeclarativeplugin.cpp
OTHER_FILES += qmldir

target.path = $$[QT_INSTALL_QML]/org/sailfish/mce
qmldir.path = $$[QT_INSTALL_QML]/org/sailfish/mce
qmldir.files += qmldir

INSTALLS += target qmldir
