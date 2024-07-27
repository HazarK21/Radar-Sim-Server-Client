QT += core gui network widgets
QT += core5compat

TEMPLATE = app
TARGET = server

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    radarbeamcontroller.cpp \
    radarfalseplotspawner.cpp \
    serverStuff.cpp

HEADERS += \
    mainwindow.h \
    radarbeamcontroller.h \
    radarfalseplotspawner.h \
    serverStuff.h

FORMS += \
    mainwindow.ui
