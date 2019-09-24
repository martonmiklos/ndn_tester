#ifndef DIALOGNDNMOVEMENTTESTER_H
#define DIALOGNDNMOVEMENTTESTER_H

#include <QDialog>
#include "ndnsettings.h"
#include "dialogaddndnproducts.h"
#include "gen_src/Transactions.h"

namespace Ui {
class DialogNDNMovementTester;
}

class MovementReportItemsModel :public QAbstractTableModel {
    Q_OBJECT
    struct MovementReportItem {
        QString NDNCode;
        double amount, netPrice, grossPrice;
    };

public:
    explicit MovementReportItemsModel(QObject *parent = nullptr);
    void addItem(QString ndnCode, double amount, double nettoPrice, double grossPrice);

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<MovementReportItem> m_items;
};

class DialogNDNMovementTester : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNDNMovementTester(NDNSettings *settings, QWidget *parent = nullptr);
    ~DialogNDNMovementTester();

private slots:
    void on_pushButtonOpeningStock_clicked();


    void on_pushButtonRemoveItem_clicked();

    void on_pushButtonAddItem_clicked();
    void createOpeningStockDone(NSTransactions::NDN_COMM__CreateOpeningStockResponse);
    void createOpeningStockError(KDSoapMessage msg);

    void on_pushButtonSubmitSalesDocument_clicked();
    void createSalesTransactionDone(NSTransactions::NDN_COMM__CreateSalesTransactionResponse);
    void createSalesTransactionError(KDSoapMessage msg);

    void on_pushButtonSubmitStockDocument_clicked();
    void createStockTransactionDone(NSTransactions::NDN_COMM__CreateStockTransactionResponse);
    void createStockTransactionError(KDSoapMessage msg);

    void itemAdded(QString ndnCode, double amount, double nettoPrice, double grossPrice);

    bool validateStockUserInput();
    bool validateSalesUserInput();
    bool validateSubmitOpeningStockUserInput();

    void on_stockDocumentMovementTypeComboBox_currentIndexChanged(int);

private:
    Ui::DialogNDNMovementTester *ui;
    NSTransactions::Transactions transactions;
    NDNSettings *m_settings;
    DialogAddNDNProducts *productAddDialog;
    MovementReportItemsModel *m_model;

    NSTransactions::NDN__Partner partnerDataFromUI();
};

#endif // DIALOGNDNMOVEMENTTESTER_H
