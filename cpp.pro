TEMPLATE = app
CONFIG += console c++2a
CONFIG -= app_bundle
#CONFIG -= qt

SOURCES += \
        bu_rbtree.cpp \
        main.cpp \
        td_rbtree.cpp

HEADERS += \
    bu_rbtree.h \
    td_rbtree.h
