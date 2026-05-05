#include "userpage.h"
#include "./ui_userpage.h"

#include "enums.h"
#include "functions.h"
#include "paymentscreen.h"
#include "scheduletable.h"
#include "structs.h"
#include "ui_userpage.h"
#include "ui_paymentscreen.h"
#include "login.h"

#include <QScrollBar>
#include <qbuttongroup.h>

UserPageMain::UserPageMain(int initialData,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserPageMain)
{
    ui->setupUi(this);
    int uID = initialData;
    m_uID = uID;
    QPixmap home("img/Home_Icon.png");


    //Sets up the table in the home page
    ui->MaintableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->MaintableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->MaintableWidget->setAlternatingRowColors(true);
    ui->MaintableWidget->setRowCount(flights.size);


    //Populates the table
    for (int row = 0; row < ui->MaintableWidget->rowCount(); row++) {
        QTableWidgetItem *fromItem = new QTableWidgetItem(QString::fromStdString(flights[row].from));
        QTableWidgetItem *toItem = new QTableWidgetItem(QString::fromStdString(flights[row].to));

        auto formattedTime = std::format("{:%F %R}", flights[row].departure_date);
        QTableWidgetItem *leaveItem = new QTableWidgetItem(QString::fromStdString(formattedTime));

        ui->MaintableWidget->setItem(row, 0, fromItem);
        ui->MaintableWidget->setItem(row, 1, toItem);
        ui->MaintableWidget->setItem(row, 2, leaveItem);
    }

    //Adds the picture in the booking page
    QPixmap pixmap1 ("img/Test_Image.png");
    ui->DestinationImaGE->setPixmap(pixmap1);
    ui->DestinationImaGE->setScaledContents(true);



    //Adds the flight selection choices to a group
    QButtonGroup *SelectedFlight = new QButtonGroup(this);

    SelectedFlight->setExclusive(true);

    SelectedFlight->addButton(ui->FirstAvFlight);
    SelectedFlight->addButton(ui->SecondAvFlight);
    SelectedFlight->addButton(ui->ThirdAvFlight);

    ui->FirstAvFlight->setCheckable(true);
    ui->SecondAvFlight->setCheckable(true);
    ui->ThirdAvFlight->setCheckable(true);


    //Start of Seat Selection set-up
    ui->SeatSelection->setColumnWidth(15, 15);

    QString seatClass[3] = {"Economy","Business", "First Class"};
    double price[3] = {150.50, 200.00, 350.99};


    //Sets the size and headers of the columna
    ui->SeatSelection->setRowCount(15);
    ui->SeatSelection->setColumnCount(9);

    QStringList RowNums;
    QStringList ColNames = {"A", "B", "Aisle", "C", "D", "E", "Aisle", "F", "G"};
    ui->SeatSelection->setHorizontalHeaderLabels(ColNames);

    //To add the seat buttons to the table
    for (int row = 0; row < 15; ++row) {

        RowNums << QString::number(row + 1);
        ui->SeatSelection->setVerticalHeaderLabels(RowNums);


        for (int col = 0; col < 9; ++col) {

            QPushButton *btn = new QPushButton();

            //Does not add a button to specific columns (Aisle Columns)
            if (col == 2 || col == 6){
                QTableWidgetItem *item = new QTableWidgetItem(RowNums[row]);
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setBackground(Qt::black);
                item->setTextAlignment(Qt::AlignCenter);
                ui->SeatSelection->setItem(row, col, item);
                continue;
            };


            QString tooltipText;

            QString colLabel = ColNames.at(col);

            //Sets the buttons tool tip and tier based on the row
            //First Class
            if(row < 2){
                tooltipText = QString("Col: %1\nRow: %2\nClass: %3\nPrice: $%4\nHas Wifi and Power Outlets for charging!")
                .arg(colLabel)    // Use the letter (A, B, C...) instead of the index
                    .arg(row + 1)   // Use row + 1 for human-readable numbers
                    .arg(seatClass[2])
                    .arg((double)flights[0].high_price, 0, 'f', 2);
                btn->setStyleSheet(
                    "QPushButton {border-image: url(img/FirstClassSeat.png); min-width: 50px; min-height: 50px;}"
                    "QPushButton:checked {border-image: url(img/SelectedSeat.png);background-color: transparent;}");

            }

            //Business Class
            else if(row < 6){
                tooltipText = QString("Col: %1\nRow: %2\nClass: %3\nPrice: $%4\nHas Wifi and Power Outlets for charging!")
                .arg(colLabel)    // Use the letter (A, B, C...) instead of the index
                    .arg(row + 1)   // Use row + 1 for human-readable numbers
                    .arg(seatClass[1])
                    .arg((double)flights[0].mid_price, 0, 'f', 2);
                btn->setStyleSheet(
                    "QPushButton {border-image: url(img/BusinessSeat.png); min-width: 50px; min-height: 50px;}"
                    "QPushButton:checked {border-image: url(img/SelectedSeat.png);background-color: transparent;}");
            }

            //Economy Class
            else{
                tooltipText = QString("Col: %1\nRow: %2\nClass: %3\nPrice: $%4\nHas Wifi and Power Outlets for charging!")
                .arg(colLabel)    // Use the letter (A, B, C...) instead of the index
                    .arg(row + 1)   // Use row + 1 for human-readable numbers
                    .arg(seatClass[0])
                    .arg((double)flights[0].low_price, 0, 'f', 2);
                btn->setStyleSheet(
                    "QPushButton {border-image: url(img/EconomySeat.png); min-width: 50px; min-height: 50px;}"
                    "QPushButton:checked {border-image: url(img/SelectedSeat.png); background-color: transparent;}");
            }


            btn->setCheckable(true);
            btn->setChecked(false);
            btn->setProperty("myRow", row);

            connect(btn, &QPushButton::clicked, this, &UserPageMain::handleSeatClick);
            connect(btn, &QPushButton::clicked, this, [=, this]() {
                this->on_SeatSelection_cellClicked(row, col);
            });

            btn->setToolTip(tooltipText);
            ui->SeatSelection->setCellWidget(row, col, btn);
        }
    }
    ui->SeatSelection->resizeColumnsToContents();
    ui->SeatSelection->resizeRowsToContents();
    //End of Seat Selection set-up


    //Sets the profile page based on the user id
    ui->UsernameLabel->setText(QString::fromStdString(users[uID].username));

    ui->EmailLabel->setText(QString::fromStdString(users[uID].email));


    //Signal and slot of the arrival airport drop down
    connect(ui->ArrivalAirport, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &UserPageMain::on_ArrivalAirport_currentIndexChanged);


    this->applyStyles();

    this->setOptions();

    ui->frame->setVisible(false);

    ui->pushButton->setCheckable(true);
}

