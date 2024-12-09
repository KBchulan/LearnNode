QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
QMAKE_CXXFLAGS += -std=c++20
QMAKE_CXXFLAGS += -Wno-deprecated-enum-enum-conversion

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    candyitem.cpp

HEADERS += \
    mainwindow.h \
    candyitem.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 