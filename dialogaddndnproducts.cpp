#include "dialogaddndnproducts.h"
#include "ui_dialogaddndnproducts.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DialogAddNDNProducts::DialogAddNDNProducts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddNDNProducts)
{
    ui->setupUi(this);
    m_model = new NDNProductsModel(this);
    ui->tableViewProducts->setModel(m_model);

    ndnProductFilterDialog = new DialogNDNProductFilters(m_model, this);
}

DialogAddNDNProducts::~DialogAddNDNProducts()
{
    delete ui;
}

void DialogAddNDNProducts::on_pushButtonFilter_clicked()
{
    ndnProductFilterDialog->show();
}

void DialogAddNDNProducts::on_tableViewProducts_doubleClicked(const QModelIndex &index)
{
    QModelIndex codeIndex = m_model->index(index.row(), 0);
    emit itemSelected(m_model->data(codeIndex).toString(),
                      ui->doubleSpinBoxAmount->value(),
                      ui->doubleSpinBoxNetPrice->value(),
                      ui->doubleSpinBoxGrossPrice->value());
}

void DialogAddNDNProducts::on_tableViewProducts_clicked(const QModelIndex &index)
{
    QString productCode =  m_model->data(m_model->index(index.row(), 0)).toString();
    QSqlQuery query;
    query.prepare("SELECT MAX(Price) AS Price FROM ndn_productprices WHERE ProductCode = :ProductCode "
                  "AND (strftime('%s', validto) > strftime('%s', 'now') OR validto is null)");
    query.bindValue(":ProductCode", productCode);
    if (query.exec() && query.next()) {
        ui->doubleSpinBoxGrossPrice->setValue(query.value("Price").toDouble());
        ui->doubleSpinBoxNetPrice->setValue(query.value("Price").toDouble() * 0.90f);
    } else {
        qWarning() << query.lastError();
    }
}

void DialogAddNDNProducts::on_pushButtonAdd_clicked()
{
    // TODO
    if (ui->tableViewProducts->selectionModel()->hasSelection()) {
        QModelIndex codeIndex = m_model->index(ui->tableViewProducts->selectionModel()->selectedRows().first().row(), 0);
        on_tableViewProducts_doubleClicked(codeIndex);
    }
}
