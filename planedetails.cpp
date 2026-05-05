#include "planedetails.h"
#include "ui_planedetails.h"
#include <QPixmap>
#include "structs.h"


PlaneDetails::PlaneDetails(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlaneDetails)
{
    ui->setupUi(this);

    QPixmap Plane("img/Airplane_PNG_Clipart-1101718429.png");
    int w = ui->PlaneIMG->width();
    int h = ui->PlaneIMG->height();
    ui->PlaneIMG->setPixmap(Plane.scaled(w,h,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));

    this->setStyleSheet(
        "#centralwidget { background-color: #0f172a; border: 1px solid #1e293b; border-radius: 15px;}"

        "QPushButton{ background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; }"

        "QPushButton:hover { "
        "   background-color:#1e293b;  "
        "   color: white; }"

        "QWidget { background-color: #020617;; color: white; }"
    );





}


//Recieves the passed variable from the Schedule table and shows the plane based on index
void PlaneDetails::setTargetRow(int row) {

    ui->ModelOut->setText(QString::fromStdString(planes[row].model));

    ui->ManufacOut->setText(QString::fromStdString(planes[row].manufacturer));

    ui->SeatsOut->setText(QString::number(planes[row].number_of_seats));

    ui->CityOut->setText(QString::fromStdString(planes[row].current_city));

    ui->FlightsNoOut->setText(QString::number(planes[row].number_of_flights));
}

PlaneDetails::~PlaneDetails()
{
    delete ui;
}
