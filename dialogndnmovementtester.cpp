#include "dialogndnmovementtester.h"
#include "ui_dialogndnmovementtester.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>

// FIXME remove it
QString getLastExecutedQuery(const QSqlQuery& query)
{
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    while (it.hasNext())
    {
        it.next();
        str.replace(it.key(), "\""+it.value().toString()+"\"");
    }
    return str;
}

MovementReportItemsModel::MovementReportItemsModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

void MovementReportItemsModel::addItem(QString ndnCode, double amount, double nettoPrice, double grossPrice)
{
    MovementReportItem item;
    item.NDNCode = ndnCode;
    item.netPrice = nettoPrice;
    item.grossPrice = grossPrice;
    item.amount = amount;
    beginResetModel();
    m_items.append(item);
    endResetModel();
    //emit dataChanged(index(0, 0), index(rowCount(QModelIndex())-1, columnCount(QModelIndex())-1));
}

int MovementReportItemsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4;
}

int MovementReportItemsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_items.count();
}

QVariant MovementReportItemsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case 0:
            return m_items.at(index.row()).NDNCode;
        case 1:
            return m_items.at(index.row()).amount;
        case 2:
            return m_items.at(index.row()).netPrice;
        case 3:
            return m_items.at(index.row()).grossPrice;
        }
    }
    return QVariant();
}

Qt::ItemFlags MovementReportItemsModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled;
}

QVariant MovementReportItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("NDN code");
        case 1:
            return tr("Amount");
        case 2:
            return tr("Netto price");
        case 3:
            return tr("Gross price");
        }
    }
    return QVariant();
}

DialogNDNMovementTester::DialogNDNMovementTester(NDNSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNDNMovementTester),
    m_settings(settings)
{
    ui->setupUi(this);

    ui->stockDocumentIssuedOnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->stockDocumentDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    ui->salesDocumentDueDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->salesDocumentDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->salesDocumentIssuedOnDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    ui->openingStockDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    transactions.clientInterface()->setSslConfiguration(settings->sslConfiguration());
    transactions.clientInterface()->setUsingAddressing(true);
    transactions.setEndPoint(settings->webserviceURLBase()+"Transactions.svc");

    connect(&transactions, SIGNAL(createOpeningStockDone(NSTransactions::TNS__CreateOpeningStockResponse)),
            this , SLOT(createOpeningStockDone(NSTransactions::TNS__CreateOpeningStockResponse)));
    connect(&transactions, SIGNAL(createOpeningStockError(KDSoapMessage)),
            this, SLOT(createOpeningStockError(KDSoapMessage)));

    connect(&transactions, SIGNAL(createSalesTransactionDone(NSTransactions::TNS__CreateSalesTransactionResponse)),
            this , SLOT(createSalesTransactionDone(NSTransactions::TNS__CreateSalesTransactionResponse)));
    connect(&transactions, SIGNAL(createSalesTransactionError(KDSoapMessage)),
            this, SLOT(createSalesTransactionError(KDSoapMessage)));

    connect(&transactions, SIGNAL(createStockTransactionDone(NSTransactions::TNS__CreateStockTransactionResponse)),
            this , SLOT(createStockTransactionDone(NSTransactions::TNS__CreateStockTransactionResponse)));
    connect(&transactions, SIGNAL(createStockTransactionError(KDSoapMessage)),
            this, SLOT(createStockTransactionError(KDSoapMessage)));

    productAddDialog = new DialogAddNDNProducts(this);
    connect(productAddDialog, SIGNAL(itemSelected(QString,double,double,double)), this, SLOT(itemAdded(QString,double, double,double)));

    m_model = new MovementReportItemsModel(this);
    ui->tableViewItems->setModel(m_model);
}

DialogNDNMovementTester::~DialogNDNMovementTester()
{
    delete ui;
}

void DialogNDNMovementTester::on_pushButtonRemoveItem_clicked()
{
    // TODO
}

void DialogNDNMovementTester::on_pushButtonAddItem_clicked()
{
    productAddDialog->show();
}

