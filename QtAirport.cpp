#include "QtAirport.h"
#include "login.h"
#include<iostream>

// MAIN WINDOW 
QtAirport::QtAirport(int airportID ,QWidget* parent) : QMainWindow(parent), airport_ID(airportID) {
    this->resize(1200, 800);
    this->setWindowTitle("Airport Management");

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Sidebar
    sidebar = new QFrame();
    sidebar->setFixedWidth(250);
    sidebar->setObjectName("sidebar");
    sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(20, 40, 20, 20);

    QLabel* logo = new QLabel("✈ Teer Enta");
    logo->setStyleSheet("font-size: 24px; color: #6366f1; font-weight: bold; margin-bottom: 20px;");
    sidebarLayout->addWidget(logo);

    btnDashboard = new QPushButton(" Dashboard");
    btnAirplanes = new QPushButton(" Airplanes");
    btnSchedule = new QPushButton(" Schedule Flight");
    btnRequests = new QPushButton(" Flight Requests");


    sidebarLayout->addWidget(btnDashboard);
    sidebarLayout->addWidget(btnAirplanes);
    sidebarLayout->addWidget(btnSchedule);
    sidebarLayout->addWidget(btnRequests);
    sidebarLayout->addStretch();

    // log out button 
    btnLogout = new QPushButton(" Log Out ");
    btnLogout->setObjectName("btnLogout");
    sidebarLayout->addWidget(btnLogout);

    //logout function 
    connect(btnLogout, &QPushButton::clicked, this, [=]() {
        Login* loginpage = new Login();
        loginpage->show();
        this->~QtAirport();
        });

    // Pages
    contentStack = new QStackedWidget();
    contentStack->addWidget(DashboardPage());      
    contentStack->addWidget(AirplanesPage());      
    contentStack->addWidget(SchedulePage());       
    contentStack->addWidget(FlightRequestsPage()); 

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentStack);

    // Connections
    connect(btnDashboard, &QPushButton::clicked, [this] { contentStack->setCurrentIndex(0); refresh(); });
    connect(btnAirplanes, &QPushButton::clicked, [this] { contentStack->setCurrentIndex(1); populateAirplanesTable(); });
    connect(btnSchedule, &QPushButton::clicked, [this] { contentStack->setCurrentIndex(2); populateScheduleTable(); });
    connect(btnRequests, &QPushButton::clicked, [this] { contentStack->setCurrentIndex(3); populateRequestsTable(); });
    //connect(btnLogout, &QPushButton::clicked, this, &QtAirport::handleLogout);

    applyStyles();
    refresh();   // initial load
}

//  refresh

void QtAirport::refresh() {
    Airport& ap = airports[airport_ID];

    lblTotalLanes->setText(QString::number(ap.capacity));
    lblAvailablePlanes->setText(QString::number(ap.free_planes_IDs.size));
    lblScheduledFlights->setText(QString::number(ap.future_flights_IDs.size));
    lblPendingRequests->setText(QString::number(ap.pending_requests.size));

    populateAirplanesTable();
    populateScheduleTable();
    populateRequestsTable();
   
}

//  DASHBOARD 
QWidget* QtAirport::DashboardPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 40, 40, 40);

    layout->addWidget(new QLabel("Dashboard Overview"));

    QLabel* sub = new QLabel(
                QString("Airport: %1  ·  Code: %2")
                .arg(QString::fromStdString(airports[airport_ID].city.location))
                .arg(QString::fromStdString(airports[airport_ID].city.code))
            );
            sub->setStyleSheet("color:#94a3b8;font-size:13px;");
            layout->addWidget(sub);

    // Grid for the 4 Stat Cards
    QGridLayout* grid = new QGridLayout();
        grid->setSpacing(20);
    
        Airport& ap = airports[airport_ID];

       
    
    grid->addWidget(createStatCard("Total Lanes",
        QString::number(ap.capacity), "#6366f1", lblTotalLanes), 0, 0);
    
    grid->addWidget(createStatCard("Available Planes",
        QString::number(ap.free_planes_IDs.size), "#22c55e", lblAvailablePlanes), 0, 1);
    
    grid->addWidget(createStatCard("Scheduled Flights",
        QString::number(ap.future_flights_IDs.size), "#ef4444", lblScheduledFlights), 1, 0);
    
    grid->addWidget(createStatCard("Pending Requests",
        QString::number(ap.pending_requests.size), "#eab308", lblPendingRequests), 1, 1);
    
    layout->addLayout(grid); 
    layout->addStretch();
    return page;
    
}


