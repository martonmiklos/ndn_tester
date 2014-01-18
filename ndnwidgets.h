#ifndef NDNWIDGETS_H
#define NDNWIDGETS_H

#include <QComboBox>
#include <QSqlQuery>

#include "wsdl_Transactions.h"

class NDNReasonCodeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit NDNReasonCodeComboBox(QWidget *parent = 0);
    NSTransactions::__ReasonCode_ReasonCodeEnum movementType();

signals:

public slots:

};

class NDNMovementTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit NDNMovementTypeComboBox(QWidget *parent = 0);
    NSTransactions::__MovementType movementType();
signals:

public slots:

};


#endif // NDNWIDGETS_H
