#-------------------------------------------------
#
# Project created by QtCreator 2013-10-10T18:58:10
#
#-------------------------------------------------

QT       += core gui sql

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
DESTDIR = bin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ndn_tester
TEMPLATE = app


SOURCES += main.cpp\
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
    ndnreasoncodecombobox.cpp \
    dialogaddndnproducts.cpp
HEADERS  += mainwindow.h \
    databasehandler.h \
    ndnproductsmodel.h \
    dialogndnproductfilters.h \
    dialogndnproductdetails.h \
    ndnenumconverter.h \
    ndnproductpricemodel.h \
    dialogndnsettings.h \
    dialogndnmonitoring.h \
    ndnsettings.h \
    dialogndnmovementtester.h \
    ndnreasoncodecombobox.h \
    dialogaddndnproducts.h
FORMS    += mainwindow.ui \
    dialogndnproductfilters.ui \
    dialogndnproductdetails.ui \
    dialogndnsettings.ui \
    dialogndnmonitoring.ui \
    dialogndnmovementtester.ui \
    dialogaddndnproducts.ui

KDSOAPDIR = /opt/KDAB/KDSoap-1.0

LIBS += -L/home/mm/Projektek/qt/qca/lib/
LIBS += -lqca

INCLUDEPATH += /home/mm/Projektek/qt/qca/include

include(kdsoap.pri)
KDSOAP_DEBUG = 1

KDWSDL =    /home/mm/Projektek/qt/qtrafik/soap/ndn_tester/wsdl/Masterdata.svc \
            /home/mm/Projektek/qt/qtrafik/soap/ndn_tester/wsdl/Monitoring.svc \
            /home/mm/Projektek/qt/qtrafik/soap/ndn_tester/wsdl/Transactions.svc

