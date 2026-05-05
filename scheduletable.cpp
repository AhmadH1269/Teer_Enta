#include "scheduletable.h"
#include "structs.h"
#include "ui_scheduletable.h"
#include "planedetails.h"
#include <QPushButton>


ScheduleTable::ScheduleTable(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::ScheduleTable)
{
    ui->setupUi(this);


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);



    ui->tableWidget->setRowCount(flights.size);


    //Populates the schedule table
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(flights[row].flight_ID + 1));
        QTableWidgetItem *fromItem = new QTableWidgetItem(QString::fromStdString(flights[row].from));
        QTableWidgetItem *toItem = new QTableWidgetItem(QString::fromStdString(flights[row].to));

        auto formattedTime = std::format("{:%F %R}", flights[row].departure_date);
        QTableWidgetItem *leaveItem = new QTableWidgetItem(QString::fromStdString(formattedTime));

        auto formattedTime2 = std::format("{:%T}", flights[row].estimated_time);
        QTableWidgetItem *etaItem = new QTableWidgetItem(QString::fromStdString(formattedTime2));




        ui->tableWidget->setItem(row, 0, idItem);
        ui->tableWidget->setItem(row, 1, fromItem);
        ui->tableWidget->setItem(row, 3, toItem);
        ui->tableWidget->setItem(row, 6, leaveItem);
        ui->tableWidget->setItem(row, 8, etaItem);



        //Creates a button for the Plane detail page
        QPushButton* btn = new QPushButton("Plane " + QString::number(row + 1));

        ui->tableWidget->setCellWidget(row, 5, btn);

        //Checks the row index to find the plane in the database
        connect(btn, &QPushButton::clicked, this, [=, this]() {


            PlaneDetails dialog(this);

            dialog.setTargetRow(row);
            dialog.exec();
        });
    }

    this->setStyleSheet(

        "QPushButton{ background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 2px; "
        "   font-weight: bold; "
        "   margin-top: 2px; }"

        "QPushButton:hover { "
        "   background-color:#1e293b;  "
        "   color: white; }"


        "QWidget { background-color: #020617;; color: white; }"



        "QTableWidget { "
        "   background-color: transparent; "
        "   alternate-background-color: #1e293b"
        "   color: white; "
        "   border: 1px solid #1e293b; "
        "   gridline-color: #27364E; "
        "   font-size: 13px; "
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 10px;"
        "   selection-background-color: #0BF442;"
        "   selection-color: white;"
        "}"

        "QHeaderView::section {"
        "background-color: 1E383B;color: white ;padding: 6px;border: none;border-bottom: 2px solid #444444;font-weight: bold;font-size: 12px;"
        "}"
        );

}

ScheduleTable::~ScheduleTable()
{
    delete ui;
}