//  AIRPLANES 
QWidget* QtAirport::AirplanesPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 40, 40, 40);

    QHBoxLayout* h = new QHBoxLayout();
    h->addWidget(new QLabel("Airplane Flight"));
    h->addStretch();
    QPushButton* buy = new QPushButton("+ Buy Airplane");
    buy->setStyleSheet("background-color: #6366f1; color: white; padding: 10px; border-radius: 5px;");
    h->addWidget(buy);
    layout->addLayout(h);

    connect(buy, &QPushButton::clicked, this, &QtAirport::openBuyAirplane);

    airplanesTable = new QTableWidget(0, 6);
    airplanesTable->setHorizontalHeaderLabels({ "ID","MANUFACTURER","MODEL", "TOTAL SEATS","NUMBER OF FLIGHTS", "STATUS"});
    airplanesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    airplanesTable->verticalHeader()->setVisible(false);
    airplanesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    airplanesTable->setSelectionBehavior(QAbstractItemView::SelectRows);// to select the whole row
    layout->addWidget(airplanesTable);
    return page;
}

void QtAirport::populateAirplanesTable() {
    if (!airplanesTable) return;
    Airport& ap = airports[airport_ID];
    airplanesTable->setRowCount(0);

    for (int i = 0; i < ap.free_planes_IDs.size; i++) {
        int pid = ap.free_planes_IDs[i];
        Airplane& pl = planes[pid];

        int row = airplanesTable->rowCount();
        airplanesTable->insertRow(row);
        airplanesTable->setItem(row, 0, new QTableWidgetItem(QString::number(pl.plane_ID)));
        airplanesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(pl.manufacturer)));
        airplanesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(pl.model)));
        airplanesTable->setItem(row, 3, new QTableWidgetItem(QString::number(pl.number_of_seats)));
        airplanesTable->setItem(row, 4, new QTableWidgetItem(QString::number(pl.number_of_flights)));
        QTableWidgetItem* status = new QTableWidgetItem("Free");
        status->setForeground(QColor("#22c55e"));
        airplanesTable->setItem(row, 5, status);

    }

    for (int i = 0; i < ap.pending_planes_IDs.size; i++) {
        int pid = ap.pending_planes_IDs[i];
        Airplane& pl = planes[pid];

        int row = airplanesTable->rowCount();
        airplanesTable->insertRow(row);
        airplanesTable->setItem(row, 0, new QTableWidgetItem(QString::number(pl.plane_ID)));
        airplanesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(pl.manufacturer)));
        airplanesTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(pl.model)));
        airplanesTable->setItem(row, 3, new QTableWidgetItem(QString::number(pl.number_of_seats)));
        airplanesTable->setItem(row, 4, new QTableWidgetItem(QString::number(pl.number_of_flights)));
        QTableWidgetItem* status = new QTableWidgetItem("Pending");
        status->setForeground(QColor("#eab308"));
        airplanesTable->setItem(row, 5, status);

    }

    
}

//  BUY AIRPLANE DIALOG 