UserPageMain::~UserPageMain()
{
    SAVE_DATA();
    delete ui;
}

//Resets Payment screen
void PaymentScreen::resetContent(){
    ui->PasswordBox->clear();
    ui->UsernameBox->clear();
    ui->stackedWidget->setCurrentIndex(0);
    ui->NameBox1->clear();
    ui->NameBox2->clear();
    ui->CardNo1->clear();
    ui->CardNoBox2->clear();
    ui->CVVBox1->clear();
    ui->CVVBox2->clear();
    ui->MonthBox1->setCurrentIndex(-1);
    ui->MonthBox2->setCurrentIndex(-1);
    ui->YearBox1->setCurrentIndex(-1);
    ui->YearBox2->setCurrentIndex(-1);
}


//Resets the Booking Page
void UserPageMain::BookingInitial(){
    //Airport Choices Reset
    ui->ArrivalAirport->blockSignals(true);
    ui->ArrivalAirport->setCurrentIndex(-1);
    ui->ArrivalAirport->blockSignals(false);
    ui->DepartureAirport->setCurrentIndex(-1);


    //Flight Choice Reset
    ui->AvailableFlights->setVisible(false);
    ui->FirstAvFlight->setChecked(false);
    ui->SecondAvFlight->setChecked(false);
    ui->ThirdAvFlight->setChecked(false);

    //Number of passengers Reset
    ui->AdultComboBox->setVisible(false);
    ui->AdultLabel->setVisible(false);
    ui->ChildrenComboBox->setVisible(false);
    ui->ChildrenLabel->setVisible(false);

    //Seat Choice & Checkout Reset
    ui->SeatSelection->setVisible(false);
    ui->PaymentButton->setVisible(false);
    ui->PriceDisplay->setVisible(false);
    ui->PriceTotal->setVisible(false);






    ui->AdultComboBox->setCurrentIndex(-1);
    ui->ChildrenComboBox->setCurrentIndex(-1);
    for (int r = 0; r < ui->SeatSelection->rowCount(); ++r) {
        for (int c = 0; c < ui->SeatSelection->columnCount(); ++c) {
            QPushButton *btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));

            if(btn && btn->isChecked()){
                btn->setChecked(false);
            }
        }
    }
}


//Home and Profile Buttons
void UserPageMain::on_HomeButton_clicked()
{
    BookingInitial();


    //Home path Button
    ui->stackedWidget->setCurrentIndex(0);

}


