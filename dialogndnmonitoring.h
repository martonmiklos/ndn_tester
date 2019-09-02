#ifndef DIALOGNDNMONITORING_H
#define DIALOGNDNMONITORING_H

#include <QDialog>
#include <QAbstractTableModel>

#include "gen_src/Monitoring.h"
#include "ndnsettings.h"

namespace Ui {
class DialogNDNMonitoring;
}

class DialogNDNMonitoring : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNDNMonitoring(NDNSettings *settings, QWidget *parent = 0);
    ~DialogNDNMonitoring();

private slots:
    void on_pushButtonGetOpeningStock_clicked();
    void getOpeningStockDone(NSMonitoring::NDN_COMM__GetOpeningStockResponse response);
    void getOpeningStockError(KDSoapMessage message);

    void getDocumentDone(NSMonitoring::NDN_COMM__GetDocumentResponse response);
    void getDocumentError(KDSoapMessage message);

    void soapError(QString,KDSoapMessage);
    void on_pushButtonGetDocument_clicked();

private:
    Ui::DialogNDNMonitoring *ui;
    NSMonitoring::Monitoring monitoring;
    NDNSettings *m_settings;

    void setTableContentsToTreeWidget(NSMonitoring::NDN__ArrayOfTableContent table);
};

#endif // DIALOGNDNMONITORING_H
