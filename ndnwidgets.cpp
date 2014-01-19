#include "ndnwidgets.h"
#include <QDebug>

NDNReasonCodeComboBox::NDNReasonCodeComboBox(QWidget *parent, bool allowEmpty) :
    QComboBox(parent)
{
    if (allowEmpty)
        addItem(tr("No reason code"), 0);

    QSqlQuery query("SELECT Reasoncode, Name FROM ndn_reasoncodes ORDER BY Name");
    if (query.exec()) {
        while (query.next()) {
            addItem(query.value("Name").toString(), query.value("ReasonCode"));
        }
    }
}

NSTransactions::__ReasonCode_ReasonCodeEnum NDNReasonCodeComboBox::reasonCode()
{
    NSTransactions::__ReasonCode_ReasonCodeEnum ret;
    ret.deserialize(this->itemData(this->currentIndex()));
    return ret;
}

NSTransactions::__ReasonCode_ReasonCodeEnum::Type NDNReasonCodeComboBox::type()
{
    NSTransactions::__ReasonCode_ReasonCodeEnum ret;
    ret.deserialize(this->itemData(this->currentIndex()));
    return ret.type();
}



NDNMovementTypeComboBox::NDNMovementTypeComboBox(QWidget *parent, bool init) :
    QComboBox(parent)
{
    if (init) {
        QSqlQuery query("SELECT MovementType, Name FROM ndn_movementtypes ORDER BY Name");
        if (query.exec()) {
            while (query.next()) {
                addItem(query.value("Name").toString(), query.value("MovementType"));
            }
        }
    }
}
bool NDNMovementTypeComboBox::reasonNeeded()
{
    QSqlQuery query;
    query.prepare("SELECT ReasonCodeNeeded FROM ndn_movementtypes WHERE Code = :Code");
    query.bindValue(":Code", type());
    if (query.exec() && query.next())
        return query.value("ReasonCodeNeeded").toBool();

    return false;
}

bool NDNMovementTypeComboBox::isStorno()
{
    QSqlQuery query;
    query.prepare("SELECT IsStorno FROM ndn_movementtypes WHERE Code = :Code");
    query.bindValue(":Code", type());
    if (query.exec() && query.next())
        return query.value("ReasonCodeNeeded").toBool();

    return false;
}

NSTransactions::__MovementType NDNMovementTypeComboBox::movementType()
{
    NSTransactions::__MovementType ret;
    ret.deserialize(this->itemData(this->currentIndex()));
    return ret;
}

NSTransactions::__MovementType::Type NDNMovementTypeComboBox::type()
{
    NSTransactions::__MovementType ret;
    ret.deserialize(this->itemData(this->currentIndex()));
    return ret.type();
}

NDNSalesMovementTypeComboBox::NDNSalesMovementTypeComboBox(QWidget *parent) :
    NDNMovementTypeComboBox(parent, false)
{
    QSqlQuery query("SELECT MovementType, Name FROM ndn_movementtypes WHERE Sign = -1 ORDER BY Name ");
    if (query.exec()) {
        while (query.next()) {
            addItem(query.value("Name").toString(), query.value("MovementType"));
        }
    }
}


NDNStockMovementTypeComboBox::NDNStockMovementTypeComboBox(QWidget *parent) :
    NDNMovementTypeComboBox(parent, false)
{
    QSqlQuery query("SELECT MovementType, Name FROM ndn_movementtypes WHERE Sign = 1 ORDER BY Name");
    if (query.exec()) {
        while (query.next()) {
            addItem(query.value("Name").toString(), query.value("MovementType"));
        }
    }
}
