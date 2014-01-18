#include "dialogndnmonitoring.h"
#include "ui_dialogndnmonitoring.h"
#include <QDebug>
#include <QSqlQuery>

OpeningStockModel::OpeningStockModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}


int OpeningStockModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 5;
}

int OpeningStockModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_items.count();
}

QVariant OpeningStockModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case 0:
            return m_items.at(index.row()).lineNo;
        case 1:
            return m_items.at(index.row()).localProductCode;
        case 2:
            return m_items.at(index.row()).NDNProductCode;
        case 3:
            return m_items.at(index.row()).name;
        case 4:
            return m_items.at(index.row()).quantity;
        }
    }
    return QVariant();
}

QVariant OpeningStockModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("");
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


DialogNDNMonitoring::DialogNDNMonitoring(NDNSettings *settings, QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::DialogNDNMonitoring),
    m_settings(settings)
{
    ui->setupUi(this);
    monitoring.clientInterface()->setSslConfiguration(settings->sslConfiguration());
    monitoring.clientInterface()->setUsingAddressing(true);
    monitoring.setEndPoint(settings->webserviceURLBase()+"Monitoring.svc");

    connect(&monitoring, SIGNAL(getOpeningStockDone(NSMonitoring::TNS__GetOpeningStockResponse)),
            this, SLOT(getOpeningStockDone(NSMonitoring::TNS__GetOpeningStockResponse)));
    connect(&monitoring, SIGNAL(getOpeningStockError(KDSoapMessage)),
            this, SLOT(getOpeningStockError(KDSoapMessage)));

    connect(&monitoring, SIGNAL(getDocumentDone(NSMonitoring::TNS__GetDocumentResponse)),
            this, SLOT(getDocumentDone(NSMonitoring::TNS__GetDocumentResponse)));
    connect(&monitoring, SIGNAL(getDocumentError(KDSoapMessage)),
            this, SLOT(getDocumentError(KDSoapMessage)));

    connect(&monitoring, SIGNAL(soapError(QString,KDSoapMessage)), this, SLOT(soapError(QString,KDSoapMessage)));

    ui->dateTimeEditOpeningStock->setDateTime(QDateTime::currentDateTime());

    m_openingStockModel = new OpeningStockModel(this);
    ui->tableViewOpeningStock->setModel(m_openingStockModel);
}

DialogNDNMonitoring::~DialogNDNMonitoring()
{
    delete ui;
}

void DialogNDNMonitoring::on_pushButtonGetOpeningStock_clicked()
{
    NSMonitoring::TNS__GetOpeningStock getOpeningStockJob;
    getOpeningStockJob.setShopId(m_settings->shopID());

    // zero  out the msec part of the timestamp
    QDateTime created = ui->dateTimeEditOpeningStock->dateTime();
    created = created.addMSecs(-created.time().msec());
    getOpeningStockJob.setCreationDate(created);

    monitoring.asyncGetOpeningStock(getOpeningStockJob);
}

void DialogNDNMonitoring::getOpeningStockDone(NSMonitoring::TNS__GetOpeningStockResponse response)
{
    qWarning() << "getOpeningStockDone();" <<  response.getOpeningStockResult().tables().tableContent().size();

    foreach (NSMonitoring::__TableContent table, response.getOpeningStockResult().tables().tableContent()) {
        qWarning() << table.tableName();
        foreach (NSMonitoring::__Record record, table.records().record()) {
            foreach (NSMonitoring::__Column column, record.columns().column()) {
                qWarning() << column.name() << column.typeName() << column.value();
            }
        }
    }
}


void DialogNDNMonitoring::getOpeningStockError(KDSoapMessage message)
{
    qWarning() << message.faultAsString();
}

void DialogNDNMonitoring::getDocumentDone(NSMonitoring::TNS__GetDocumentResponse response)
{

}

void DialogNDNMonitoring::getDocumentError(KDSoapMessage message)
{

}

void DialogNDNMonitoring::soapError(QString method, KDSoapMessage fault)
{
    qWarning() << method << fault.faultAsString();
}

void DialogNDNMonitoring::on_pushButtonGetDocument_clicked()
{
    NSMonitoring::TNS__GetDocument getDocument;
    getDocument.setShopId(m_settings->shopID());
    getDocument.setLocalDocumentId(ui->lineEditLocalDocumentID->text());
    NSMonitoring::__MovementType movementType;
    movementType.deserialize(ui->comboBoxMovementType->itemData(ui->comboBoxMovementType->currentIndex()));
    getDocument.setMovementType(movementType);
    monitoring.asyncGetDocument(getDocument);
}
