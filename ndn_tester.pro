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


SOURCES += main.cpp\
  KDSoap/src/KDSoapClient/KDDateTime.cpp \
  KDSoap/src/KDSoapClient/KDQName.cpp \
  KDSoap/src/KDSoapClient/KDSoapAuthentication.cpp \
  KDSoap/src/KDSoapClient/KDSoapClientInterface.cpp \
  KDSoap/src/KDSoapClient/KDSoapClientThread.cpp \
  KDSoap/src/KDSoapClient/KDSoapEndpointReference.cpp \
  KDSoap/src/KDSoapClient/KDSoapFaultException.cpp \
  KDSoap/src/KDSoapClient/KDSoapJob.cpp \
  KDSoap/src/KDSoapClient/KDSoapMessage.cpp \
  KDSoap/src/KDSoapClient/KDSoapMessageAddressingProperties.cpp \
  KDSoap/src/KDSoapClient/KDSoapMessageReader.cpp \
  KDSoap/src/KDSoapClient/KDSoapMessageWriter.cpp \
  KDSoap/src/KDSoapClient/KDSoapNamespaceManager.cpp \
  KDSoap/src/KDSoapClient/KDSoapNamespacePrefixes.cpp \
  KDSoap/src/KDSoapClient/KDSoapPendingCall.cpp \
  KDSoap/src/KDSoapClient/KDSoapPendingCallWatcher.cpp \
  KDSoap/src/KDSoapClient/KDSoapReplySslHandler.cpp \
  KDSoap/src/KDSoapClient/KDSoapSslHandler.cpp \
  KDSoap/src/KDSoapClient/KDSoapValue.cpp \
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
    KDSoap/src/KDSoapClient/KDDateTime.h \
    KDSoap/src/KDSoapClient/KDQName.h \
    KDSoap/src/KDSoapClient/KDSoap.h \
    KDSoap/src/KDSoapClient/KDSoapAuthentication.h \
    KDSoap/src/KDSoapClient/KDSoapClientInterface.h \
    KDSoap/src/KDSoapClient/KDSoapClientInterface_p.h \
    KDSoap/src/KDSoapClient/KDSoapClientThread_p.h \
    KDSoap/src/KDSoapClient/KDSoapEndpointReference.h \
    KDSoap/src/KDSoapClient/KDSoapFaultException.h \
    KDSoap/src/KDSoapClient/KDSoapGlobal.h \
    KDSoap/src/KDSoapClient/KDSoapJob.h \
    KDSoap/src/KDSoapClient/KDSoapMessage.h \
    KDSoap/src/KDSoapClient/KDSoapMessageAddressingProperties.h \
    KDSoap/src/KDSoapClient/KDSoapMessageReader_p.h \
    KDSoap/src/KDSoapClient/KDSoapMessageWriter_p.h \
    KDSoap/src/KDSoapClient/KDSoapNamespaceManager.h \
    KDSoap/src/KDSoapClient/KDSoapNamespacePrefixes_p.h \
    KDSoap/src/KDSoapClient/KDSoapPendingCall.h \
    KDSoap/src/KDSoapClient/KDSoapPendingCallWatcher.h \
    KDSoap/src/KDSoapClient/KDSoapPendingCallWatcher_p.h \
    KDSoap/src/KDSoapClient/KDSoapPendingCall_p.h \
    KDSoap/src/KDSoapClient/KDSoapReplySslHandler_p.h \
    KDSoap/src/KDSoapClient/KDSoapSslHandler.h \
    KDSoap/src/KDSoapClient/KDSoapValue.h \
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
    dialogaddndnproducts.h \
    ndnwidgets.h
FORMS    += mainwindow.ui \
    dialogndnproductfilters.ui \
    dialogndnproductdetails.ui \
    dialogndnsettings.ui \
    dialogndnmonitoring.ui \
    dialogndnmovementtester.ui \
    dialogaddndnproducts.ui

LIBS += -lqca

INCLUDEPATH += KDSoap/src/KDSoapClient
INCLUDEPATH += KDSoap/src

OTHER_FILES += \
    Changelog.txt

