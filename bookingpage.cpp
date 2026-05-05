#include "bookingpage.h"
#include "ui_bookingpage.h"

BookingPage::BookingPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BookingPage)
{
    ui->setupUi(this);
}

BookingPage::~BookingPage()
{
    delete ui;
}
