#include "ndnsettings.h"
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>

#include <QtCrypto/QtCrypto>

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

    /*
    // read client cert first
    QFile clientCertFile("/home/mm/Projektek/qt/qtrafik/soap/certs/clientcert.pem");
    clientCertFile.open(QFile::ReadOnly);
    QSslCertificate cert(&clientCertFile);
    clientCertFile.close();
    m_SSLConfiguration.setLocalCertificate(cert);

    // read ca cert
    QFile caCertFile("/home/mm/Projektek/qt/qtrafik/soap/certs/cacert.pem");
    caCertFile.open(QFile::ReadOnly);
    QSslCertificate caCert(&caCertFile);
    caCertFile.close();
    CACerts.append(caCert);
    m_SSLConfiguration.setCaCertificates(CACerts);

    //read key file
    QFile keyFile("/home/mm/Projektek/qt/qtrafik/soap/certs/privkey.key");
    keyFile.open(QFile::ReadOnly);
    QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "1234");
    m_SSLConfiguration.setPrivateKey(key);*/

    //initialize QCA
    QCA::init();

    if (QFile::exists(m_certificatePath)) {
        if (QCA::isSupported("pkcs12")) {
            QCA::KeyBundle bundle(m_certificatePath, QCA::SecureArray(m_certificatePassword.toLocal8Bit()));

            QList<QSslCertificate> CACerts;
            foreach (QCA::Certificate cert, bundle.certificateChain()) {
                if (cert.isCA()) {
                    QSslCertificate CACertificate(cert.toPEM().toLocal8Bit(), QSsl::Pem);
                    CACerts.append(CACertificate);
                } else {
                    m_SSLConfiguration.setLocalCertificate(QSslCertificate(cert.toPEM().toLocal8Bit(), QSsl::Pem));
                }
            }

            m_SSLConfiguration.setCaCertificates(CACerts);

            QSslKey key(bundle.privateKey().toDER().toByteArray(), QSsl::Rsa, QSsl::Der);
            m_SSLConfiguration.setPrivateKey(key);
        } else {
            setError(tr("QCA PKCS12# support is missing check the QCA libraries in your plugin load paths!"));
        }
    } else {
        setError(tr("Certificate file does not exists"));
    }
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