class BuyAirplane : public QDialog {
public:
    BuyAirplane(int airportID, QWidget* parent = nullptr) : QDialog(parent), ap_ID(airportID) {
        setWindowTitle("Buy New Airplane"); 
        setFixedSize(350, 200);
        setStyleSheet("background-color: #0f172a; color: white;");

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(30, 30, 30, 30);
        layout->setSpacing(14);

        QString style = "background-color: #1e293b; border: 1px solid #334155; padding: 8px; border-radius: 5px; color: white;";

        // Airplane Specs 

        layout->addWidget(new QLabel("Choose Model"));
         model = new QComboBox();

        for (int i = 0; i < models.size; i++) {
            string model_name = models[i].manufacturer + " - " + models[i].model;
            model->addItem(QString::fromStdString(model_name), i);
        }
        model->setStyleSheet(style);
        layout->addWidget(model);


        // Buttons
        QHBoxLayout* btns = new QHBoxLayout();
        c = new QPushButton("Cancel");
        b = new QPushButton("Buy"); 
        b->setStyleSheet("background-color: #22c55e; font-weight: bold; padding: 10px; border-radius: 5px;");

        btns->addWidget(c);
        btns->addWidget(b);
        layout->addLayout(btns);

        connect(c, &QPushButton::clicked, this, &QDialog::reject); // reject to closes the dialog and returns cancel
        connect(b, &QPushButton::clicked, this, &BuyAirplane::onBuy);

        if (models.size == 0) {
            buyBtn->setEnabled(false);
        }
    }
    private slots:

        void onBuy() {
            int model_index = model->currentData().toInt();

            bool success = buy_new_plane(ap_ID, model_index);
            if (!success) {
                QMessageBox::warning(this, "Error", "Failed to buy airplane!");
            }
            else {
                QMessageBox::information(this, "Success", "Added new airplane to your fleet");
                accept(); // Closes the dialog with 'Accepted' result
            }
        }


private:
    int ap_ID;
    QComboBox* model;
    QPushButton* buyBtn;
    QPushButton* b;
    QPushButton* c;


};

void QtAirport::openBuyAirplane() {
    if (models.size == 0) {
        QMessageBox::information(this, "No Models",
            "No airplane models are available on the market yet.\n"
            "Ask an admin to add models via the Admin page.");
        return;
    }
    BuyAirplane d(airport_ID, this);
    if (d.exec() == QDialog::Accepted)
        refresh();
}




//  SCHEDULE
QWidget* QtAirport::SchedulePage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(16);

    QPushButton* btnNew = new QPushButton("+ Schedule New Flight");
    btnNew->setStyleSheet("background-color: #6366f1; color: white; padding: 5px; border-radius: 8px;");
    layout->addWidget(btnNew);



    scheduleTable = new QTableWidget(0, 7);
    scheduleTable->setHorizontalHeaderLabels({ "FLIGHT #", "AIRPLANE", "DESTINATION", "DEPARTURE", "ARRIVAL", "PRICE", "STATUS" });
    scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scheduleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(scheduleTable);

    connect(btnNew, &QPushButton::clicked, this, &QtAirport::openScheduleDialog);
    return page;
}

void QtAirport::populateScheduleTable() {
    if (!scheduleTable) return;
    scheduleTable->setRowCount(0);
    Airport& ap = airports[airport_ID];

    for (int i = 0; i < ap.future_flights_IDs.size; i++) {
        int fid = ap.future_flights_IDs[i];
        Flight& fl = flights[fid];
        string plane = planes[fl.plane_ID].manufacturer + " - " + planes[fl.plane_ID].model;


        int row = scheduleTable->rowCount();
        scheduleTable->insertRow(row);
        scheduleTable->setItem(row, 0, new QTableWidgetItem(QString::number(fl.flight_ID)));
        scheduleTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(plane)));
        scheduleTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(fl.to)));
        auto time = std::format("{:%F %R }", fl.departure_date);
        auto time2 = std::format("{:%F %R }", fl.arrival_date);
        scheduleTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(time)));
        scheduleTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(time2)));
        scheduleTable->setItem(row, 5, new QTableWidgetItem(
        QString("$%1 / $%2 / $%3")
        .arg(fl.low_price).arg(fl.mid_price).arg(fl.high_price)));
       
        QTableWidgetItem* status = new QTableWidgetItem("Scheduled");
        status->setForeground(QColor("#ef4444"));
        scheduleTable->setItem(row, 6, status);

    }


}

