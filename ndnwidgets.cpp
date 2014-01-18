#include "ndnwidgets.h"

NDNReasonCodeComboBox::NDNReasonCodeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    QSqlQuery query("SELECT Reasoncode, Name FROM ndn_reasoncodes ORDER BY Name");
    if (query.exec()) {
        while (query.next()) {
            addItem(query.value("Name").toString(), query.value("ReasonCode"));
        }
    }
}

NSTransactions::__ReasonCode_ReasonCodeEnum NDNReasonCodeComboBox::movementType()
{
    return NSTransactions::__ReasonCode_ReasonCodeEnum(
                (NSTransactions::__ReasonCode_ReasonCodeEnum::Type)this->itemData(this->currentIndex()).toInt()
                );
}


NDNMovementTypeComboBox::NDNMovementTypeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    QSqlQuery query("SELECT MovementType, Name FROM ndn_movementtypes ORDER BY Name");
    if (query.exec()) {
        while (query.next()) {
            addItem(query.value("Name").toString(), query.value("MovementType"));
        }
    }
}

NSTransactions::__MovementType NDNMovementTypeComboBox::movementType()
{
    return NSTransactions::__MovementType(
                (NSTransactions::__MovementType::Type)this->itemData(this->currentIndex()).toInt()
                );
}
