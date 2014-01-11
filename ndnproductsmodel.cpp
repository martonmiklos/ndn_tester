#include "ndnproductsmodel.h"

NDNProductsModel::NDNProductsModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    m_queryString = "SELECT Code, Name FROM ndn_products";
}

void NDNProductsModel::setNameFilter(QString name)
{
    setStringFieldFilter("Name", name);
}

void NDNProductsModel::setValidFilter(bool validOnly)
{
    m_validOnly = validOnly;
    for (int i = 0; i<filters.size(); i++) {
        if (filters.at(i).fieldName == "validto") {
            filters.removeAt(i);
            i--;
        }
    }

    if (validOnly) {
        NDNProductFilter filter;
        filter.fieldName = "validto";
        filter.condition = "(strftime('%s', validto) > strftime('%s', 'now') OR validto is null)";
        filters.append(filter);
    }
    select();
}

void NDNProductsModel::setBarcodeFilter(QString barcode)
{
    setStringFieldFilter("BaseBarCode", barcode);
}



void NDNProductsModel::select()
{
    m_filterString = "";
    if (filters.size()) {
        m_filterString = " WHERE ";
        for (int i = 0; i<filters.size(); i++) {
            m_filterString.append(filters.at(i).condition);

            if (i != filters.size()-1) {
                m_filterString.append(" AND ");
            }
        }
    }

    setQuery(m_queryString+m_filterString);
    emit rowCountChanged(this->rowCount());
}


void NDNProductsModel::setStringFieldFilter(QString field, QString value)
{
    if (value.isEmpty()) { // string filter set to empty -> remove from filtering
        for (int i = 0; i<filters.size(); i++) {
            if (filters.at(i).fieldName == field) {
                filters.removeAt(i);
                i--;
            }
        }
    } else {
        bool found = false;
        for (int i = 0; i<filters.size(); i++) { // if filter is already in the list
            if (filters.at(i).fieldName == field) {
                if (found) { // if multiple filter items found remove the others
                    filters.removeAt(i);
                    i--;
                } else {
                    filters[i].condition = field+" LIKE \"%"+value+"%\""; // set the new condition of the filter
                }
            }
        }

        // filter was not present -> add it
        if (found == false) {
            NDNProductFilter filter;
            filter.fieldName = field;
            filter.condition = field+" LIKE \"%"+value+"%\"";
            filters.append(filter);
        }
    }
    select();
}
