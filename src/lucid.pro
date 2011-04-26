#-------------------------------------------------
#
# Project created by QtCreator 2011-04-21T14:21:47
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = lucid
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    feature.cpp \
    contractor.cpp \
    grammar.cpp \
    rule.cpp \
	parser.cpp \  
    factory.cpp \
    referee.cpp \
    condition.cpp \
    grammarnode.cpp \
    symbol.cpp \
    operation.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    common.h \
    feature.h \
    contractor.h \
    grammar.h \
    rule.h \
	parser.h \  
    factory.h \
    referee.h \
    condition.h \
    grammarnode.h \
    symbol.h \
    operation.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    first.sym \
    first.rul \
    first.gmr