void DialogNDNMovementTester::itemAdded(QString ndnCode, double amount, double nettoPrice, double grossPrice)
{
    m_model->addItem(ndnCode, amount, nettoPrice, grossPrice);
    ui->tableViewItems->reset();
}

NSTransactions::__Partner DialogNDNMovementTester::partnerDataFromUI()
{
    NSTransactions::__Partner partner;
    partner.setLocalPartnerCode(ui->localPartnerCodeLineEdit->text());
    partner.setName(ui->partnerNameLineEdit->text());
    partner.setCity(ui->partnerCityLineEdit->text());
    partner.setPostCode(ui->partnerPostcodeLineEdit->text());
    partner.setStreet(ui->partnerStreetLineEdit->text());
    return partner;
}

bool DialogNDNMovementTester::validateStockUserInput()
{
    return true;
}

bool DialogNDNMovementTester::validateSalesUserInput()
{
    return true;
}

bool DialogNDNMovementTester::validateSubmitOpeningStockUserInput()
{
    return true;
}

void DialogNDNMovementTester::on_stockDocumentMovementTypeComboBox_currentIndexChanged(int )
{
    ui->stockDocumentReasoncodeComboBox->setEnabled(ui->stockDocumentMovementTypeComboBox->reasonNeeded());
    if (!ui->stockDocumentMovementTypeComboBox->reasonNeeded())
        ui->stockDocumentReasoncodeComboBox->setCurrentIndex(0);
}

void DialogNDNMovementTester::on_pushButtonOpeningStock_clicked()
{
    NSTransactions::TNS__CreateOpeningStock createOpeningStockRequest;
    createOpeningStockRequest.setShopId(m_settings->shopID());

    NSTransactions::__OpeningStock openingStockDocument;

    // zero  out the msec part of the timestamp
    QDateTime created = ui->openingStockDateDateTimeEdit->dateTime();
    created = created.addMSecs(-created.time().msec());
    openingStockDocument.setCreatedAt(created);
    qWarning() << openingStockDocument.createdAt().toDateString();

    openingStockDocument.setShopId(m_settings->shopID());

    QList<NSTransactions::__OpeningStockLine> openingStockLines;

    for (int row = 0; row<m_model->rowCount(QModelIndex()); row++) {
        NSTransactions::__OpeningStockLine openingStockLine;
        openingStockLine.setLineNo(row);
        QString NDNCode = m_model->data(m_model->index(row, 0), Qt::DisplayRole).toString();
        openingStockLine.setProductCode(NDNCode); // NDNcode

        QSqlQuery query;
        query.prepare("SELECT p.rowid AS LocalProductCode, p.Name AS ProductName, p.ProductGroup "
                      "FROM ndn_products p "
                      "WHERE p.Code = :Code");
        query.bindValue(":Code", NDNCode);
        if (query.exec() && query.next())  {
            openingStockLine.setLocalProductCode(query.value("LocalProductCode").toString());
            openingStockLine.setProductGroup(query.value("ProductGroup").toInt());
            openingStockLine.setProductName(query.value("ProductName").toString());
            openingStockLine.setQuantity(m_model->data(m_model->index(row, 1), Qt::DisplayRole).toDouble());
        } else {
            qWarning() << query.lastError();
            return;
        }
        openingStockLines.append(openingStockLine);
    }
    NSTransactions::__ArrayOfOpeningStockLine lines;
    lines.setOpeningStockLine(openingStockLines);
    openingStockDocument.setLines(lines);

    createOpeningStockRequest.setDocument(openingStockDocument);
    transactions.asyncCreateOpeningStock(createOpeningStockRequest);
}

void DialogNDNMovementTester::createOpeningStockDone(NSTransactions::TNS__CreateOpeningStockResponse response)
{
    m_settings->setTransactionCallConfig(response.createOpeningStockResult().currentConfig());
    foreach (NSTransactions::__ResponseCode responseCode, response.createOpeningStockResult().responseCodes().responseCode()) {
        qWarning() << responseCode.code() << responseCode.key() << responseCode.message();
    }
}

