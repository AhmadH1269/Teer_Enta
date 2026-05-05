#pragma once

#include <QDialog>

#include "userpage.h"

namespace Ui {
class PaymentScreen;
}

class PaymentScreen : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentScreen(QWidget *parent = nullptr);
    ~PaymentScreen();
    void resetContent();
    void applyStyles();
    void showAnimated();




private slots:
    void on_PayAirportButton_clicked();

    void on_MasterCardButton_clicked();

    void on_VisaButton_clicked();

    void on_CheckOutButton_clicked();

    //void showAnimated();

private:
    Ui::PaymentScreen *ui;
};
