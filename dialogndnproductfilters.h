#ifndef DIALOGNDNPRODUCTFILTERS_H
#define DIALOGNDNPRODUCTFILTERS_H

#include <QDialog>
#include <QAbstractButton>
#include "ndnproductsmodel.h"

namespace Ui {
class DialogNDNProductFilters;
}

class DialogNDNProductFilters : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNDNProductFilters(NDNProductsModel *model, QWidget *parent = 0);
    ~DialogNDNProductFilters();
    void showEvent(QShowEvent * event);
private slots:
    void on_buttonBox_accepted();

    void on_checkBoxValidProductsOnly_toggled(bool checked);

    void on_lineEditProductNameFilter_textChanged(const QString &arg1);

    void on_lineEditBarcodeFilter_textChanged(const QString &arg1);


private:
    Ui::DialogNDNProductFilters *ui;
    NDNProductsModel *m_model;
};

#endif // DIALOGNDNPRODUCTFILTERS_H
