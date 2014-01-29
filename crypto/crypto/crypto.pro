TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    client.c \
    main.c \
    server.c \
    common.c \
    list.c

HEADERS += \
    client.h \
    common.h \
    server.h \
    list.h


unix|win32: LIBS += -lncurses

unix|win32: LIBS += -lpthread
