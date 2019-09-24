#ifndef DIALOGNDNSETTINGS_H
#define DIALOGNDNSETTINGS_H

#include <QDialog>
#include "ndnsettings.h"

#include "gen_src/MasterData.h"

namespace Ui {
class DialogNDNSettings;
}

class DialogNDNSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNDNSettings(NDNSettings *settings, QWidget *parent = nullptr);
    ~DialogNDNSettings();
    void showEvent(QShowEvent *event);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonRefreshCommunicationParameters_clicked();
    void on_pushButtonBrowseCertificate_clicked();
    void on_pushButtonTestConnection_clicked();
    void testOK();
    void testFailed(const KDSoapMessage &fault);
    void testSoapError(const QString& method, const KDSoapMessage& fault);
    void tearDownTestSignals();

private:
    Ui::DialogNDNSettings *ui;
    NDNSettings *m_settings;
    NSMasterData::MasterData m_masterData;

    void restoreGUIFromSettings();
};

#endif // DIALOGNDNSETTINGS_H
