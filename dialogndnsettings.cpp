#include "dialogndnsettings.h"
#include "ui_dialogndnsettings.h"

#include "gen_src/MasterData.h"

#include <QFileDialog>
#include <QMessageBox>

DialogNDNSettings::DialogNDNSettings(NDNSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNDNSettings),
    m_settings(settings)
{
    ui->setupUi(this);
    restoreGUIFromSettings();
}

DialogNDNSettings::~DialogNDNSettings()
{
    delete ui;
}

void DialogNDNSettings::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    on_pushButtonRefreshCommunicationParameters_clicked();
}

void DialogNDNSettings::on_buttonBox_accepted()
{
    // FIXME sanity checking
    m_settings->setWebserviceURLBase(ui->webserviceURLLineEdit->text());
    m_settings->setShopID(ui->shopIDLineEdit->text());
    if (!QFile::exists(ui->lineEditCertificatePath->text())) {
        QMessageBox::warning(this, tr("Unable to find certificate file"), tr("The certificate path is invalid!\nPlease correct!"));
        ui->lineEditCertificatePath->setFocus();
    } else {
        m_settings->setCertificatePath(ui->lineEditCertificatePath->text());
    }
    m_settings->setCashRegisterID(ui->lineEditCashregisterID->text());
    m_settings->setCertificatePassword(ui->lineEditCertificatePassword->text());
    m_settings->save();
    m_settings->loadCertificate();
}

void DialogNDNSettings::on_buttonBox_rejected()
{
    restoreGUIFromSettings();
}

void DialogNDNSettings::on_pushButtonRefreshCommunicationParameters_clicked()
{
    ui->spinBoxBatchSize->setValue(m_settings->batchSize());
    ui->spinBoxFrequency->setValue(m_settings->frequencyInMinutes());
    ui->spinBoxOffset->setValue(m_settings->offsetInMinutes());
}

void DialogNDNSettings::on_pushButtonBrowseCertificate_clicked()
{
    QString certPath = QFileDialog::getOpenFileName(this,
                                                    tr("Please select the PKCS12# certificate file"),
                                                    m_settings->certificatePath(),
                                                    tr("PFX files (*.pfx)"));
    if (!certPath.isEmpty()) {
        ui->lineEditCertificatePath->setText(certPath);
    }
}

void DialogNDNSettings::restoreGUIFromSettings()
{
    ui->shopIDLineEdit->setText(m_settings->shopID());
    ui->webserviceURLLineEdit->setText(m_settings->webserviceURLBase());
    ui->lineEditCertificatePath->setText(m_settings->certificatePath());
    ui->lineEditCertificatePassword->setText(m_settings->certificatePassword());
    ui->lineEditCashregisterID->setText(m_settings->cashRegisterID());
}

void DialogNDNSettings::tearDownTestSignals()
{
    disconnect(&m_masterData, &NSMasterData::MasterData::soapError,
            this, &DialogNDNSettings::testSoapError);
    disconnect(&m_masterData, &NSMasterData::MasterData::getCurrentVATRatesDone,
            this, &DialogNDNSettings::testOK);
    disconnect(&m_masterData, &NSMasterData::MasterData::getCurrentVATRatesError,
            this, &DialogNDNSettings::testFailed);
}

void DialogNDNSettings::on_pushButtonTestConnection_clicked()
{
    NSMasterData::NDN_COMM__GetCurrentVATRates getVATRates;
    NDNSettings tmpSettings;
    getVATRates.setShopId(ui->shopIDLineEdit->text());

    if (!tmpSettings.loadCertificate()) {
        QMessageBox::critical(this,
                              tr("Hiba"),
                              tr("Nem sikerült a tanúsítványt betölteni!\n"
                                 "Talán a jelszó nem helyes?"));
        return;
    }
    connect(&m_masterData, &NSMasterData::MasterData::soapError,
            this, &DialogNDNSettings::testSoapError);
    connect(&m_masterData, &NSMasterData::MasterData::getCurrentVATRatesDone,
            this, &DialogNDNSettings::testOK);
    connect(&m_masterData, &NSMasterData::MasterData::getCurrentVATRatesError,
            this, &DialogNDNSettings::testFailed);
    m_masterData.clientInterface()->setSslConfiguration(tmpSettings.sslConfiguration());
    m_masterData.clientInterface()->setUseWsAddressing(true);
    m_masterData.setEndPoint(ui->webserviceURLLineEdit->text() + "MasterData.svc");
    m_masterData.asyncGetCurrentVATRates(getVATRates);
}

void DialogNDNSettings::testOK()
{
    tearDownTestSignals();
    QMessageBox::information(this,
                          tr("Siker"),
                          tr("NDN kapcsolat rendben"));
}

void DialogNDNSettings::testFailed(const KDSoapMessage& fault)
{
    tearDownTestSignals();
    QMessageBox::information(this,
                          tr("Hiba"),
                          tr("NDN kapcsolat sikertelen\n"
                             "%1").arg(fault.faultAsString()));
}

void DialogNDNSettings::testSoapError(const QString& method, const KDSoapMessage& fault)
{
    Q_UNUSED(method)
    tearDownTestSignals();
    QMessageBox::information(this,
                          tr("Hiba"),
                          tr("NDN kapcsolat sikertelen\n"
                             "%1").arg(fault.faultAsString()));
}
