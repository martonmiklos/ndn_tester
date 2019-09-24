#include "ndnsettings.h"
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>

#include <QtCrypto/QtCrypto>

bool NDNSettings::loadCertificate()
{
    QFile certFile(m_certificatePath);
    if (!certFile.open(QFile::ReadOnly)) {
        setError(tr("Unable to open the %1 certificate file").arg(m_certificatePath));
        return  false;
    }
    QSslCertificate certificate;
    QList<QSslCertificate> CaCertificates;
    QSslKey sslKey;

    bool certLoaded = QSslCertificate::importPkcs12(&certFile,
                                                    &sslKey,
                                                    &certificate,
                                                    &CaCertificates,
                                                    m_certificatePassword.toLocal8Bit());
    certFile.close();

    if (!certLoaded) {
        setError(tr("Unable to load the %1 certificate file").arg(m_certificatePath));
        return  false;
    }
    m_SSLConfiguration.setPrivateKey(sslKey);
    m_SSLConfiguration.setLocalCertificate(certificate);
    m_SSLConfiguration.setCaCertificates(CaCertificates);
    return  true;
}

NDNSettings::NDNSettings(QObject *parent) :
    QObject(parent)
{
    m_settings.beginGroup("NDN");
    m_shopID = m_settings.value("shopID", "a529a279-dc92-4943-93b8-d2912941381b").toString();
    m_webserviceURLBase = m_settings.value("webserviceURLBase", "https://adatkuldes-teszt.nemzetidohany.gov.hu:8444/nd/").toString();
    m_certificatePath = m_settings.value("certificatePath").toString();
    m_certificatePassword = m_settings.value("certificatePassword").toString();
    m_cashRegisterID = m_settings.value("cashRegisterID").toString();
    m_settings.endGroup();

    m_batchSize = 0;
    m_frequencyInMinutes = 0;
    m_offsetInMinutes = 0;

    loadCertificate();
}

void NDNSettings::setTransactionCallConfig(const NSTransactions::NDN__TransactionCallConfig & callConfig)
{
    m_batchSize = callConfig.batchSize();
    m_frequencyInMinutes = callConfig.frequency();
    m_offsetInMinutes = callConfig.offset();
}


void NDNSettings::save()
{
    m_settings.beginGroup("NDN");
    m_settings.setValue("shopID", m_shopID);
    if (!m_webserviceURLBase.endsWith('/'))
        m_webserviceURLBase.append('/');
    m_settings.setValue("webserviceURLBase", m_webserviceURLBase);
    m_settings.setValue("certificatePath", m_certificatePath);
    m_settings.setValue("certificatePassword", m_certificatePassword);
    m_settings.setValue("cashRegisterID", m_cashRegisterID);
    m_settings.endGroup();
}

void NDNSettings::setError(QString error)
{
    m_errors.append(error);
    emit errorHappened();
}
