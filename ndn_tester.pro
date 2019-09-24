#-------------------------------------------------
#
# Project created by QtCreator 2013-10-10T18:58:10
#
#-------------------------------------------------

QT       += core gui sql widgets network

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
DESTDIR = bin

TARGET = ndn_tester
TEMPLATE = app
KDSOAPLIB=kdsoap
include(KDSoap/examples/examples.pri)

LIBS += -LKDSoap/lib


SOURCES += main.cpp\
  gen_src/MasterData.cpp \
  gen_src/Monitoring.cpp \
  gen_src/Transactions.cpp \
        mainwindow.cpp \
    databasehandler.cpp \
    ndnproductsmodel.cpp \
    dialogndnproductfilters.cpp \
    dialogndnproductdetails.cpp \
    ndnenumconverter.cpp \
    ndnproductpricemodel.cpp \
    dialogndnsettings.cpp \
    dialogndnmonitoring.cpp \
    ndnsettings.cpp \
    dialogndnmovementtester.cpp \
    dialogaddndnproducts.cpp \
    ndnwidgets.cpp
HEADERS  += mainwindow.h \
    databasehandler.h \
    gen_src/MasterData.h \
    gen_src/Monitoring.h \
    gen_src/Transactions.h \
    ndnproductsmodel.h \
    dialogndnproductfilters.h \
    dialogndnproductdetails.h \
    ndnenumconverter.h \
    ndnproductpricemodel.h \
    dialogndnsettings.h \
    dialogndnmonitoring.h \
    ndnsettings.h \
    dialogndnmovementtester.h \
    dialogaddndnproducts.h \
    ndnwidgets.h

FORMS    += mainwindow.ui \
    dialogndnproductfilters.ui \
    dialogndnproductdetails.ui \
    dialogndnsettings.ui \
    dialogndnmonitoring.ui \
    dialogndnmovementtester.ui \
    dialogaddndnproducts.ui

INCLUDEPATH += KDSoap/src/KDSoapClient
INCLUDEPATH += KDSoap/src

OTHER_FILES += \
    Changelog.txt