//Profile path Button
void UserPageMain::on_ProfileButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


//Booking path Button
void UserPageMain::on_BookingPageButton_clicked()
{
    BookingInitial();
    ui->stackedWidget->setCurrentIndex(1);
}


//Full Schedule Button
void UserPageMain::on_FullSchedButton_clicked()
{
    ScheduleTable flightSched;
    flightSched.setModal(true);
    flightSched.setMinimumHeight(480);
    flightSched.setMinimumWidth(1100);
    flightSched.setWindowTitle("Flight Schedule");
    flightSched.exec();
}


//Resets the booking page and send user back to home
void UserPageMain::on_CancelButton1_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cancelling...", "Don't wanna make memories abroad anymore?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->AdultComboBox->setCurrentIndex(-1);
        ui->ChildrenComboBox->setCurrentIndex(-1);
        ui->ArrivalAirport->setCurrentIndex(-1);
        ui->DepartureAirport->setCurrentIndex(-1);
        ui->SeatSelection->clearSelection();
        ui->AdultComboBox->setVisible(false);
        ui->AdultLabel->setVisible(false);
        ui->ChildrenComboBox->setVisible(false);
        ui->ChildrenLabel->setVisible(false);
        ui->stackedWidget->setCurrentIndex(0);
    } else {
        // User clicked No
    }
}


//Logout Button
void UserPageMain::on_LogOutButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Loging Out...", "Are you sure you want to log out?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Login* loginpage = new Login();
        loginpage->show();
        this->close();
    }
    else {

    }
}


//To show the Booking History
void UserPageMain::on_pushButton_clicked()
{
    if (ui->pushButton->isChecked()){
        ui->frame->setVisible(true);
        ui->pushButton->setText("Hide Booking History");
    }
    else {
        ui->frame->setVisible(false);
        ui->pushButton->setText("Booking History");
    }
}


//Populates the airport drop downs
void UserPageMain::setOptions() {


    for(int i = 0; i < flights.size; i++) {
        ui->DepartureAirport->addItem(QString::fromStdString(flights[i].from));
        ui->ArrivalAirport->addItem(QString::fromStdString(flights[i].to));

    };

}