class ScheduleDialog : public QDialog {
public:
    ScheduleDialog(int airportID, QWidget* parent = nullptr) : QDialog(parent), ap_ID(airportID) {
        setWindowTitle("Schedule New Flight");
        setFixedSize(450, 600);
        setStyleSheet("background-color: #0f172a; color: white;");
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(30, 30, 30, 30);
        layout->setSpacing(15);

        QString style = "background-color: #1e293b; border: 1px solid #334155; padding: 8px; border-radius: 5px; color: white;";

        
        layout->addWidget(new QLabel("Airplane:"));
         air = new QComboBox(this);
         air->setStyleSheet(style); 
         for (int i = 0; i < airports[ap_ID].free_planes_IDs.size; i++) {
             Airplane& pl = planes[airports[ap_ID].free_planes_IDs[i]];
             string name = pl.manufacturer + " - " + pl.model;
             air->addItem(QString::fromStdString(name), pl.plane_ID);
         }
         layout->addWidget(air);

        layout->addWidget(new QLabel("Destination:"));
         dest= new QComboBox(this);
         dest->setStyleSheet(style); 
         for (int i = 0; i < airports.size; i++) {
             if (i == airportID) continue;
             Airport& ap = airports[i];
             string name = ap.city.location + " - " + ap.city.code;
             dest->addItem(QString::fromStdString(name), ap.airport_ID);
         }
        layout->addWidget(dest);

        connect(air, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ScheduleDialog::combo_changed);

        connect(dest, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ScheduleDialog::combo_changed);

        layout->addWidget(new QLabel("Departure Date:"));
        dpt = new QDateTimeEdit(QDateTime::currentDateTime());
        dpt->setMinimumDateTime(QDateTime::currentDateTime().addSecs(1800));
        dpt->setCalendarPopup(true);
        dpt->setMaximumDate(QDate(7999, 12, 31));
        dpt->setDisplayFormat("yyyy-MM-dd HH:mm");
        dpt->setStyleSheet(style);
        layout->addWidget(dpt);

        prc = new QLineEdit(this);
        layout->addWidget(new QLabel("Base Price (Economy Seat Price):"));
         prc->setStyleSheet(style);  layout->addWidget(prc); prc->setReadOnly(true); prc->setText("$000,000");

         dis = new QLineEdit(this);
         layout->addWidget(new QLabel("Distance:"));
         dis->setStyleSheet(style);  layout->addWidget(dis); dis->setReadOnly(true); dis->setText("00.00 Km");
         
        QHBoxLayout* btns = new QHBoxLayout();
        QPushButton* c = new QPushButton("Cancel");
        QPushButton* s = new QPushButton("Send Request");
        s->setStyleSheet("background-color: #6366f1; font-weight: bold; padding: 10px; border-radius: 5px;");
        btns->addWidget(c); btns->addWidget(s);
        layout->addLayout(btns);

        connect(c, &QPushButton::clicked, this, &QDialog::reject);
        connect(s, &QPushButton::clicked, this, &ScheduleDialog::onSchedule);

       
    }

private slots:
    void onSchedule() {
        
        int planeID = air->currentData().toInt();
        int destID = dest->currentData().toInt();
        QDateTime qtDateTime = dpt->dateTime();

        // Convert from qt date time to sys_time
        auto timePoint = system_clock::from_time_t(qtDateTime.toSecsSinceEpoch());
        sys_time<minutes> dep = time_point_cast<minutes>(timePoint);


        bool success = send_request(planeID, ap_ID, destID, dep);

        if (!success) {
            QMessageBox::warning(this, "Error", "Failed to send request!");
        }
        else {
            QMessageBox::information(this, "Success", "Request Sent! Now it's in Pending Requests");
            accept(); // Closes the dialog with 'Accepted' result
        }

        if (dpt->dateTime() < QDateTime::currentDateTime()) {
            QMessageBox::warning(this, "Invalid Date", "The selected time has already passed. Please pick a future time.");
            //  Reset it to current time automatically
            dpt->setDateTime(QDateTime::currentDateTime());
            return;
        }
    }

