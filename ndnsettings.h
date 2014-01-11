#ifndef NDNSETTINGS_H
#define NDNSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QSslConfiguration>
#include <QStringList>

class NDNSettingsPrivate
{

};

class NDNSettings : public QObject
{
    Q_OBJECT
public:
    explicit NDNSettings(QObject *parent = 0);
    QString shopID() const {return m_shopID;}
    QString webserviceURLBase() const {return m_webserviceURLBase;}

    void setShopID(QString shopID) {m_shopID = shopID;}
    void setWebserviceURLBase(QString webserviceURLBase) {m_webserviceURLBase = webserviceURLBase;}

    quint16 batchSize() const {return m_batchSize;}
    void setBatchSize(quint16 batchSize) {m_batchSize = batchSize;}

    quint16 frequencyInMinutes() const {return m_frequencyInMinutes;}
    void setFrequencyInMinutes(quint16 frequencyInMinutes) {m_frequencyInMinutes = frequencyInMinutes;}

    quint16 offsetInMinutes() const {return m_offsetInMinutes;}
    void setOffsetInMinutes(quint16 offsetInMinutes) {m_offsetInMinutes = offsetInMinutes;}

    QSslConfiguration sslConfiguration() const {return m_SSLConfiguration;}

    QString certificatePath() const {return m_certificatePath;}
    void setCertificatePath(QString certificatePath) {m_certificatePath = certificatePath;}
    QString certificatePassword() const {return m_certificatePassword;}
    void setCertificatePassword(QString certificatePassword) {m_certificatePassword = certificatePassword;}

    QStringList errors() const {return m_errors;}
    void clearErrors() {m_errors.clear();}
    void save();
signals:
    void errorHappened();

private:
    void setError(QString error);

    QString m_shopID, m_webserviceURLBase;
    QSettings m_settings;

    quint16 m_batchSize;
    quint16 m_frequencyInMinutes;
    quint16 m_offsetInMinutes;

    QSslConfiguration m_SSLConfiguration;
    QString m_certificatePath;
    QString m_certificatePassword;

     QStringList m_errors;
};

#endif // NDNSETTINGS_H