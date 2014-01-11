#include "dialogndnproductfilters.h"
#include "ui_dialogndnproductfilters.h"

DialogNDNProductFilters::DialogNDNProductFilters(NDNProductsModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNDNProductFilters),
    m_model(model)
{
    ui->setupUi(this);
    ui->checkBoxValidProductsOnly->setChecked(m_model->validFilter());
}

DialogNDNProductFilters::~DialogNDNProductFilters()
{
    delete ui;
}

void DialogNDNProductFilters::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    ui->lineEditProductNameFilter->setFocus();
}

void DialogNDNProductFilters::on_buttonBox_accepted()
{
}

void DialogNDNProductFilters::on_checkBoxValidProductsOnly_toggled(bool checked)
{
    m_model->setValidFilter(checked);
}

void DialogNDNProductFilters::on_lineEditProductNameFilter_textChanged(const QString &arg1)
{
    m_model->setNameFilter(arg1);
}

void DialogNDNProductFilters::on_lineEditBarcodeFilter_textChanged(const QString &arg1)
{
    m_model->setBarcodeFilter(arg1);
}

