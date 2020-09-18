TEMPLATE = app
CONFIG += console c++2a
CONFIG -= app_bundle
#CONFIG -= qt

SOURCES += \
        dt_rbtree.cpp \
        main.cpp \
        td_rbtree.cpp

HEADERS += \
    dt_rbtree.h \
    td_rbtree.h
