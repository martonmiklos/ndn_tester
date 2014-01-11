#ifndef NDNPRODUCTSMODEL_H
#define NDNPRODUCTSMODEL_H

#include <QSqlQueryModel>

class NDNProductsModel : public QSqlQueryModel
{
    struct NDNProductFilter {
        QString fieldName, condition;
    };

    Q_OBJECT
public:
    explicit NDNProductsModel(QObject *parent = 0);

    void select();

    void setNameFilter(QString name);
    void setBarcodeFilter(QString barcode);
    void setValidFilter(bool validOnly);
    bool validFilter() const {return m_validOnly;}
    void setGroupFilter(int productGroupCode);
signals:
    void rowCountChanged(int);

public slots:
private:
    QString m_queryString, m_filterString;
    QList<NDNProductFilter> filters;

    void setStringFieldFilter(QString field, QString value);

    bool m_validOnly;
};

#endif // NDNPRODUCTSMODEL_H