    void combo_changed() {
        if (!air || !dest) return;
        if (air->currentIndex() < 0 || dest->currentIndex() < 0) return;

        int planeID = air->currentData().toInt();
        int destID = dest->currentData().toInt();

        Airplane& pl = planes[planeID];
        Airport& from_ap = airports[ap_ID];
        Airport& to_ap = airports[destID];
        double distance = haversine(from_ap.city.latitude, from_ap.city.longitude, to_ap.city.latitude, to_ap.city.longitude);
        double base_price = base_ticket_price(planeID, distance);

        // Calculate your price logic here
        prc->setText(QString("$ %1").arg(base_price));
        dis->setText(QString("%1 Km").arg(distance));

       
    }



private:
    int ap_ID;
    
    QComboBox* air;
    QComboBox* dest;
    QDateTimeEdit* dpt;
    QLineEdit* prc;
    QLineEdit* dis;

};

void QtAirport::openScheduleDialog() {
        ScheduleDialog d(airport_ID, this);
        if (d.exec() == QDialog::Accepted)
            refresh();
    }
 

//  REQUESTS 
QWidget* QtAirport::FlightRequestsPage() {QWidget* page = new QWidget();
   
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 40, 40, 40);

    QHBoxLayout* h = new QHBoxLayout();
    QPushButton* ok = new QPushButton(" Accept");
    QPushButton* no = new QPushButton(" Deny");
    

    connect(requestsTable, &QTableWidget::itemSelectionChanged, [=]() {
        bool hasSelection = requestsTable->selectionModel()->hasSelection();
        ok->setEnabled(hasSelection);
        no->setEnabled(hasSelection);
        });
    
    ok->setStyleSheet("background-color: #00CF00; color: white; padding: 10px 20px; border-radius: 8px;");
    no->setStyleSheet("background-color: #ff0d2c; color: white; padding: 10px 20px; border-radius: 8px;");
    
    
    h->addStretch(); h->addWidget(ok); h->addWidget(no);
    layout->addLayout(h);

    requestsTable = new QTableWidget(0, 7);
    requestsTable->setHorizontalHeaderLabels({ "FLIGHT ID", "AIRPLANE", "From", "DEPARTURE", "ARRIVAL", "PRICE", "STATUS" });
    requestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    requestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    requestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    sentRequestsTable = new QTableWidget(0, 7);
    sentRequestsTable->setHorizontalHeaderLabels({ "FLIGHT ID", "AIRPLANE", "To", "DEPARTURE", "ARRIVAL", "PRICE", "STATUS" });
    sentRequestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sentRequestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sentRequestsTable->setAttribute(Qt::WA_TransparentForMouseEvents);

    layout->addWidget(requestsTable);
    layout->addWidget(sentRequestsTable);

    connect(ok, &QPushButton::clicked, this, &QtAirport::handleAcceptRequest);
    connect(no, &QPushButton::clicked, this, &QtAirport::handleDenyRequest);

    return page;
}

void QtAirport::handleAcceptRequest() {
    int currentRow = requestsTable->currentRow();

    // Check if  row is  selected
    if (currentRow < 0) {
        QMessageBox::warning(this, "Selection Required", "Please select a request to accept.");
        return;
    }

   
    int request_ID = requestsTable->item(currentRow, 0)->text().toInt();
    int from_airport_ID = airports[airport_ID].pending_requests[request_ID].from_airport_ID;
    
    if (accept_request(airport_ID, from_airport_ID, request_ID)) {
        QMessageBox::information(this, "Success", "Flight request accepted.");
        refresh(); 
    }
    else {
        QMessageBox::critical(this, "Error", "Could not accept request.");
    }
}

void QtAirport::handleDenyRequest() {
    int currentRow = requestsTable->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Selection Required", "Please select a request to deny.");
        return;
    }

    int request_ID = requestsTable->item(currentRow, 0)->text().toInt();
    int from_airport_ID = airports[airport_ID].pending_requests[request_ID].from_airport_ID;
    
    if (deny_request(airport_ID, from_airport_ID, request_ID)) {
        QMessageBox::information(this, "Denied", "Flight request has been denied.");
        refresh();
    }
    else {
        QMessageBox::critical(this, "Error", "Action failed.");
    }
}

