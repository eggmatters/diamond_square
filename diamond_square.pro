# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.
QT += core gui opengl

TARGET = diamond_square

TEMPLATE = app

SOURCES += main.cpp\
           glwidget.cpp \
    terraingen.cpp \
    plane.cpp \
    world.cpp \
    sector.cpp

HEADERS += glwidget.h \
    terraingen.h \
    plane.h \
    world.h \
    sector.h \
    terrainConfig.h

OTHER_FILES += \
    vertexShader.vsh \
    fragmentShader.fsh \
    cities_log.txt \
    textures/grassa512.bmp \
    textures/grassb512.bmp \
    textures/rock512.bmp \
    textures/sand512.bmp

RESOURCES += \
    resources.qrc