void DialogNDNMovementTester::createOpeningStockError(KDSoapMessage msg)
{
    qWarning() << "createOpeningStockError" << msg.toXml();
}

void DialogNDNMovementTester::on_pushButtonSubmitSalesDocument_clicked()
{
    float VATSum = 0.0f, grossSum = 0.0f;
    QList<NSTransactions::__SalesDocumentLine> salesDocumentLineItems;
    QList<NSTransactions::__DocumentVatBreakDown> VATBreakDownItems;

    for (int row = 0; row<m_model->rowCount(QModelIndex()); row++) { // loop through the adde products and create the SOAP message
        NSTransactions::__SalesDocumentLine salesDocumentLine;
        QString NDNCode = m_model->data(m_model->index(row, 0), Qt::DisplayRole).toString();
        double quantity = m_model->data(m_model->index(row, 1), Qt::DisplayRole).toDouble();
        double netListPrice = m_model->data(m_model->index(row, 2), Qt::DisplayRole).toDouble();
        double grossPrice = m_model->data(m_model->index(row, 3), Qt::DisplayRole).toDouble();

        QSqlQuery getProductInfoQuery;
        getProductInfoQuery.prepare("SELECT p.rowid AS LocalProductCode, p.Name AS ProductName, p.ProductGroup, "
                                    "p.BaseUnitOfMeasure AS UnitOfMeasure, p.CashRegisterVATCode AS VATCode, "
                                    "p.ExciseCategory, p.BaseBarcode AS ProductBarcode, "
                                    "p.PackagingQuantity, p.PackagingUnitOfMeasure, "
                                    "vr.Percentage AS VATPercent "
                                    "FROM ndn_products p "
                                    "LEFT JOIN ndn_vatrates AS vr ON "
                                    "p.CashRegisterVATCode "
                                    "WHERE p.Code = :Code");
        getProductInfoQuery.bindValue(":Code", NDNCode);
        if (getProductInfoQuery.exec() && getProductInfoQuery.next())  {
            double VATValue = grossPrice*(getProductInfoQuery.value("VATPercent").toDouble()/100);
            NSTransactions::__VATCode itemVATCode((NSTransactions::__VATCode::Type)getProductInfoQuery.value("VATCode").toInt());

            salesDocumentLine.setLineNo(row);
            salesDocumentLine.setLocalProductCode(getProductInfoQuery.value("LocalProductCode").toString());
            salesDocumentLine.setProductCode(NDNCode);
            salesDocumentLine.setProductName(getProductInfoQuery.value("ProductName").toString());
            salesDocumentLine.setProductGroup(getProductInfoQuery.value("ProductGroup").toInt());
            salesDocumentLine.setQuantity(quantity);
            salesDocumentLine.setUnitOfMeasure(getProductInfoQuery.value("UnitOfMeasure").toString());
            salesDocumentLine.setNetListPrice(netListPrice);
            salesDocumentLine.setDiscountValue(0);
            salesDocumentLine.setNetUnitPrice(netListPrice);
            salesDocumentLine.setVATCode(itemVATCode);
            salesDocumentLine.setVATPercent(getProductInfoQuery.value("VATPercent").toDouble());
            salesDocumentLine.setGrossUnitPrice(grossPrice);
            salesDocumentLine.setGrossLineTotal(grossPrice*quantity);
            salesDocumentLine.setVATLineTotal(VATValue*quantity);
            salesDocumentLine.setProductStockPrice(grossPrice);
            salesDocumentLine.setGrossConsumerPrice(grossPrice);
            salesDocumentLine.setExciseCategory(NSTransactions::__ExciseCategory((NSTransactions::__ExciseCategory::Type)getProductInfoQuery.value("ExciseCategory").toInt()));
            salesDocumentLine.setProductBarCode(getProductInfoQuery.value("ProductBarcode").toString());
            salesDocumentLine.setProductBarCode(getProductInfoQuery.value("PackagingQuantity").toString());
            salesDocumentLine.setProductBarCode(getProductInfoQuery.value("PackagingUnitOfMeasure").toString());

            salesDocumentLineItems.append(salesDocumentLine);

            grossSum += grossPrice * quantity;
            VATSum += VATValue * quantity;

            bool VATFound = false;
            for (QList<NSTransactions::__DocumentVatBreakDown>::iterator i = VATBreakDownItems.begin(); i != VATBreakDownItems.end(); ++i) {
                if ((*i).vATCode().type() == itemVATCode.type()) {
                    VATFound = true;
                    (*i).setVATAmount((*i).vATAmount() + VATSum);
                    (*i).setNetAmount((*i).netAmount() + netListPrice * quantity);
                    break;
                }
            }

            if (VATFound == false) {
                NSTransactions::__DocumentVatBreakDown VATBreakDownItem;
                VATBreakDownItem.setVATCode(itemVATCode);
                VATBreakDownItem.setNetAmount(netListPrice * quantity);
                VATBreakDownItem.setVATAmount(VATSum);

                VATBreakDownItems.append(VATBreakDownItem);
            }
        } else {
            qWarning() << getProductInfoQuery.lastError() << getLastExecutedQuery(getProductInfoQuery);
            return;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO ndn_salesdocuments "
                  "(CashRegId, Comment, CreatedAt, DocumentDate, DueDate, Gross, "
                  "IssuedOnDate, MovementType, Partner, Reference, "
                  "StornoReference, VAT) "
                  "VALUES "
                  "(:CashRegId, :Comment, :CreatedAt, :DocumentDate, :DueDate, :Gross, "
                  ":IssuedOnDate, :MovementType, :Partner, :Reference, "
                  ":StornoReference, :VAT)");
    query.bindValue(":CashRegId", m_settings->cashRegisterID());
    query.bindValue(":Reference", ui->salesDocumentReferenceLineEdit->text());
    query.bindValue(":StornoReference", ui->salesDocumentStornoReferenceLineEdit->text());
    query.bindValue(":MovementType", (int)ui->salesDocumentMovementTypeComboBox->movementType());
    query.bindValue(":IssuedOnDate", ui->salesDocumentIssuedOnDateTimeEdit->dateTime());
    query.bindValue(":DocumentDate", ui->salesDocumentDateDateTimeEdit->dateTime());
    query.bindValue(":DueDate", ui->salesDocumentDueDateDateTimeEdit->dateTime());
    query.bindValue(":Partner", ui->localPartnerCodeLineEdit->text().toInt());
    query.bindValue(":Comment", ui->salesDocumentCommentLineEdit->text());
    query.bindValue(":CreatedAt", QDateTime::currentDateTime());
    query.bindValue(":Gross", grossSum);
    query.bindValue(":VAT", VATSum);
    if (query.exec()) {
        QString localDocumentID = "" ;
        if (query.driver()->hasFeature(QSqlDriver::LastInsertId)) {
            localDocumentID = query.lastInsertId().toString();
        } else {
            query.exec("SELECT LocalDocumentID from ndn_salesdocuments ORDER by LocalDocumentID DESC LIMIT 1");
            if (query.exec() && query.next())
                localDocumentID = query.value("LocalDocumentID").toString();
        }

        QList<NSTransactions::__SalesDocument> salesDocuments;
        NSTransactions::__SalesDocument salesDocument;
        salesDocument.setShopId(m_settings->shopID());
        salesDocument.setLocalDocumentId(localDocumentID);
        salesDocument.setCashRegId(m_settings->cashRegisterID());
        salesDocument.setReference(ui->salesDocumentReferenceLineEdit->text());
        salesDocument.setStornoReference(ui->salesDocumentStornoReferenceLineEdit->text());
        salesDocument.setMovementType(ui->salesDocumentMovementTypeComboBox->movementType());
        qWarning() << ui->salesDocumentMovementTypeComboBox->movementType();
        salesDocument.setIssuedOnDate(ui->salesDocumentIssuedOnDateTimeEdit->dateTime());
        salesDocument.setDocumentDate(ui->salesDocumentDateDateTimeEdit->dateTime());
        salesDocument.setDueDate(ui->salesDocumentDueDateDateTimeEdit->dateTime());

        salesDocument.setPartner(partnerDataFromUI());

        salesDocument.setCreatedAt(QDateTime::currentDateTime());
        salesDocument.setGross(grossSum);
        salesDocument.setVAT(VATSum);
        salesDocument.setComment(ui->salesDocumentCommentLineEdit->text());

        NSTransactions::__ArrayOfSalesDocumentLine lines;
        lines.setSalesDocumentLine(salesDocumentLineItems);

        salesDocument.setLines(lines);

        NSTransactions::__ArrayOfDocumentVatBreakDown VATBreakDown;
        VATBreakDown.setDocumentVatBreakDown(VATBreakDownItems);
        salesDocument.setVATBreakDown(VATBreakDown);
        salesDocuments.append(salesDocument);

        NSTransactions::TNS__CreateSalesTransaction transaction;
        NSTransactions::__ArrayOfSalesDocument documents;
        documents.setSalesDocument(salesDocuments);
        transaction.setDocuments(documents);
        transaction.setShopId(m_settings->shopID());
        transactions.asyncCreateSalesTransaction(transaction);
    } else {
        qWarning() << query.lastError() << getLastExecutedQuery(query);
    }
}

void DialogNDNMovementTester::createSalesTransactionDone(NSTransactions::TNS__CreateSalesTransactionResponse response)
{
    qWarning() << "createSalesTransactionDone";
    m_settings->setTransactionCallConfig(response.createSalesTransactionResult().currentConfig());
    foreach (NSTransactions::__ResponseCode responseCode, response.createSalesTransactionResult().responseCodes().responseCode()) {
        qWarning() << responseCode.code() << responseCode.message();
    }
}

void DialogNDNMovementTester::createSalesTransactionError(KDSoapMessage msg)
{
    qWarning() << "createSalesTransactionError";
    qWarning() << msg.toXml();
}

void DialogNDNMovementTester::on_pushButtonSubmitStockDocument_clicked()
{
    float VATSum = 0.0f, grossSum = 0.0f;
    QList<NSTransactions::__StockDocumentLine> stockDocumentLineItems;
    QList<NSTransactions::__DocumentVatBreakDown> VATBreakDownItems;

    for (int row = 0; row<m_model->rowCount(QModelIndex()); row++) { // loop through the adde products and create the SOAP message
        NSTransactions::__StockDocumentLine stockDocumentLine;
        QString NDNCode = m_model->data(m_model->index(row, 0), Qt::DisplayRole).toString();
        double quantity = m_model->data(m_model->index(row, 1), Qt::DisplayRole).toDouble();
        double netListPrice = m_model->data(m_model->index(row, 2), Qt::DisplayRole).toDouble();
        double grossPrice = m_model->data(m_model->index(row, 3), Qt::DisplayRole).toDouble();

        QSqlQuery getProductInfoQuery;
        getProductInfoQuery.prepare("SELECT p.rowid AS LocalProductCode, p.Name AS ProductName, p.ProductGroup, "
                                    "p.BaseUnitOfMeasure AS UnitOfMeasure, p.CashRegisterVATCode AS VATCode, "
                                    "p.ExciseCategory, p.BaseBarcode AS ProductBarcode, "
                                    "p.PackagingQuantity, p.PackagingUnitOfMeasure, "
                                    "vr.Percentage AS VATPercent "
                                    "FROM ndn_products p "
                                    "LEFT JOIN ndn_vatrates AS vr ON "
                                    "p.CashRegisterVATCode "
                                    "WHERE p.Code = :Code");
        getProductInfoQuery.bindValue(":Code", NDNCode);
        if (getProductInfoQuery.exec() && getProductInfoQuery.next())  {
            double VATValue = grossPrice*(getProductInfoQuery.value("VATPercent").toDouble()/100);
            NSTransactions::__VATCode itemVATCode((NSTransactions::__VATCode::Type)getProductInfoQuery.value("VATCode").toInt());

            stockDocumentLine.setLineNo(row);
            stockDocumentLine.setLocalProductCode(getProductInfoQuery.value("LocalProductCode").toString());
            stockDocumentLine.setProductCode(NDNCode);
            stockDocumentLine.setProductName(getProductInfoQuery.value("ProductName").toString());
            stockDocumentLine.setProductGroup(getProductInfoQuery.value("ProductGroup").toInt());
            stockDocumentLine.setQuantity(quantity);
            stockDocumentLine.setUnitOfMeasure(getProductInfoQuery.value("UnitOfMeasure").toString());
            stockDocumentLine.setNetListPrice(netListPrice);
            stockDocumentLine.setDiscountValue(0);
            stockDocumentLine.setNetUnitPrice(netListPrice);
            stockDocumentLine.setVATCode(itemVATCode);
            stockDocumentLine.setVATPercent(getProductInfoQuery.value("VATPercent").toDouble());
            stockDocumentLine.setGrossUnitPrice(grossPrice);
            stockDocumentLine.setGrossLineTotal(grossPrice*quantity);
            stockDocumentLine.setVATLineTotal(VATValue*quantity);
            stockDocumentLine.setProductStockPrice(grossPrice);
            stockDocumentLine.setGrossConsumerPrice(grossPrice);
            stockDocumentLine.setExciseCategory(NSTransactions::__ExciseCategory((NSTransactions::__ExciseCategory::Type)getProductInfoQuery.value("ExciseCategory").toInt()));
            stockDocumentLine.setProductBarCode(getProductInfoQuery.value("ProductBarcode").toString());
            stockDocumentLine.setProductBarCode(getProductInfoQuery.value("PackagingQuantity").toString());
            stockDocumentLine.setProductBarCode(getProductInfoQuery.value("PackagingUnitOfMeasure").toString());

            stockDocumentLineItems.append(stockDocumentLine);

            grossSum += grossPrice * quantity;
            VATSum += VATValue * quantity;

            bool VATFound = false;
            for (QList<NSTransactions::__DocumentVatBreakDown>::iterator i = VATBreakDownItems.begin(); i != VATBreakDownItems.end(); ++i) {
                if ((*i).vATCode().type() == itemVATCode.type()) {
                    VATFound = true;
                    (*i).setVATAmount((*i).vATAmount() + VATSum);
                    (*i).setNetAmount((*i).netAmount() + netListPrice * quantity);
                    break;
                }
            }

            if (VATFound == false) {
                NSTransactions::__DocumentVatBreakDown VATBreakDownItem;
                VATBreakDownItem.setVATCode(itemVATCode);
                VATBreakDownItem.setNetAmount(netListPrice * quantity);
                VATBreakDownItem.setVATAmount(VATSum);

                VATBreakDownItems.append(VATBreakDownItem);
            }
        } else {
            qWarning() << "getProductInfoQuery" << getProductInfoQuery.lastError();
            return;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO ndn_stockdocuments "
                  "(Comment, CreatedAt, DocumentDate, Gross, "
                  "IssuedOnDate, MovementType, Partner, Reference, "
                  "StornoReference, VAT, ReasonCode) "
                  "VALUES "
                  "(:Comment, :CreatedAt, :DocumentDate, :Gross, "
                  ":IssuedOnDate, :MovementType, :Partner, :Reference, "
                  ":StornoReference, :VAT, :ReasonCode)");
    query.bindValue(":Reference", ui->stockDocumentReferenceLineEdit->text());
    query.bindValue(":StornoReference", ui->stockDocumentStornoReferenceLineEdit->text());
    query.bindValue(":MovementType", (int)ui->stockDocumentMovementTypeComboBox->movementType());
    query.bindValue(":IssuedOnDate", ui->stockDocumentIssuedOnDateTimeEdit->dateTime());
    query.bindValue(":DocumentDate", ui->stockDocumentDateDateTimeEdit->dateTime());
    query.bindValue(":Partner", ui->localPartnerCodeLineEdit->text().toInt());
    query.bindValue(":Comment", ui->stockDocumentCommentLineEdit->text());
    query.bindValue(":CreatedAt", QDateTime::currentDateTime());
    query.bindValue(":Gross", grossSum);
    query.bindValue(":VAT", VATSum);
    query.bindValue(":ReasonCode", ui->stockDocumentReasoncodeComboBox->type());

    if (query.exec()) { // document saved to database successfully
        QString localDocumentID = "" ;
        if (query.driver()->hasFeature(QSqlDriver::LastInsertId)) {
            localDocumentID = query.lastInsertId().toString();
        } else {
            query.exec("SELECT LocalDocumentID from ndn_stockdocuments ORDER by LocalDocumentID DESC LIMIT 1");
            if (query.exec() && query.next())
                localDocumentID = query.value("LocalDocumentID").toString();
        }

        QList<NSTransactions::__StockDocument> stockDocuments;
        NSTransactions::__StockDocument stockDocument;
        stockDocument.setShopId(m_settings->shopID());
        stockDocument.setLocalDocumentId(localDocumentID);
        stockDocument.setReference(ui->stockDocumentReferenceLineEdit->text());
        stockDocument.setStornoReference(ui->stockDocumentStornoReferenceLineEdit->text());
        stockDocument.setMovementType(ui->stockDocumentMovementTypeComboBox->movementType());
        stockDocument.setIssuedOnDate(ui->stockDocumentIssuedOnDateTimeEdit->dateTime());
        stockDocument.setDocumentDate(ui->stockDocumentDateDateTimeEdit->dateTime());
        if (ui->stockDocumentMovementTypeComboBox->reasonNeeded()) {
            stockDocument.setReasonCode(ui->stockDocumentReasoncodeComboBox->reasonCode());
            stockDocument.setReason(ui->stockDocumentReasonLineEdit->text());
        }

        stockDocument.setPartner(partnerDataFromUI());
        stockDocument.setCreatedAt(QDateTime::currentDateTime());
        stockDocument.setGross(grossSum);
        stockDocument.setVAT(VATSum);
        stockDocument.setComment(ui->stockDocumentCommentLineEdit->text());

        NSTransactions::__ArrayOfStockDocumentLine lines;
        lines.setStockDocumentLine(stockDocumentLineItems);

        stockDocument.setLines(lines);

        NSTransactions::__ArrayOfDocumentVatBreakDown VATBreakDown;
        VATBreakDown.setDocumentVatBreakDown(VATBreakDownItems);
        stockDocument.setVATBreakDown(VATBreakDown);
        stockDocuments.append(stockDocument);

        NSTransactions::TNS__CreateStockTransaction transaction;
        NSTransactions::__ArrayOfStockDocument documents;
        documents.setStockDocument(stockDocuments);
        transaction.setDocuments(documents);
        transaction.setShopId(m_settings->shopID());
        qWarning() << transaction.documents().stockDocument().at(0).shopId();
        qWarning() << transaction.documents().stockDocument().at(0).lines().stockDocumentLine().size();
        transactions.asyncCreateStockTransaction(transaction);
    } else {
        qWarning() << "INSERT STOCKDOC failed";
    }
}

void DialogNDNMovementTester::createStockTransactionDone(NSTransactions::TNS__CreateStockTransactionResponse response)
{
    qWarning() << "createStockTransactionDone";
    m_settings->setTransactionCallConfig(response.createStockTransactionResult().currentConfig());
    foreach (NSTransactions::__ResponseCode responseCode, response.createStockTransactionResult().responseCodes().responseCode()) {
        qWarning() << responseCode.code() << responseCode.key() << responseCode.message();
    }
}

void DialogNDNMovementTester::createStockTransactionError(KDSoapMessage msg)
{
    qWarning() << "createStockTransactionError" << msg.toXml();
}

