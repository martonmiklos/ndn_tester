#ifndef NDNWIDGETS_H
#define NDNWIDGETS_H

#include <QComboBox>
#include <QSqlQuery>

#include "gen_src/Transactions.h"

class NDNReasonCodeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit NDNReasonCodeComboBox(QWidget *parent = 0, bool allowEmpty = true);
    NSTransactions::NDN__ReasonCode_ReasonCodeEnum reasonCode();
    NSTransactions::NDN__ReasonCode_ReasonCodeEnum::Type type();
};

class NDNMovementTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit NDNMovementTypeComboBox(QWidget *parent = 0, bool init = true);
    NSTransactions::NDN__MovementType movementType();
    NSTransactions::NDN__MovementType::Type type();
    bool reasonNeeded();
    bool isStorno();
};

class NDNSalesMovementTypeComboBox : public NDNMovementTypeComboBox
{
    Q_OBJECT
public:
    explicit NDNSalesMovementTypeComboBox(QWidget *parent = 0);
};

class NDNStockMovementTypeComboBox : public NDNMovementTypeComboBox
{
    Q_OBJECT
public:
    explicit NDNStockMovementTypeComboBox(QWidget *parent = 0);
};



#endif // NDNWIDGETS_H
