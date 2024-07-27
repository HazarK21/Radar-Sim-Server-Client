QT += core gui network widgets
QT += core5compat
QT += charts

TEMPLATE = app
TARGET = client

MOC_DIR     += generated/mocs
UI_DIR      += generated/uis
RCC_DIR     += generated/rccs
OBJECTS_DIR += generated/objs

SOURCES += main.cpp\
    chartview.cpp \
        mainwindow.cpp \
    clientStuff.cpp \
    plottermodel.cpp \
    radarppiscope.cpp

HEADERS  += mainwindow.h \
    chartview.h \
    clientStuff.h \
    plottermodel.h \
    radarppiscope.h

FORMS    += mainwindow.ui
