#include "dialogndnsettings.h"
#include "ui_dialogndnsettings.h"
#include <QFileDialog>
#include <QMessageBox>

DialogNDNSettings::DialogNDNSettings(NDNSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNDNSettings),
    m_settings(settings)
{
    ui->setupUi(this);
    on_buttonBox_rejected(); // this will fill the inputs from the settings
}

DialogNDNSettings::~DialogNDNSettings()
{
    delete ui;
}

void DialogNDNSettings::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    on_pushButtonRefreshCommunicationParameters_clicked();
}

void DialogNDNSettings::on_buttonBox_accepted()
{
    m_settings->setWebserviceURLBase(ui->webserviceURLLineEdit->text());
    m_settings->setShopID(ui->shopIDLineEdit->text());
    if (!QFile::exists(ui->lineEditCertificatePath->text())) {
        QMessageBox::warning(this, tr("Unable to find certificate file"), tr("The certificate path is invalid!\nPlease correct!"));
        ui->lineEditCertificatePath->setFocus();
    } else {
        m_settings->setCertificatePath(ui->lineEditCertificatePath->text());
    }

    m_settings->setCertificatePassword(ui->lineEditCertificatePassword->text());
    m_settings->save();
}

void DialogNDNSettings::on_buttonBox_rejected()
{
    ui->shopIDLineEdit->setText(m_settings->shopID());
    ui->webserviceURLLineEdit->setText(m_settings->webserviceURLBase());
    ui->lineEditCertificatePath->setText(m_settings->certificatePath());
    ui->lineEditCertificatePassword->setText(m_settings->certificatePassword());
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
