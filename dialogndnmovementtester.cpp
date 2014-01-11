#include "dialogndnmovementtester.h"
#include "ui_dialogndnmovementtester.h"
#include <QSqlQuery>
#include <QSqlError>

MovementReportItemsModel::MovementReportItemsModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

void MovementReportItemsModel::addItem(QString ndnCode, double amount, double nettoPrice, double bruttoPrice)
{
    MovementReportItem item;
    item.NDNCode = ndnCode;
    item.netPrice = nettoPrice;
    item.bruttoPrice = bruttoPrice;
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
            return m_items.at(index.row()).bruttoPrice;
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
            return tr("Brutto price");
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

    ui->issuedOnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->documentDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dueDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->salesDocumentDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->salesIssuedOnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->openingStockDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    transactions.clientInterface()->setSslConfiguration(settings->sslConfiguration());
    transactions.clientInterface()->setUsingAddressing(true);
    transactions.setEndPoint(settings->webserviceURLBase()+"Transactions.svc");

    connect(&transactions, SIGNAL(createOpeningStockDone(NSTransactions::TNS__CreateOpeningStockResponse)),
            this , SLOT(createOpeningStockDone(NSTransactions::TNS__CreateOpeningStockResponse)));
    connect(&transactions, SIGNAL(createOpeningStockError(KDSoapMessage)),
            this, SLOT(createOpeningStockError(KDSoapMessage)));

    productAddDialog = new DialogAddNDNProducts(this);
    connect(productAddDialog, SIGNAL(itemSelected(QString,double,double,double)), this, SLOT(itemAdded(QString,double, double,double)));

    m_model = new MovementReportItemsModel(this);
    ui->tableViewItems->setModel(m_model);
}

DialogNDNMovementTester::~DialogNDNMovementTester()
{
    delete ui;
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
        query.prepare("SELECT p.rowid as LocalProductCode, p.Name as ProductName, p.ProductGroup "
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

void DialogNDNMovementTester::on_pushButtonRemoveItem_clicked()
{

}

void DialogNDNMovementTester::on_pushButtonAddItem_clicked()
{
    productAddDialog->show();
}

void DialogNDNMovementTester::createOpeningStockDone(NSTransactions::TNS__CreateOpeningStockResponse response)
{
    m_settings->setBatchSize(response.createOpeningStockResult().currentConfig().batchSize());
    m_settings->setFrequencyInMinutes(response.createOpeningStockResult().currentConfig().frequency());
    m_settings->setOffsetInMinutes(response.createOpeningStockResult().currentConfig().offset());

    foreach (NSTransactions::__ResponseCode responseCode, response.createOpeningStockResult().responseCodes().responseCode()) {
        qWarning() << responseCode.code() << responseCode.key() << responseCode.message();
    }
}

void DialogNDNMovementTester::createOpeningStockError(KDSoapMessage msg)
{

}

void DialogNDNMovementTester::itemAdded(QString ndnCode, double amount, double netPrice, double brutPrice)
{
    m_model->addItem(ndnCode, amount, netPrice, brutPrice);
    ui->tableViewItems->reset();
}

void DialogNDNMovementTester::on_pushButtonSubmitSalesDocument_clicked()
{
    QSqlQuery query;
    query.prepare("INSERT INTO ndn_salesdocuments"
                  "(CashRegId, Comment, CreatedAt, DocumentDate, DueDate, Gross, "
                  "IssuedOnDate, MovementType, Partner, Reference, "
                  "StornoReference, VAT)"
                  "VALUES"
                  "(:CashRegId, :Comment, :CreatedAt, :DocumentDate, :DueDate, :Gross, "
                  ":IssuedOnDate, :MovementType, :Partner, :Reference, "
                  ":StornoReference, :VAT)");
    query.bindValue(":CashRegId", ui->cashRegisterIDLineEdit->text());
    query.bindValue(":Comment", ui->commentLineEdit->text());
    query.bindValue(":CreatedAt", QDateTime::currentDateTime());
    query.bindValue(":DocumentDate", ui->salesDocumentDateDateTimeEdit->dateTime());
    query.bindValue(":DueDate", ui->dueDateDateTimeEdit->dateTime());
    //query.bindValue(":Gross", result.gross());
    query.bindValue(":IssuedOnDate", ui->salesIssuedOnDateTimeEdit->dateTime());
    //query.bindValue(":MovementType", result.movementType());
    //query.bindValue(":Partner", result.partner());
    query.bindValue(":Reference", ui->salesReferenceLineEdit->text());
    query.bindValue(":StornoReference", ui->salesStornoReferenceLineEdit->text());
    //query.bindValue(":VAT", );
    qWarning() << query.exec() << query.lastError();
}
