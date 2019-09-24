#ifndef NDNSETTINGS_H
#define NDNSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QSslConfiguration>
#include <QStringList>

#include "gen_src/Transactions.h"

class NDNSettingsPrivate
{

};

class NDNSettings : public QObject
{
    Q_OBJECT
public:
    explicit NDNSettings(QObject *parent = nullptr);
    QString shopID() const {return m_shopID;}
    QString webserviceURLBase() const {return m_webserviceURLBase;}

    void setShopID(QString shopID) {m_shopID = shopID;}
    void setWebserviceURLBase(QString webserviceURLBase) {m_webserviceURLBase = webserviceURLBase;}

    // NDN communication interval settings
    void setTransactionCallConfig(const NSTransactions::NDN__TransactionCallConfig & callConfig);
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

    QString cashRegisterID() const {return m_cashRegisterID;}
    void setCashRegisterID(QString cashRegisterID) {m_cashRegisterID = cashRegisterID;}

    QStringList errors() const {return m_errors;}
    void clearErrors() {m_errors.clear();}
    void save();
    bool loadCertificate();

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
    QString m_cashRegisterID;
};

#endif // NDNSETTINGS_H
