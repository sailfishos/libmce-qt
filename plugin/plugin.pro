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

target.path = $$[QT_INSTALL_QML]/Nemo/Mce
qmldir.path = $$[QT_INSTALL_QML]/Nemo/Mce
qmldir.files += qmldir plugins.qmltypes

INSTALLS += target qmldir

qmltypes.commands = qmlplugindump -nonrelocatable Nemo.Mce 1.0 > $$PWD/plugins.qmltypes
QMAKE_EXTRA_TARGETS += qmltypes
