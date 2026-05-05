#include "paymentscreen.h"
#include "ui_userpage.h"
#include "ui_paymentscreen.h"
#include <QTimer>
#include <QMessageBox>
#include <QCloseEvent>
#include <QButtonGroup>
#include <QPropertyAnimation>


PaymentScreen::PaymentScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PaymentScreen)
{
    ui->setupUi(this);

    //sets the images on each label
    QPixmap MasterCard("img/MasterCardLogo.png");
    int w = ui->MasterCardLogo->width();
    int h = ui->MasterCardLogo->height();
    ui->MasterCardLogo->setPixmap(MasterCard.scaled(w,h,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));

    QPixmap Visa("img/VisaLogo.png");
    ui->VisaLogo->setPixmap(Visa.scaled(w,h,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));


    QButtonGroup *PaymentMethod = new QButtonGroup(this);
    PaymentMethod->setExclusive(true);

    PaymentMethod->addButton(ui->VisaButton);
    PaymentMethod->addButton(ui->MasterCardButton);
    PaymentMethod->addButton(ui->PayAirportButton);

    ui->VisaButton->setCheckable(true);
    ui->MasterCardButton->setCheckable(true);
    ui->PayAirportButton->setCheckable(true);

    connect(ui->CheckOutButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->CancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setAttribute(Qt::WA_StyledBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);



    this->setStyleSheet(
        "#PaymentScreen { background-color: #0f172a; border: 1px solid #1e293b; border-radius: 15px;}"

        "QLabel{"
        "   background-color: transparent;"
        "   color: white"
        ""
        "}"



        "QStackedWidget {"
        "   background-color: #020617;"
        "   border: 1px solid transparent;"
        "   border-radius: 15px;"
        "}"


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

        "QPushButton:checked {"
        "   background-color: #0078D4;"
        "   color: white;"
        "   border: 1px solid #005A9E;"
        "   font-weight: bold;"
        "}"



        "#CancelButton { "
        "   background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; "
        "} "
        "#CancelButton:hover { "
        "   background-color: #EF4444; "
        "   color: white; "
        "   border: 2px solid #960D0D; "
        "} "


        "#CheckOutButton { "
        "   background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; "
        "} "
        "#CheckOutButton:hover { "
        "   background-color: #44EF9A; "
        "   color: white; "
        "   border: 2px solid #11C069; "
        "} "



        "QLineEdit {"
        "    background-color: #0A101E;"
        "    border: 1px solid #1A2234;"
        "    border-radius: 8px;"
        "    padding: 8px 12px;"
        "    color: #FFFFFF;"
        "    selection-background-color: #0078D4;"
        "}"
        "QLineEdit:hover {"
        "    border: 1px solid #303B52;"
        "}"

        "QLineEdit:focus {"
        "    border: 1px solid #0078D4;"
        "    background-color: #0D1425;"
        "}"


        "QComboBox {"
        "   border: 1px solid #333333;"
        "   border-radius: 4px;"
        "   padding: 5px 5px;"
        "   background-color: #0f172a;"
        "   color: #E0E0E0;"
        "   font-size: 12px;"
        "   min-width: 6em;"
        "}"

        "QComboBox:focus {"
        "    border: 1px solid #0078D4;"
        "    background-color: #0D1425;"
        "}"

        "QComboBox:hover {border: 1px solid #1e293b;background-color: #1e293b;}"

        "QComboBox:on { border-bottom-left-radius: 0px;border-bottom-right-radius: 0px;}"

        "QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 15px;border-left: 1px solid #333333;}"

        "QComboBox QAbstractItemView {background-color: #0f172a;border: 1px solid #333333;selection-background-color: #0078D4;selection-color: white;outline: none;}"

        "QComboBox QAbstractItemView::item { padding: 8px;color: #E0E0E0;})"



    );

}



PaymentScreen::~PaymentScreen()
{
    delete ui;
}



//Animates the page on start
void PaymentScreen::showAnimated() {
    this->setWindowOpacity(0.0);
    this->show();

    //Creates animation based on the window opacity
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(180);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void PaymentScreen::on_PayAirportButton_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}


void PaymentScreen::on_MasterCardButton_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
}

void PaymentScreen::on_VisaButton_clicked()
{
     ui->stackedWidget->setCurrentIndex(3);
}

//Checkout Message box (also saves data in the booking history)
void PaymentScreen::on_CheckOutButton_clicked()
{
    UserPageMain *mainWin = qobject_cast<UserPageMain*>(this->parent());
    if (mainWin) {
        mainWin->savechanges();
        mainWin->BacktoHome();
    }

    QMessageBox *msg = new QMessageBox();
    msg->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    msg->setText("Check out complete!!!");
    msg->setStyleSheet("QMessageBox {"
                       "    background-color: #050914;"
                       "    border: 1px solid #1A2234;"
                       "}"
                       "QMessageBox QLabel {"
                       "    color: #FFFFFF;"
                       "    font-size: 13px;"
                       "    min-width: 200px;"
                       "    qproperty-alignment: 'AlignCenter';"
                       "    width: 100%;"
                       "}"

                       "QDialogButtonBox {"
                       "    qproperty-centerButtons: true;"
                       "}"

                       "QPushButton{ background-color: transparent; "
                       "   color: #94a3b8; "
                       "   border: 1px solid #334155; "
                       "   padding: 7px; "
                       "   border-radius: 8px; "
                       "   font-weight: bold; "
                       "   margin-top: 7px; "
                       "   width: 100%;"
                       "}"

                       "QPushButton:hover { "
                       "   background-color:#1e293b;  "
                       "   color: white; }"
                       );
    msg->show();
    QTimer::singleShot(2000, msg, &QMessageBox::close);



}
//Saves booking changes
void UserPageMain::savechanges() {
    int row = ui->BookedTableWidget->rowCount();

    ui->BookedTableWidget->insertRow(row);

    ui->BookedTableWidget->setItem(row,0,new QTableWidgetItem(QString::number(row + 1)));

    QString DepartureAir = ui->DepartureAirport->currentText();
    QString ArrivalAir = ui->ArrivalAirport->currentText();
    ui->BookedTableWidget->setItem(row, 1, new QTableWidgetItem(DepartureAir));
    ui->BookedTableWidget->setItem(row, 2, new QTableWidgetItem(ArrivalAir));

    QString departureText[3];
    for (int i = 0; i < 3; ++i) {
        auto formattedTime = std::format("{:%F %R}", flights[i].departure_date);
        departureText[i] = QString::fromStdString(formattedTime);
    }


    if(ui->FirstAvFlight->isChecked()) {

        IDFlight = 0;
        ui->BookedTableWidget->setItem(row, 3, new QTableWidgetItem(departureText[0]));
    };
    if(ui->SecondAvFlight->isChecked()) {
        IDFlight = 1;
        ui->BookedTableWidget->setItem(row, 3, new QTableWidgetItem(departureText[1]));
    };
    if(ui->ThirdAvFlight->isChecked()) {
        IDFlight = 2;
        ui->BookedTableWidget->setItem(row, 3, new QTableWidgetItem(departureText[2]));
    };




    int maxSelection = 1;

    maxSelection += ui->AdultComboBox->currentIndex();
    maxSelection += ui->ChildrenComboBox->currentIndex();

    ui->BookedTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(maxSelection)));
    ui->BookedTableWidget->setItem(row,5, new QTableWidgetItem(QString::number(TotalPrice)));

}


void UserPageMain::BacktoHome() {

    ui->stackedWidget->setCurrentIndex(0);
    this->BookingInitial();

}