//Logic to show the widgets in the booking page & to show the matching flight routes
void UserPageMain::on_ArrivalAirport_currentIndexChanged(int index)
{
    ui->AvailableFlights->setVisible(true);
    ui->AdultComboBox->setVisible(true);
    ui->AdultComboBox->setCurrentIndex(-1);
    ui->AdultLabel->setVisible(true);
    ui->ChildrenComboBox->setVisible(true);
    ui->ChildrenComboBox->setCurrentIndex(-1);
    ui->ChildrenLabel->setVisible(true);

    //Convert the selections to the format needed
    QString SelLeaveAirport = ui->DepartureAirport->currentText();
    QString SelArriveAirport = ui->ArrivalAirport->currentText();

    Vector<int> flight_ids = search_flights(SelLeaveAirport.toStdString(),SelArriveAirport.toStdString());

    //Check if the vector is empty
    if (0 >= flight_ids.size) {
        QMessageBox *msg = new QMessageBox();
        msg->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        msg->setText("No flights found matching the travel route.\nTry again");
        msg->setStyleSheet("QMessageBox {"
                           "    background-color: #050914;" /* Midnight navy */
                           "    border: 1px solid #1A2234;"
                           "}"
                           "QMessageBox QLabel {"
                           "    color: #FFFFFF;" /* White text */
                           "    font-size: 13px;"
                           "    min-width: 400px;" /* Force the label to be wide */
                           "    qproperty-alignment: 'AlignCenter';"
                           "    width: 100%;" /* Qt-specific CSS property */
                           "}"

                           "QDialogButtonBox {"
                           "    qproperty-centerButtons: true;" /* Centers the buttons in the box */
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

        BookingInitial();
        return;
    }
    //First Details Block
    if (flight_ids.size > 0) {
        IDFlight = flight_ids[0];
        ui->FirstPlane->setText("Plane: " + QString::fromStdString(planes[IDFlight].manufacturer) + " " + QString::fromStdString(planes[IDFlight].model));
        ui->DestinationLabel1->setText("Goes: " + SelArriveAirport);
        std::string formattedTime = std::format("{:%F %R}", flights[IDFlight].departure_date);
        ui->LeavingDate1->setText("Leaves At: " + QString::fromStdString(formattedTime));
    }

    //Second Details Block
    if (flight_ids.size > 1) {
        IDFlight = flight_ids[1];
        ui->SecondPlane->setText("Plane: " + QString::fromStdString(planes[IDFlight].manufacturer) + " " + QString::fromStdString(planes[IDFlight].model));
        ui->DestinationLabel2->setText("Goes: " + SelArriveAirport);
        std::string formattedTime = std::format("{:%F %R}", flights[IDFlight].departure_date);
        ui->LeavingDate2->setText("Leaves At: " + QString::fromStdString(formattedTime));
    }

    //Third Details Block
    if (flight_ids.size > 2) {
        IDFlight = flight_ids[2];
        ui->ThirdPlane->setText("Plane: " + QString::fromStdString(planes[IDFlight].manufacturer) + " " + QString::fromStdString(planes[IDFlight].model));
        ui->DestinationLabel3->setText("Goes: " + SelArriveAirport);
        std::string formattedTime = std::format("{:%F %R}", flights[IDFlight].departure_date);
        ui->LeavingDate3->setText("Leaves At: " + QString::fromStdString(formattedTime));
    }
}


void UserPageMain::on_ChildrenComboBox_currentIndexChanged(int index)
{
    ui->SeatSelection->setVisible(true);

}


//Seat Handling(Max selected seats AND to show the payment button)
void UserPageMain::handleSeatClick() {
    int maxSelection = 1;

    maxSelection += ui->AdultComboBox->currentIndex();
    maxSelection += ui->ChildrenComboBox->currentIndex();

    QList<QPushButton*> allButtons = ui->SeatSelection->findChildren<QPushButton*>();
    int currentSelected = 0;
    for (QPushButton* btn : allButtons) {
        if (btn->isChecked()) currentSelected++;
    }

    QPushButton* clickedBtn = qobject_cast<QPushButton*>(sender());

    ui->PriceTotal->setVisible(true);
    ui->PriceDisplay->setVisible(true);
    ui->PaymentButton->setVisible(true);

    if (currentSelected > maxSelection) {
        clickedBtn->setChecked(false);
        return;
    }




}


//Handles the price
void UserPageMain::on_SeatSelection_cellClicked(int Row, int column)
{
    int SelectedPrice = 0;
    TotalPrice = 0;


    for (int r = 0; r < ui->SeatSelection->rowCount(); ++r) {
        for (int c = 0; c < ui->SeatSelection->columnCount(); ++c) {
            QPushButton *btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));


            if(btn && btn->isChecked()){

                Seat seat;
                seat.row = r;
                seat.column = c;

                if (r < 2) {
                    seat.tier = 1;
                    SelectedPrice += flights[IDFlight].high_price;
                }
                else if (r < 6) {
                    seat.tier = 2;
                    SelectedPrice +=  flights[IDFlight].mid_price;
                }
                else {
                    seat.tier = 3;
                    SelectedPrice +=  flights[IDFlight].low_price;
                }

                SelectedSeats.push_back(seat);

            }
            else {

                for (int i = 0; i < SelectedSeats.size; ++i) {

                    if(SelectedSeats[i].row == r && SelectedSeats[i].column == c){
                        SelectedSeats.erase(i);
                    };
                }
            }
        }

        TotalPrice += SelectedPrice;


        ui->PriceDisplay->setText("$" + QString::number(TotalPrice));
    }
}


