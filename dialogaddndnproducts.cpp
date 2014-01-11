#include "dialogaddndnproducts.h"
#include "ui_dialogaddndnproducts.h"

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
                      ui->doubleSpinBoxBruttoPrice->value());
}
