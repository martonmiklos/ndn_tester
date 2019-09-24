#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QSslConfiguration>
#include <QSslKey>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbHandler = new DataBaseHandler(this);
    connect(dbHandler, SIGNAL(errorHappened()), this, SLOT(dataBaseError()));
    dbHandler->init();

    m_masterData.clientInterface()->setSslConfiguration(ndnSettings.sslConfiguration());
    m_masterData.clientInterface()->setUseWsAddressing(true);
    m_masterData.setEndPoint(ndnSettings.webserviceURLBase()+"MasterData.svc");
    connect(&m_masterData, &NSMasterData::MasterData::soapError,
            this, &MainWindow::soapError);

    connect(&m_masterData, SIGNAL(getManufacturersDone(NSMasterData::NDN_COMM__GetManufacturersResponse)),
            this, SLOT(getManufacturersDone(NSMasterData::NDN_COMM__GetManufacturersResponse)));
    connect(&m_masterData, SIGNAL(getProductsDone(NSMasterData::NDN_COMM__GetProductsResponse)),
            this, SLOT(getProductsDone(NSMasterData::NDN_COMM__GetProductsResponse)));
    connect(&m_masterData, SIGNAL(getCurrentVATRatesDone(NSMasterData::NDN_COMM__GetCurrentVATRatesResponse)),
            this, SLOT(getVATRatesDone(NSMasterData::NDN_COMM__GetCurrentVATRatesResponse)));
    connect(&m_masterData, SIGNAL(getProductGroupsDone(NSMasterData::NDN_COMM__GetProductGroupsResponse)),
            this, SLOT(getProductGroupsDone(NSMasterData::NDN_COMM__GetProductGroupsResponse)));
    connect(&m_masterData, &NSMasterData::MasterData::getProductsSinceDone,
            this, &MainWindow::getProductsSinceDone);

    productsModel = new NDNProductsModel(this);
    connect(productsModel, SIGNAL(rowCountChanged(int)), this, SLOT(productsModelRowCountChanged(int)));
    productsModel->setHeaderData(0, Qt::Horizontal, tr("NDN code"));
    productsModel->setHeaderData(1, Qt::Horizontal, tr("Name"));    
    ui->tableViewProducts->setModel(productsModel);
    productsModel->setValidFilter(true);
    productsModel->select();

    ndnProductDetails = new DialogNDNProductDetails(this);

    ndnProductFilterDialog = new DialogNDNProductFilters(productsModel, this);


    productGroupsModel = new QSqlTableModel(this);
    productGroupsModel->setTable("ndn_productgroups");
    productGroupsModel->select();
    productGroupsModel->setHeaderData(1, Qt::Horizontal, tr("Fixed price"));
    ui->tableViewProductGroups->setModel(productGroupsModel);

    manufacturersModel = new QSqlTableModel(this);
    manufacturersModel->setTable("ndn_manufacturers");
    manufacturersModel->select();
    ui->tableViewManufacturers->setModel(manufacturersModel);

    vatRatesModel = new QSqlTableModel(this);
    vatRatesModel->setTable("ndn_vatrates");
    vatRatesModel->select();
    ui->tableViewVATRates->setModel(vatRatesModel);


    ndnSettingsDialog = new DialogNDNSettings(&ndnSettings, this);
    ndnMonitoringDialog = new DialogNDNMonitoring(&ndnSettings, this);
    ndnMovementTesterDialog = new DialogNDNMovementTester(&ndnSettings, this);

    QDateTime lastProductsUpdateSaved = m_settings.value("lastProductsUpdate", QDateTime()).toDateTime();
    if (lastProductsUpdateSaved.isValid())
        ui->labelLastUpdated->setText(lastProductsUpdateSaved.toString());
    else
        ui->labelLastUpdated->setText(tr("Product data not yet retrived"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataBaseError()
{
    QMessageBox msgBox;
    msgBox.setText(dbHandler->errorString());
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWindow::on_pushButtonGetProducts_clicked()
{
    QDateTime lastUpdate = m_settings.value("lastProductsUpdate", QDateTime()).toDateTime();
    QDateTime lastProductQueryStarted = QDateTime::currentDateTime();
    if (!lastUpdate.isValid()) {
        NSMasterData::NDN_COMM__GetProducts getProducts;
        getProducts.setShopId(ndnSettings.shopID());
        m_masterData.asyncGetProducts(getProducts);
    } else {
        NSMasterData::NDN_COMM__GetProductsSince getProducts;
        getProducts.setShopId(ndnSettings.shopID());
        getProducts.setModifiedSince(lastUpdate);
        m_masterData.asyncGetProductsSince(getProducts);
    }
}

void MainWindow::getProductsDone(NSMasterData::NDN_COMM__GetProductsResponse response)
{
    m_settings.setValue("lastProductsUpdate", lastProductQueryStarted);
    QSqlDatabase::database().transaction();
    foreach (NSMasterData::NDN__Product product, response.getProductsResult().product()) {
        if (!dbHandler->productExists(product.code()))
            dbHandler->addProduct(product);
        else
            dbHandler->updateProduct(product);
    }
    QSqlDatabase::database().commit();
}

void MainWindow::getProductsSinceDone(NSMasterData::NDN_COMM__GetProductsSinceResponse response)
{
    m_settings.setValue("lastProductsUpdate", lastProductQueryStarted);
    QSqlDatabase::database().transaction();
    foreach (NSMasterData::NDN__Product product, response.getProductsSinceResult().product()) {
        if (!dbHandler->productExists(product.code()))
            dbHandler->addProduct(product);
        else
            dbHandler->updateProduct(product);
    }
    QSqlDatabase::database().commit();
}

void MainWindow::on_pushButtonGetManufacturers_clicked()
{
    NSMasterData::NDN_COMM__GetManufacturers getManufacturers;
    getManufacturers.setShopId(ndnSettings.shopID());
    m_masterData.asyncGetManufacturers(getManufacturers);
}

void MainWindow::getManufacturersDone(NSMasterData::NDN_COMM__GetManufacturersResponse response)
{
    foreach (NSMasterData::NDN__Manufacturer manufacturer, response.getManufacturersResult().manufacturer()) {
        dbHandler->addManufacturer(manufacturer);
    }
}

void MainWindow::on_pushButtonGetVATRates_clicked()
{
    NSMasterData::NDN_COMM__GetCurrentVATRates getVATRates;
    getVATRates.setShopId(ndnSettings.shopID());
    m_masterData.asyncGetCurrentVATRates(getVATRates);
}

void MainWindow::getVATRatesDone(NSMasterData::NDN_COMM__GetCurrentVATRatesResponse response)
{
    foreach (NSMasterData::NDN__VATRate vatRate, response.getCurrentVATRatesResult().vATRate()) {
        
        dbHandler->addVATRate(vatRate);
    }
}

void MainWindow::on_pushButtonGetProductGroups_clicked()
{
    NSMasterData::NDN_COMM__GetProductGroups getProductGroups;
    getProductGroups.setShopId(ndnSettings.shopID());
    m_masterData.asyncGetProductGroups(getProductGroups);
}

void MainWindow::getProductGroupsDone(NSMasterData::NDN_COMM__GetProductGroupsResponse response)
{
    foreach (NSMasterData::NDN__ProductGroup productGroup, response.getProductGroupsResult().productGroup()) {
        dbHandler->addProductGroup(productGroup);
    }
}

void MainWindow::on_pushButtonUpdateVATRates_clicked()
{

}

void MainWindow::on_pushButtonUpdateProducts_clicked()
{

}
void MainWindow::on_pushButtonProductFilterOptions_clicked()
{
    ndnProductFilterDialog->show();
}

void MainWindow::productsModelRowCountChanged(int rowCount)
{
    ui->labelRecordsCount->setText(tr("Show %1 records").arg(rowCount));
}

void MainWindow::on_tableViewProducts_doubleClicked(const QModelIndex &index)
{
    QModelIndex codeIndex = productsModel->index(index.row(), 0);
    ndnProductDetails->showItem(productsModel->data(codeIndex).toString());
}

void MainWindow::on_actionNDN_connection_triggered()
{
    ndnSettingsDialog->show();
}

void MainWindow::on_actionNDN_logs_triggered()
{
    ndnMonitoringDialog->show();
}

void MainWindow::on_actionTransaction_tester_triggered()
{
    ndnMovementTesterDialog->show();
}

void MainWindow::soapError(const QString& method, const KDSoapMessage& fault)
{
    ui->textBrowserLog->append(tr("%1: Error in method %2")
                               .arg(QDateTime::currentDateTime().toString(), method));
    ui->textBrowserLog->append(fault.faultAsString());
}
