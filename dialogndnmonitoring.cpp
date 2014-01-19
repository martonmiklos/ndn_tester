#include "dialogndnmonitoring.h"
#include "ui_dialogndnmonitoring.h"
#include <QDebug>
#include <QSqlQuery>

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
}

DialogNDNMonitoring::~DialogNDNMonitoring()
{
    delete ui;
}

void DialogNDNMonitoring::setTableContentsToTreeWidget(NSMonitoring::__ArrayOfTableContent tables)
{
    ui->treeWidgetGetDocument->clear();
    foreach (NSMonitoring::__TableContent table, tables.tableContent()) {
        QTreeWidgetItem *tableItem = new QTreeWidgetItem(ui->treeWidgetGetDocument);
        tableItem->setText(0, table.tableName());
        int recordCounter = 0;
        foreach (NSMonitoring::__Record record, table.records().record()) {
            QTreeWidgetItem *recordItem = new QTreeWidgetItem(tableItem);
            recordItem->setText(0, tr("Record %1").arg(recordCounter));
            foreach (NSMonitoring::__Column column, record.columns().column()) {
                QTreeWidgetItem *columnItem = new QTreeWidgetItem(recordItem);
                columnItem->setText(0, QString("%1 - %2").arg(column.name()).arg(column.value().value().toString()));
            }
            recordCounter++;
        }
        ui->treeWidgetGetDocument->addTopLevelItem(tableItem);
    }
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
    setTableContentsToTreeWidget(response.getOpeningStockResult().tables());
}


void DialogNDNMonitoring::getOpeningStockError(KDSoapMessage message)
{
    qWarning() << message.faultAsString();
}


void DialogNDNMonitoring::soapError(QString method, KDSoapMessage fault)
{
    qWarning() << method << fault.faultAsString();
}

void DialogNDNMonitoring::on_pushButtonGetDocument_clicked()
{
    NSMonitoring::TNS__GetDocument getDocument;
    NSMonitoring::__MovementType movementType((NSMonitoring::__MovementType::Type) ui->comboBoxMovementType->type());

    getDocument.setShopId(m_settings->shopID());
    getDocument.setLocalDocumentId(ui->lineEditLocalDocumentID->text());
    getDocument.setMovementType(movementType);
    monitoring.asyncGetDocument(getDocument);
}

void DialogNDNMonitoring::getDocumentDone(NSMonitoring::TNS__GetDocumentResponse response)
{
    setTableContentsToTreeWidget(response.getDocumentResult().tables());
}

void DialogNDNMonitoring::getDocumentError(KDSoapMessage message)
{
    qWarning() << message.faultAsString();
}
