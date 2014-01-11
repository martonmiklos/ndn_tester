#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSslConfiguration>
#include <QSettings>
#include <QSqlQueryModel>
#include <QSqlTableModel>

#include "wsdl_MasterData.h"
#include "databasehandler.h"
#include "ndnproductsmodel.h"
#include "ndnsettings.h"

#include "dialogndnproductfilters.h"
#include "dialogndnproductdetails.h"
#include "dialogndnsettings.h"
#include "dialogndnmonitoring.h"
#include "dialogndnmovementtester.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButtonGetProducts_clicked();

    void on_pushButtonGetManufacturers_clicked();
    void getManufacturersDone(NSMasterData::TNS__GetManufacturersResponse);
    void getProductsDone(NSMasterData::TNS__GetProductsResponse);
    void getVATRatesDone(NSMasterData::TNS__GetCurrentVATRatesResponse);
    void getProductGroupsDone(NSMasterData::TNS__GetProductGroupsResponse response);
    void on_pushButtonGetVATRates_clicked();

    void on_pushButtonGetProductGroups_clicked();

    void on_pushButtonUpdateVATRates_clicked();

    void on_pushButtonUpdateProducts_clicked();


    void dataBaseError();
    void productsModelRowCountChanged(int);

    void on_pushButtonProductFilterOptions_clicked();

    void on_tableViewProducts_doubleClicked(const QModelIndex &index);

    void on_actionNDN_connection_triggered();

    void on_actionNDN_logs_triggered();

    void on_actionTransaction_tester_triggered();

private:
    NSMasterData::MasterData masterData;

    Ui::MainWindow *ui;
    QSslConfiguration mSslConfiguration;
    QSettings settings;
    DataBaseHandler *dbHandler;
    QSqlTableModel *manufacturersModel, *vatRatesModel, *productGroupsModel;
    NDNProductsModel *productsModel;
    DialogNDNProductFilters *ndnProductFilterDialog;
    DialogNDNProductDetails *ndnProductDetails;
    DialogNDNSettings *ndnSettingsDialog;
    DialogNDNMonitoring *ndnMonitoringDialog;
    DialogNDNMovementTester *ndnMovementTesterDialog;
    NDNSettings ndnSettings;
};

#endif // MAINWINDOW_H