//To show payment page & Create a ticket
void UserPageMain::on_PaymentButton_clicked()
{
    int adults = ui->AdultComboBox->currentIndex(), children = ui->ChildrenComboBox->currentIndex();

    CREATE_TICKET newTicket = create_ticket(users[m_uID].user_ID,IDFlight, adults , children , SelectedSeats);

    if(newTicket == BROKE_RULES) {
        QMessageBox *msg = new QMessageBox();
        msg->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        msg->setText("Maximum selected adults and children exceeded");
        msg->setStyleSheet("QMessageBox {"
                           "    background-color: #050914;" /* Midnight navy */
                           "    border: 1px solid #1A2234;"
                           "}"
                           "QMessageBox QLabel {"
                           "    color: #FFFFFF;" /* White text */
                           "    font-size: 13px;"
                           "    min-width: 400px;" /* Force the label to be wide */
                           "    qproperty-alignment: 'AlignCenter';"
                           "    width: 100%;" /* Qt-specific CSS property */
                           "}"

                           "QDialogButtonBox {"
                           "    qproperty-centerButtons: true;" /* Centers the buttons in the box */
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
    }

    if(newTicket == TICKET_SUCCESS){
        PaymentScreen *Pay = new PaymentScreen(this);
        Pay->resetContent();
        Pay->setModal(true);
        Pay->setWindowTitle("Payment Screen");
        Pay->showAnimated();
    }

    else if(newTicket == CANNOT_ADD_TICKETS){
        QMessageBox *msg = new QMessageBox();
        msg->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        msg->setText("More than 10 tickets have been made");
        msg->setStyleSheet("QMessageBox {"
                           "    background-color: #050914;" /* Midnight navy */
                           "    border: 1px solid #1A2234;"
                           "}"
                           "QMessageBox QLabel {"
                           "    color: #FFFFFF;" /* White text */
                           "    font-size: 13px;"
                           "    min-width: 400px;" /* Force the label to be wide */
                           "    qproperty-alignment: 'AlignCenter';"
                           "    width: 100%;" /* Qt-specific CSS property */
                           "}"

                           "QDialogButtonBox {"
                           "    qproperty-centerButtons: true;" /* Centers the buttons in the box */
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
    }

}


//Function for the design scheme of app
void UserPageMain::applyStyles() {
    for (int i = 0; i < ui->stackedWidget->count(); ++i) {
        ui->stackedWidget->widget(i)->setAttribute(Qt::WA_StyledBackground, true);
    }
    ui->stackedWidget->setAttribute(Qt::WA_StyledBackground, true);
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
    ui->BookedTableWidget->setAlternatingRowColors(true);
    this->setStyleSheet(
        "#centralwidget {"
        "   background-color: #0f172a;"
        "   border: 1px solid #1e293b;"
        "   border-radius: 15px;"
        "}"


        "QWidget {"
        "   background-color: #020617;"
        "   color: white;"
        "}"


        "QScrollBar:vertical {"
        "    border: 1px solid #20202C;"
        "    background: #20202C;"
        "    width: 12px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #27364E;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    border: none;"
        "    background: none;"
        "    height: 0px;"
        "}"


        "QStackedWidget {"
        "   background-color: #020617;"
        "   border: 1px solid #020617;"
        "   padding: 5px;"
        "   margin: 5px;"
        "   border-radius: 15px;"
        "}"


        "QPushButton{ background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; "
        "}"
        "QPushButton:hover { "
        "   background-color:#1e293b;  "
        "   color: white; "
        "}"
        "QPushButton:checked {"
        "   background-color: #0078D4;"
        "   color: white;"
        "   border: 1px solid #005A9E;"
        "   font-weight: bold;"
        "}"

        "#CancelButton1 { "
        "   background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; "
        "} "
        "#CancelButton1:hover { "
        "   background-color: #ef4444; " // Red background
        "   color: white; "
        "   border: 3px solid #ef4444; "
        "} "

        "#LogOutButton { "
        "   background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; "
        "} "
        "#LogOutButton:hover { "
        "   background-color: #ef4444; " // Red background
        "   color: white; "
        "   border: 3px solid #ef4444; "
        "}"


        "QTableWidget { "
        "   background-color: transparent; "
        "   alternate-background-color: #374B6D"
        "   color: white; "
        "   border: 1px solid #1e293b; "
        "   gridline-color: #27364E; "
        "   font-size: 13px; "
        "   border: 20px solid #FFFFFF;"
        "   border-radius: 10px;"
        "}"
        "QTableWidget::item:selected { "
        "   background-color: green; "
        "   color: white; "
        "}"
        "QHeaderView::section {"
        "   background-color: 1E383B;"
        "   color: white ;"
        "   padding: 6px;"
        "   border: none;"
        "   border-bottom: 2px solid #444444;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"


        "QComboBox {"
        "   border: 1px solid #333333;"
        "   border-radius: 4px;"
        "   padding: 5px 15px;"
        "   background-color: #0f172a;"
        "   color: #E0E0E0;"
        "   font-size: 13px;"
        "   min-width: 6em;"
        "}"
        "QComboBox:hover {"
        "   border: 1px solid #1e293b;"
        "   background-color: #1e293b;"
        "}"
        "QComboBox:on {"
        "   border-bottom-left-radius: 0px;"
        "   border-bottom-right-radius: 0px;"
        "}"
        "QComboBox::drop-down {"
        "   subcontrol-origin: padding;"
        "   subcontrol-position: top right;"
        "   width: 15px;"
        "   border-left: 1px solid #333333;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: #0f172a;"
        "   border: 1px solid #333333;"
        "   selection-background-color: #0078D4;"
        "   selection-color: white;"
        "   outline: none;"
        "}"
        "QComboBox QAbstractItemView::item {"
        "   padding: 8px;"
        "   color: #E0E0E0;"
        "}"
    );
}
