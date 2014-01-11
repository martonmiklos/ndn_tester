#include "dialogndnproductdetails.h"
#include "ui_dialogndnproductdetails.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

#include "ndnenumconverter.h"

DialogNDNProductDetails::DialogNDNProductDetails(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNDNProductDetails)
{
    ui->setupUi(this);
    priceModel = new NDNProductPriceModel(this);
    ui->tableViewPrices->setModel(priceModel);
}

DialogNDNProductDetails::~DialogNDNProductDetails()
{
    delete ui;
}

void DialogNDNProductDetails::showItem(QString code)
{
    ui->nDNCodeLineEdit->setText(code);
    QSqlQuery query; // FIXME custom query
    query.prepare("SELECT p.BaseBarCode, p.BaseUnitOfMeasure, "
                  "p.CustCommCode, p.ExciseCategory, p.LastModifiedOn, "
                  "p.Name, p.PackagingQuantity, p.PackagingUnitOfMeasure, "
                  "p. ShortName, p.ValidTo, "
                  "pg.Name as ProductGroup, pg.isFixedPrice as IsFixedPrice, "
                  "vr.Percentage VATPercentage , vr.Name as CashRegisterVATCode, "
                  "man.Name as ManufacturerName "
                  "FROM ndn_products p "
                  "LEFT JOIN ndn_productgroups pg ON pg.Code = p.ProductGroup "
                  "LEFT JOIN  ndn_vatrates vr  ON vr.CashRegisterVATCode = p.CashRegisterVATCode "
                  "LEFT JOIN ndn_manufacturers man ON man.Code = p.ManufacturerCode "
                  "WHERE p.Code = :Code");
    query.bindValue(":Code", code);
    if (query.exec() && query.next()) {
        ui->barcodeLineEdit->setText(query.value("BaseBarCode").toString());
        ui->unitOfMeasureLineEdit->setText(query.value("BaseUnitOfMeasure").toString());
        QString VATInfo = query.value("CashRegisterVATCode").toString();
        if (query.value("VATPercentage").toInt())
            VATInfo.append(" ("+query.value("VATPercentage").toString()+"%)");
        ui->cashRegisterCodeLineEdit->setText(VATInfo);
        ui->custCommCodeLineEdit->setText(query.value("CustCommCode").toString());
        ui->exciseCategoryLineEdit->setText(NDNEnumConverter::exciseCategoryName(query.value("ExciseCategory").toInt()));
        ui->lastModifiedLineEdit->setText(query.value("LastModifiedOn").toString());
        ui->manufacturerLineEdit->setText(query.value("ManufacturerName").toString());
        ui->fullNameLineEdit->setText(query.value("Name").toString());
        ui->packagingLineEdit->setText(query.value("PackagingQuantity").toString()+" "+query.value("PackagingUnitOfMeasure").toString());
        ui->productGroupLineEdit->setText(query.value("ProductGroup").toString());
        ui->shortNameLineEdit->setText(query.value("ShortName").toString());
        ui->validToLineEdit->setText(query.value("ValidTo").toString());

        if (query.value("isFixedPrice").toBool()) {
            ui->labelPrices->setText(tr("Prices:"));
        } else {
            ui->labelPrices->setText(tr("Prices: (this is not a fixed price product)"));
        }

        this->setWindowTitle(tr("Details of %1").arg(query.value("Name").toString()));
        reloadPrices(!ui->checkBoxPriceValidFilter->isChecked());
        this->show();
    } else {
        QMessageBox msgBox;
        msgBox.setText(tr("NDN product was not found with code: %1").arg(code));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        qWarning() << query.lastQuery();
    }
}

void DialogNDNProductDetails::reloadPrices(bool validOnly)
{
    priceModel->selectPrices(ui->nDNCodeLineEdit->text(), validOnly);
    ui->tableViewPrices->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableViewPrices->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableViewPrices->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void DialogNDNProductDetails::on_checkBoxPriceValidFilter_toggled(bool checked)
{
    reloadPrices(!checked);
}
