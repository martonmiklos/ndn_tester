#ifndef DIALOGNDNMONITORING_H
#define DIALOGNDNMONITORING_H

#include <QDialog>
#include <QAbstractTableModel>

#include "wsdl_Monitoring.h"
#include "ndnsettings.h"

namespace Ui {
class DialogNDNMonitoring;
}

class OpeningStockModel : public QAbstractTableModel {
    Q_OBJECT

public:

    struct OpeningStockItem {
        int lineNo;
        QString localProductCode, NDNProductCode, name;
        double quantity;
    };

    OpeningStockModel(QObject *parent = NULL);

    void clear();
    void addItem(OpeningStockItem item);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<OpeningStockItem> m_items;
};

class DialogNDNMonitoring : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNDNMonitoring(NDNSettings *settings, QWidget *parent = 0);
    ~DialogNDNMonitoring();

private slots:
    void on_pushButtonGetOpeningStock_clicked();
    void getOpeningStockDone(NSMonitoring::TNS__GetOpeningStockResponse response);
    void getOpeningStockError(KDSoapMessage message);

    void getDocumentDone(NSMonitoring::TNS__GetDocumentResponse response);
    void getDocumentError(KDSoapMessage message);

    void soapError(QString,KDSoapMessage);
    void on_pushButtonGetDocument_clicked();

private:
    Ui::DialogNDNMonitoring *ui;
    NSMonitoring::Monitoring monitoring;
    NDNSettings *m_settings;
    OpeningStockModel *m_openingStockModel;
};

#endif // DIALOGNDNMONITORING_H
