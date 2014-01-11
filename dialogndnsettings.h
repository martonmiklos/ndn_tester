#ifndef DIALOGNDNSETTINGS_H
#define DIALOGNDNSETTINGS_H

#include <QDialog>
#include "ndnsettings.h"

namespace Ui {
class DialogNDNSettings;
}

class DialogNDNSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNDNSettings(NDNSettings *settings, QWidget *parent = 0);
    ~DialogNDNSettings();
    void showEvent(QShowEvent *event);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButtonRefreshCommunicationParameters_clicked();

    void on_pushButtonBrowseCertificate_clicked();

private:
    Ui::DialogNDNSettings *ui;
    NDNSettings *m_settings;
};

#endif // DIALOGNDNSETTINGS_H
