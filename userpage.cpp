#include "userpage.h"
#include "./ui_userpage.h"

#include "data_base.h"
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


UserPageMain::UserPageMain(int initialData, QWidget* parent)
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
        QTableWidgetItem* fromItem = new QTableWidgetItem(QString::fromStdString(flights[row].from));
        QTableWidgetItem* toItem = new QTableWidgetItem(QString::fromStdString(flights[row].to));

        auto formattedTime = std::format("{:%F %R}", flights[row].departure_date);
        QTableWidgetItem* leaveItem = new QTableWidgetItem(QString::fromStdString(formattedTime));

        ui->MaintableWidget->setItem(row, 0, fromItem);
        ui->MaintableWidget->setItem(row, 1, toItem);
        ui->MaintableWidget->setItem(row, 2, leaveItem);
    }

    //Booking no. , Departure, Arrival, Takeoff time, total passengers, price
    ui->BookedTableWidget->setColumnCount(6);
    QStringList BookingcolNames = { "Booking Number", "Departure Airport", "Arrival Airport", "Take-off Time", "Total Passengers"," " };
    ui->BookedTableWidget->setHorizontalHeaderLabels(BookingcolNames);

    ui->BookedTableWidget->setRowCount(0); // Prevents stacking duplicate data

    for (int i = 0; i < users[m_uID].tickets.size; i++) {
        // Creating references so we don't repeat long chains of array lookups
        auto& currentTicket = users[m_uID].tickets[i];
        auto& currentFlight = flights[currentTicket.flight_ID];

        int row = ui->BookedTableWidget->rowCount();
        ui->BookedTableWidget->insertRow(row);

        // Format the time safely
        auto formattedTime = std::format("{:%F %R}", currentFlight.departure_date);

        // Row Number (1, 2, 3...)
        ui->BookedTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));

        // From / To
        ui->BookedTableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(currentFlight.from)));
        ui->BookedTableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(currentFlight.to)));

        // Departure Date
        ui->BookedTableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(formattedTime)));

        // Passenger / Seat Count (Adjust .size to .size() if seats is a std::vector!)
        int passengerNo = currentTicket.seats.size;
        ui->BookedTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(passengerNo)));

        QStringList options = { "Edit Booking", "Delete Booking" };

        QComboBox* comboBox = new QComboBox(ui->BookedTableWidget);

        comboBox->addItems(options);

        comboBox->setPlaceholderText("Options");

        comboBox->setCurrentIndex(-1);

        QObject::connect(comboBox, &QComboBox::activated, [this, comboBox](int index) {

            int currentRealRow = ui->BookedTableWidget->indexAt(comboBox->pos()).row();

            switch (index) {
            case 0:
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(ui->BookedTableWidget, "Edit Booking...", "Are you sure you want to edit this booking?",
                    QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes) {
                    EditBooking();
                    comboBox->setCurrentIndex(-1);
                }
                else {
                    comboBox->setCurrentIndex(-1);

                }
                break;
            case 1:
                QMessageBox::StandardButton reply2;
                reply = QMessageBox::question(ui->BookedTableWidget, "Delete Booking...", "Are you sure you want to delete this booking?",
                    QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes) {
                    DeleteBooking();
                    comboBox->setCurrentIndex(-1);
                }
                else {
                    comboBox->setCurrentIndex(-1);

                }
                break;
            default:
                break;
            }
            });

        // 4. Inject the combo box directly into the specified grid coordinates
        ui->BookedTableWidget->setCellWidget(row, 5, comboBox);


    }

    //Adds the picture in the booking page
    QPixmap pixmap1("img/Test_Image.png");
    ui->DestinationImaGE->setPixmap(pixmap1);
    ui->DestinationImaGE->setScaledContents(true);



    //Adds the flight selection choices to a group
    QButtonGroup* SelectedFlight = new QButtonGroup(this);

    SelectedFlight->setExclusive(true);

    SelectedFlight->addButton(ui->FirstAvFlight);
    SelectedFlight->addButton(ui->SecondAvFlight);
   

    ui->FirstAvFlight->setCheckable(true);
    ui->SecondAvFlight->setCheckable(true);
    
    ui->FirstAvFlight->setChecked(false);
    ui->SecondAvFlight->setChecked(false);
    


    //Start of Seat Selection set-up
    ui->SeatSelection->setColumnWidth(15, 15);

    QString seatClass[3] = { "Economy","Business", "First Class" };
    double price[3] = { 150.50, 200.00, 350.99 };


    //Sets the size and headers of the columna
    ui->SeatSelection->setRowCount(15);
    ui->SeatSelection->setColumnCount(9);

    QStringList RowNums;
    QStringList ColNames = { "A", "B", "Aisle", "C", "D", "E", "Aisle", "F", "G" };
    ui->SeatSelection->setHorizontalHeaderLabels(ColNames);

    //To add the seat buttons to the table
    for (int row = 0; row < 15; ++row) {

        RowNums << QString::number(row + 1);
        ui->SeatSelection->setVerticalHeaderLabels(RowNums);


        for (int col = 0; col < 9; ++col) {

            QPushButton* btn = new QPushButton();

            //Does not add a button to specific columns (Aisle Columns)
            if (col == 2 || col == 6) {
                QTableWidgetItem* item = new QTableWidgetItem(RowNums[row]);
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
            if (row < 2) {
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
            else if (row < 6) {
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
            else {
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

            //connect(btn, &QPushButton::clicked, this, &UserPageMain::handleSeatClick);
            connect(btn, &QPushButton::clicked, this, [=, this]() {
                this->on_SeatSelection_cellClicked(row, col);
                });

            connect(ui->AdultComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &UserPageMain::updateSeatSelectionUI);

            connect(ui->ChildrenComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &UserPageMain::updateSeatSelectionUI);

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


//Resets Payment screen
void PaymentScreen::resetContent() {
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
void UserPageMain::BookingInitial() {
    //Airport Choices Reset
    ui->ArrivalAirport->blockSignals(true);
    ui->DepartureAirport->blockSignals(true);

    ui->ArrivalAirport->setCurrentIndex(-1);
    ui->DepartureAirport->setCurrentIndex(-1);

    ui->ArrivalAirport->blockSignals(false);
    ui->DepartureAirport->blockSignals(false);

    //Flight Choice Reset
    ui->AvailableFlights->setVisible(false);
    ui->FirstAvFlight->setChecked(false);
    ui->SecondAvFlight->setChecked(false);
  

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
            QPushButton* btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));

            if (btn && btn->isChecked()) {
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
    }
    else {
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
        this->~UserPageMain();
    }
    else {

    }
}


//To show the Booking History
void UserPageMain::on_pushButton_clicked()
{
    if (ui->pushButton->isChecked()) {
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


    for (int i = 0; i < flights.size; i++) {
        ui->DepartureAirport->addItem(QString::fromStdString(flights[i].from));
        ui->ArrivalAirport->addItem(QString::fromStdString(flights[i].to));

    };

}


//Logic to show the widgets in the booking page & to show the matching flight routes
void UserPageMain::on_ArrivalAirport_currentIndexChanged(int index)
{
    if (index == -1)
        return;

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

    Vector<int> flight_ids = search_flights(SelLeaveAirport.toStdString(), SelArriveAirport.toStdString());

    //Check if the vector is empty
    if (0 >= flight_ids.size) {
        QMessageBox* msg = new QMessageBox();
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

    
}


void UserPageMain::on_ChildrenComboBox_currentIndexChanged(int index)
{
    ui->SeatSelection->setVisible(true);

}


//Seat Handling(Max selected seats AND to show the payment button)
//Handles the price
void UserPageMain::on_SeatSelection_cellClicked(int Row, int column)
{
    QPushButton* clickedBtn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(Row, column));
    if (!clickedBtn) return;

    // Run the centralized UI and vector update system
    updateSeatSelectionUI();
}

void UserPageMain::updateSeatSelectionUI()
{
    // 1. Calculate current maximum allowed seat selections
    int maxSelection = 1 + ui->AdultComboBox->currentIndex() + ui->ChildrenComboBox->currentIndex();

    // 2. Count total active selections across the grid
    int currentSelected = 0;
    for (int r = 0; r < ui->SeatSelection->rowCount(); ++r) {
        for (int c = 0; c < ui->SeatSelection->columnCount(); ++c) {
            QPushButton* btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));
            if (btn && btn->isChecked()) {
                currentSelected++;
            }
        }
    }

    // 3. Handle over-limit selections (e.g., when combo box values decrease)
    if (currentSelected > maxSelection) {
        for (int r = ui->SeatSelection->rowCount() - 1; r >= 0; --r) {
            for (int c = ui->SeatSelection->columnCount() - 1; c >= 0; --c) {
                if (currentSelected <= maxSelection) break;

                QPushButton* btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));
                if (btn && btn->isChecked()) {
                    btn->setChecked(false);
                    currentSelected--;
                }
            }
        }
    }

    // 4. Dynamic Interface Locking Matrix
    bool reachedLimit = (currentSelected >= maxSelection);
    for (int r = 0; r < ui->SeatSelection->rowCount(); ++r) {
        for (int c = 0; c < ui->SeatSelection->columnCount(); ++c) {
            QPushButton* btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));
            if (btn) {
                if (!btn->isChecked()) {
                    btn->setEnabled(!reachedLimit);
                }
                else {
                    btn->setEnabled(true);
                }
            }
        }
    }

    // 5. Clean, Safe Vector Rebuild Phase
    SelectedSeats.size = 0;
    int calculatedTotal = 0;

    for (int r = 0; r < ui->SeatSelection->rowCount(); ++r) {
        for (int c = 0; c < ui->SeatSelection->columnCount(); ++c) {
            QPushButton* btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));

            if (btn && btn->isChecked()) {
                Seat seat;
                seat.row = r;
                seat.column = c;

                if (r < 2) {
                    seat.tier = 1;
                    calculatedTotal += flights[IDFlight].high_price;
                }
                else if (r < 6) {
                    seat.tier = 2;
                    calculatedTotal += flights[IDFlight].mid_price;
                }
                else {
                    seat.tier = 3;
                    calculatedTotal += flights[IDFlight].low_price;
                }
                SelectedSeats.push_back(seat);
            }
        }
    }

    // 6. Apply structural variables back to class variables and UI labels
    TotalPrice = calculatedTotal;
    ui->PriceDisplay->setText("$" + QString::number(TotalPrice));

    bool hasSelections = (currentSelected > 0);
    ui->PriceTotal->setVisible(hasSelections);
    ui->PriceDisplay->setVisible(hasSelections);
    ui->PaymentButton->setVisible(hasSelections);
}