void QtAirport::populateRequestsTable() {
    if (!requestsTable) return;
    requestsTable->setRowCount(0);
    Airport& ap = airports[airport_ID];

    for (int i = 0; i < ap.pending_requests.size; i++) {
        Flight& fl = ap.pending_requests[i];
        string plane = planes[fl.plane_ID].manufacturer + " - " + planes[fl.plane_ID].model;
        
        int row = requestsTable->rowCount();
        requestsTable->insertRow(row);
        requestsTable->setItem(row, 0, new QTableWidgetItem(QString::number(fl.flight_ID)));
        requestsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(plane)));
        requestsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(fl.from)));
        auto time = std::format("{:%F %R }", fl.departure_date);
        auto time2 = std::format("{:%F %R }", fl.arrival_date);
        requestsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(time)));
        requestsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(time2)));
        requestsTable->setItem(row, 5, new QTableWidgetItem(
                    QString("$%1 / $%2 / $%3")
                    .arg(fl.low_price).arg(fl.mid_price).arg(fl.high_price)));
        QTableWidgetItem* status = new QTableWidgetItem("Pending");
            status->setForeground(QColor("#eab308"));
            requestsTable->setItem(row, 6, status);
    }

    if (!sentRequestsTable) return;
    sentRequestsTable->setRowCount(0);

    for (int i = 0; i < ap.sent_pending_requests.size; i++) {
        Flight& fl = ap.sent_pending_requests[i];
        string plane = planes[fl.plane_ID].manufacturer + " - " + planes[fl.plane_ID].model;

        int row = sentRequestsTable->rowCount();
        sentRequestsTable->insertRow(row);
        sentRequestsTable->setItem(row, 0, new QTableWidgetItem(QString::number(fl.flight_ID)));
        sentRequestsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(plane)));
        sentRequestsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(fl.to)));
        auto time = std::format("{:%F %R }", fl.departure_date);
        auto time2 = std::format("{:%F %R }", fl.arrival_date);
        sentRequestsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(time)));
        sentRequestsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(time2)));
        sentRequestsTable->setItem(row, 5, new QTableWidgetItem(
            QString("$%1 / $%2 / $%3")
            .arg(fl.low_price).arg(fl.mid_price).arg(fl.high_price)));
        QTableWidgetItem* status = new QTableWidgetItem("Pending");
        status->setForeground(QColor("#eab308"));
        sentRequestsTable->setItem(row, 6, status);
    }

}


QWidget* QtAirport::createStatCard(const QString& title, const QString& value,
    const QString& color, QLabel*& outLabel) {
    QFrame* card = new QFrame();
    card->setFixedSize(300, 150);
    card->setObjectName("statCard");

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(6);

    QLabel* lblTitle = new QLabel(title.toUpper());
    lblTitle->setStyleSheet("color:#94a3b8;font-size:11px;font-weight:bold;letter-spacing:1px;");

    outLabel = new QLabel(value);
    outLabel->setStyleSheet(
        QString("color:%1;font-size:38px;font-weight:bold;").arg(color));

    layout->addWidget(lblTitle);
    layout->addWidget(outLabel);
    layout->addStretch();
    return card;
}

void QtAirport::applyStyles() {
    this->setStyleSheet(
        "QMainWindow { background-color: #020617; }"
        "#sidebar { background-color: #0f172a; border-right: 1px solid #1e293b; }"
        "QPushButton { background-color: transparent; color: #94a3b8; border: none; padding: 12px; text-align: left; font-size: 14px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #1e293b; color: white; }"
        "QTableWidget { background-color: #0f172a; color: white; border: none; gridline-color: transparent; }"
        "QHeaderView::section { background-color: #1e293b; color: #94a3b8; border: none; padding: 10px; }"
        "QLabel { color: white; }"
        "#statCard { "
        "   background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #1e293b, stop:1 #0f172a); "
        "   border: 1px solid #334155; "
        "   border-radius: 16px; "
        "} "
        "#statCard:hover { "
        "   border: 1px solid #6366f1; " // Subtle indigo glow on hover
        "   background-color: #1e293b; "
        "} "

        "#btnLogout { "
        "   background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; "
        "} "
        "#btnLogout:hover { "
        "   background-color: #ef4444; " // Red background 
        "   color: white; "
        "   border: 1px solid #ef4444; "
        "} "

    );
}
