#ifndef DIALOGNDNPRODUCTDETAILS_H
#define DIALOGNDNPRODUCTDETAILS_H

#include <QDialog>
#include "ndnproductpricemodel.h"

namespace Ui {
class DialogNDNProductDetails;
}

class DialogNDNProductDetails : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNDNProductDetails(QWidget *parent = 0);
    ~DialogNDNProductDetails();
    void showItem(QString code);
    void reloadPrices(bool validOnly);

private slots:

    void on_checkBoxPriceValidFilter_toggled(bool checked);

private:
    Ui::DialogNDNProductDetails *ui;
    NDNProductPriceModel *priceModel;
};

#endif // DIALOGNDNPRODUCTDETAILS_H
