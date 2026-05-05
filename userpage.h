#pragma once

#include "structs.h"
#include <QMainWindow>
#include <qcalendarwidget.h>
#include <qlabel.h>
#include <QCalendarWidget>
#include <QLabel>
#include <QDate>
#include <QObject>
#include <qboxlayout.h>
#include <QPixmap>
#include <QCalendarWidget>
#include <QTableWidget>
#include <QItemSelection>
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QGridLayout>
#include "data_base.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class UserPageMain;
}
QT_END_NAMESPACE

class UserPageMain : public QMainWindow
{
    Q_OBJECT

     
public:

    explicit UserPageMain(int initialData,QWidget *parent = nullptr);
    ~UserPageMain() override;

    void BacktoHome();

    void savechanges();

    int TotalPrice = 0;

    Vector<Seat> SelectedSeats;

    int IDFlight;

private slots:

    void on_FullSchedButton_clicked();

    void on_BookingPageButton_clicked();

    void on_HomeButton_clicked();

    void on_PaymentButton_clicked();

    void on_CancelButton1_clicked();

    void on_LogOutButton_clicked();

    void on_ProfileButton_clicked();

    void handleSeatClick();

    void BookingInitial();

    void on_ArrivalAirport_currentIndexChanged(int index);

    void on_ChildrenComboBox_currentIndexChanged(int index);

    void applyStyles();

    void setOptions();

    void on_SeatSelection_cellClicked(int row, int column);

    void on_pushButton_clicked();


private:
    Ui::UserPageMain *ui;

    int m_uID;
};
