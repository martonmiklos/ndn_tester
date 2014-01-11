#include "ndnreasoncodecombobox.h"

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
