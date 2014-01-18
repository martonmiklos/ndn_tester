#ifndef DIALOGADDNDNPRODUCTS_H
#define DIALOGADDNDNPRODUCTS_H

#include <QDialog>
#include "ndnproductsmodel.h"
#include "dialogndnproductfilters.h"

namespace Ui {
class DialogAddNDNProducts;
}

class DialogAddNDNProducts : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddNDNProducts(QWidget *parent = 0);
    ~DialogAddNDNProducts();

private slots:
    void on_pushButtonFilter_clicked();

    void on_tableViewProducts_doubleClicked(const QModelIndex &index);
    void on_tableViewProducts_clicked(const QModelIndex &index);

signals:
    void itemSelected(QString ndnCode, double amount, double netPrice, double brutPrice);

private:
    Ui::DialogAddNDNProducts *ui;
    NDNProductsModel *m_model;
    DialogNDNProductFilters *ndnProductFilterDialog;
};

#endif // DIALOGADDNDNPRODUCTS_H
