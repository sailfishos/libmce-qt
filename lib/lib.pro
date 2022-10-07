TARGET = mce-qt5
CONFIG += create_pc create_prl no_install_prl link_pkgconfig
PKGCONFIG += mce

QT += dbus
QT -= gui
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-psabi
INCLUDEPATH += include

include(version.pri)

TEMPLATE = lib
DEFINES += QMCE_LIBRARY

isEmpty(PREFIX) {
  PREFIX=/usr
}

XML_FILES += \
    dbus/com.nokia.mce.request.xml \
    dbus/com.nokia.mce.signal.xml

OTHER_FILES += $$XML_FILES

SOURCES += \
    src/qmcebatterylevel.cpp \
    src/qmcebatterystatus.cpp \
    src/qmcebatterystate.cpp \
    src/qmcecablestate.cpp \
    src/qmcecallstate.cpp \
    src/qmcechargertype.cpp \
    src/qmcechargerstate.cpp \
    src/qmcechargingstate.cpp \
    src/qmcedisplay.cpp \
    src/qmcenameowner.cpp \
    src/qmcepowersavemode.cpp \
    src/qmceproxy.cpp \
    src/qmcetklock.cpp

PUBLIC_HEADERS += \
    include/qmcebatterylevel.h \
    include/qmcebatterystatus.h \
    include/qmcebatterystate.h \
    include/qmcecablestate.h \
    include/qmcecallstate.h \
    include/qmcechargertype.h \
    include/qmcechargerstate.h \
    include/qmcechargingstate.h \
    include/qmcedisplay.h \
    include/qmcenameowner.h \
    include/qmcepowersavemode.h \
    include/qmcetypes.h \
    include/qmcetklock.h

HEADERS += \
    $$PUBLIC_HEADERS \
    src/qmceproxy.h

DBUS_INTERFACES += com_nokia_mce_request
com_nokia_mce_request.files = dbus/com.nokia.mce.request.xml
com_nokia_mce_request.header_flags = -N -c QMceRequestProxy
com_nokia_mce_request.source_flags = -N -c QMceRequestProxy

DBUS_INTERFACES += com_nokia_mce_signal
com_nokia_mce_signal.files = dbus/com.nokia.mce.signal.xml
com_nokia_mce_signal.header_flags = -N -c QMceSignalProxy
com_nokia_mce_signal.source_flags = -N -c QMceSignalProxy

target.path = $$[QT_INSTALL_LIBS]

headers.files = $$PUBLIC_HEADERS
headers.path = $$INSTALL_ROOT$$PREFIX/include/mce-qt5

pkgconfig.files = mce-qt5.pc
pkgconfig.path = $$[QT_INSTALL_LIBS]/pkgconfig

QMAKE_PKGCONFIG_NAME = $$TARGET
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESCRIPTION = Qt bindings for mce
QMAKE_PKGCONFIG_PREFIX = $$PREFIX

INSTALLS += target headers pkgconfig