//To show payment page & Create a ticket
void UserPageMain::on_PaymentButton_clicked()
{
    int adults = ui->AdultComboBox->currentIndex(), children = ui->ChildrenComboBox->currentIndex();

    CREATE_TICKET newTicket = create_ticket(users[m_uID].user_ID, IDFlight, adults, children, SelectedSeats);

    if (newTicket == BROKE_RULES) {
        QMessageBox* msg = new QMessageBox();
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

    if (newTicket == TICKET_SUCCESS) {
        PaymentScreen* Pay = new PaymentScreen(this);
        Pay->resetContent();
        Pay->setModal(true);
        Pay->setWindowTitle("Payment Screen");
        Pay->showAnimated();
    }

    else if (newTicket == CANNOT_ADD_TICKETS) {
        QMessageBox* msg = new QMessageBox();
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
        "QPushButton:unchecked {"
        "   background-color: transparent;"
        "   color: #94a3b8;"
        "   border: 1px solid #334155;"
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


void UserPageMain::EditBooking() {

    TicketID = ui->BookedTableWidget->currentRow();

    // 1. Ensure a valid row is selected
    if (TicketID < 0 || TicketID >= users[m_uID].tickets.size) {
        return;
    }

    // 2. Get the specific ticket being edited
    Ticket& targetTicket = users[m_uID].tickets[TicketID];

    // 3. Find the matching flight details from the global vector
    Flight targetFlight;
    bool flightFound = false;
    for (int i = 0; i < flights.size; ++i) {
        if (flights[i].flight_ID == targetTicket.flight_ID) {
            targetFlight = flights[i];
            flightFound = true;
            break;
        }
    }

    // 4. Switch to the booking page
    ui->stackedWidget->setCurrentIndex(1);

    // 5. Populate text fields (convert std::string to QString)
    if (flightFound) {
        ui->DepartureAirport->setCurrentText(QString::fromStdString(targetFlight.from));

        ui->ArrivalAirport->setCurrentText(QString::fromStdString(targetFlight.to));
    }


    // If you have a departure field:


// 6. Populate quantities back to comboboxes
    ui->AdultComboBox->setCurrentIndex(targetTicket.adults - 1);
    ui->ChildrenComboBox->setCurrentIndex(targetTicket.children);

    // 7. Reset and re-check the saved seats in the table
    // First, clear all current seat checkmarks
    for (int r = 0; r < ui->SeatSelection->rowCount(); ++r) {
        for (int c = 0; c < ui->SeatSelection->columnCount(); ++c) {
            QPushButton* btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));
            if (btn) btn->setChecked(false);
        }
    }

    // Second, toggle the buttons for the previously saved seats
    for (int i = 0; i < targetTicket.seats.size; ++i) {
        int r = targetTicket.seats[i].row;
        int c = targetTicket.seats[i].column;
        QPushButton* btn = qobject_cast<QPushButton*>(ui->SeatSelection->cellWidget(r, c));
        if (btn) btn->setChecked(true);
    }
}

void UserPageMain::DeleteBooking() {

    int indexToDelete = ui->BookedTableWidget->currentRow();
    auto& userTickets = users[m_uID].tickets;
    userTickets.erase(indexToDelete);
    ui->BookedTableWidget->removeRow(indexToDelete);
}
