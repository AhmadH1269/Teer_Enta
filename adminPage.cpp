#include "adminPage.h"
#include "login.h"
#include <QGridLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QScrollArea>
#include<QComboBox>
using namespace std;


AdminPage::AdminPage(QWidget* parent) : QWidget(parent) {



    // constructor body
    this->setWindowTitle("TEER ENTA ADMIN PAGE ");
    // this->resize(1100, 600);

    setupTables();
    setupAddPlaneForm();
    setupLayout();
    setupConnections();
    setupAddAirportForm();
}

void AdminPage::setupTables() {

    QString tableStyle =
        "QTableWidget {"
        "   background-color: #0b101e;"
        "   color: #5cd6ff;"
        "   gridline-color: #1c2237;"
        "   border: none;"
        "   selection-background-color: #1c2237;"
        "}"
        "QHeaderView::section {"
        "   background-color: #1c2237;"
        "   color: #5cd6ff;"
        "   padding: 10px;"
        "   border: none;"
        "   font-weight: bold;"
        "}";

   
    usersTable = new QTableWidget(users.size, 4);
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    usersTable->setHorizontalHeaderLabels({ "ID", "Name", "Role", "Email" });
    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // To make columns strech horizontally
    usersTable->setStyleSheet(tableStyle);

    usersTable = new QTableWidget(0, 4);
    usersTable->setHorizontalHeaderLabels({ "ID", "Name", "Role", "Email" });
    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    usersTable->setStyleSheet(tableStyle);
    loadUsersTable();

    
    adminsWidget = new QWidget();
    adminsWidget->setStyleSheet("background-color: #0b101e;");

    QVBoxLayout* adminsLayout = new QVBoxLayout(adminsWidget);
    adminsLayout->setContentsMargins(40, 40, 40, 40);
    adminsLayout->setSpacing(20);

    QLabel* adminsTitle = new QLabel("ADMINS LIST 👑");
    adminsTitle->setStyleSheet("font: bold 22px; color: #ffffff;");
    adminsLayout->addWidget(adminsTitle);

    adminsTable = new QTableWidget(0, 1);
    adminsTable->setHorizontalHeaderLabels({ "Name" });
    adminsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    adminsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    adminsTable->setStyleSheet(
        "QTableWidget { background-color: #0b101e; color: #5cd6ff;"
        "   gridline-color: #1c2237; border: none; }"
        "QHeaderView::section { background-color: #1c2237; color: #5cd6ff;"
        "   padding: 8px; border: none; font-weight: bold; }"
    );

    string initialAdmins[] = { "Ahmed Medhat","Ziad Badawy","Nour Mahmoud","Moataz Omran","Ziad Mohamed" };
    for (int i = 0; i < 5; i++) {
        int row = adminsTable->rowCount();
        adminsTable->insertRow(row);
        adminsTable->setItem(row, 0, new QTableWidgetItem(
            QString::fromStdString(initialAdmins[i])));
    }

    adminsLayout->addWidget(adminsTable);
    QPushButton* addAdminFromListBtn = new QPushButton("➕ Add Admin");
    addAdminFromListBtn->setFixedHeight(42);
    addAdminFromListBtn->setStyleSheet(
        "QPushButton { background-color: #5c67ff; color: white; font-weight: bold;"
        "   border-radius: 8px; font-size: 14px; border: none; padding: 8px; }"
        "QPushButton:hover { background-color: #4a55e0; }"
    );
    addAdminFromListBtn->setFixedWidth(200);
    connect(addAdminFromListBtn, &QPushButton::clicked, this, &AdminPage::showAddAdminDialog);
    adminsLayout->addWidget(addAdminFromListBtn);

    adminsLayout->addStretch();

    

    stackedWidget = new QStackedWidget();
    stackedWidget->setStyleSheet("background-color: #050811;");
    stackedWidget->addWidget(usersTable);// index 0
    QScrollArea* adminsScroll = new QScrollArea();
    adminsScroll->setWidget(adminsWidget);
    adminsScroll->setWidgetResizable(true);
    adminsScroll->setStyleSheet("border: none; background-color: #0b101e;");
    stackedWidget->addWidget(adminsScroll); // index 1

}
void AdminPage::setupAddPlaneForm() {

    addPlaneWidget = new QWidget();
    addPlaneWidget->setStyleSheet("background-color: #0b101e;");

    QVBoxLayout* mainLay = new QVBoxLayout(addPlaneWidget);
    mainLay->setContentsMargins(40, 40, 40, 40);
    mainLay->setSpacing(20);

    QLabel* title = new QLabel("ADD NEW PLANE ");
    title->setStyleSheet("font: bold 22px; color: #ffffff;");
    mainLay->addWidget(title);

    
    QFrame* card = new QFrame();
    card->setStyleSheet(
        "QFrame { background-color: #1c2237; border-radius: 12px;"
        "         border: 1px solid #33475B; }"
        "QLabel  { color: #a0a7b5; font-size: 13px; border: none; }"
        "QLineEdit {"
        "   background-color: #0b101e; border: 1px solid #33475B;"
        "   border-radius: 6px; padding: 8px; color: #ffffff; font-size: 13px;"
        "}"
        "QLineEdit:focus { border: 1px solid #5cd6ff; }"
    );
    card->setMaximumWidth(520);

    QFormLayout* form = new QFormLayout(card);
    form->setContentsMargins(30, 30, 30, 30);
    form->setSpacing(18);
    form->setLabelAlignment(Qt::AlignRight);

    manufacturerCombo = new QComboBox();
    manufacturerCombo->setFixedHeight(38);
    manufacturerCombo->setStyleSheet(
        "QComboBox { background-color: #0b101e; border: 1px solid #33475B;"
        "   border-radius: 6px; padding: 8px; color: #ffffff; font-size: 13px; }"
        "QComboBox QAbstractItemView { background-color: #1c2237; color: white; }"
    );
    manufacturerCombo->addItems({ "Airbus", "Boeing", "Embraer" });

    modelCombo = new QComboBox();
    modelCombo->setFixedHeight(38);
    modelCombo->setStyleSheet(
        "QComboBox { background-color: #0b101e; border: 1px solid #33475B;"
        "   border-radius: 6px; padding: 8px; color: #ffffff; font-size: 13px; }"
        "QComboBox QAbstractItemView { background-color: #1c2237; color: white; }"
    );

    planeSeatsEdit = new QLineEdit();
    planeSeatsEdit->setPlaceholderText("e.g. 180");
    planeSeatsEdit->setFixedHeight(38);

    planeSpeedEdit = new QLineEdit();
    planeSpeedEdit->setPlaceholderText("e.g. 850");
    planeSpeedEdit->setFixedHeight(38);

    planeFuelEdit = new QLineEdit();
    planeFuelEdit->setPlaceholderText("e.g. 12.5");
    planeFuelEdit->setFixedHeight(38);

    form->addRow("Manufacturer :", manufacturerCombo);
    form->addRow("Model :", modelCombo);
    form->addRow("Seats :", planeSeatsEdit);
    form->addRow("Speed (km/h) :", planeSpeedEdit);
    form->addRow("Fuel Price/km :", planeFuelEdit);

    updatePlaneModels();

    connect(manufacturerCombo, &QComboBox::currentTextChanged,
        this, &AdminPage::updatePlaneModels);



    QPushButton* addBtn = new QPushButton("＋  Add Plane to System");
    addBtn->setFixedHeight(42);
    addBtn->setStyleSheet(
        "QPushButton { background-color: #5c67ff; color: white; font-weight: bold;"
        "   border-radius: 8px; font-size: 14px; border: none; }"
        "QPushButton:hover { background-color: #4a55e0; }"
    );

    connect(addBtn, &QPushButton::clicked, this, &AdminPage::addPlaneToSystem);
    form->addRow(addBtn);

    mainLay->addWidget(card);
    mainLay->addSpacing(20);



    QLabel* addedTitle = new QLabel("Planes added to system");
    addedTitle->setStyleSheet("font: bold 16px; color: #5cd6ff;");
    mainLay->addWidget(addedTitle);


    planesTable = new QTableWidget(0, 5);
    planesTable->setHorizontalHeaderLabels({ "#", "Manufacturer", "Model", "Seats", "Speed" }); planesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    planesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    planesTable->setMinimumHeight(120);
    planesTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    planesTable->setStyleSheet(
        "QTableWidget { background-color: #0b101e; color: #5cd6ff;"
        "   gridline-color: #1c2237; border: none; }"
        "QHeaderView::section { background-color: #1c2237; color: #5cd6ff;"
        "   padding: 8px; border: none; font-weight: bold; }"
    );
    mainLay->addWidget(planesTable);
    mainLay->addSpacing(10);

    QScrollArea* planeScroll = new QScrollArea();
    planeScroll->setWidget(addPlaneWidget);
    planeScroll->setWidgetResizable(true);
    planeScroll->setStyleSheet("border: none; background-color: #0b101e;");
    stackedWidget->addWidget(planeScroll);


    setupEnterNavigation({ planeSeatsEdit, planeSpeedEdit, planeFuelEdit });//----> enter pressed fn

}

void AdminPage::showAddUserDialog() {

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Add New User");
    dialog->setFixedSize(420, 340);
    dialog->setStyleSheet(
        "QDialog { background-color: #0b101e; }"
        "QLabel  { color: #a0a7b5; font-size: 13px; }"
        "QLineEdit {"
        "   background-color: #1c2237; border: 1px solid #33475B;"
        "   border-radius: 6px; padding: 8px; color: #ffffff; font-size: 13px;"
        "}"
        "QLineEdit:focus { border: 1px solid #5cd6ff; }"
    );

    QVBoxLayout* dlgLayout = new QVBoxLayout(dialog);
    dlgLayout->setContentsMargins(30, 25, 30, 25);
    dlgLayout->setSpacing(16);

    QLabel* dlgTitle = new QLabel("👤  Add New User");
    dlgTitle->setStyleSheet("font: bold 18px; color: #ffffff;");
    dlgLayout->addWidget(dlgTitle);


    QFormLayout* form = new QFormLayout();
    form->setSpacing(12);
    form->setLabelAlignment(Qt::AlignRight);


    

    QLineEdit* idEdit = new QLineEdit(); idEdit->setPlaceholderText("e.g. 20251700310");
    QLineEdit* nameEdit = new QLineEdit(); nameEdit->setPlaceholderText("Full Name");
    QLineEdit* roleEdit = new QLineEdit(); roleEdit->setPlaceholderText("e.g. Passenger");
    QLineEdit* emailEdit = new QLineEdit(); emailEdit->setPlaceholderText("e.g. user@mail.com");

    for (auto* le : { idEdit,nameEdit,roleEdit,emailEdit })
        le->setFixedHeight(36);

    form->addRow("ID :", idEdit);
    form->addRow("Name :", nameEdit);
    form->addRow("Role :", roleEdit);
    form->addRow("Email :", emailEdit);
    dlgLayout->addLayout(form);



    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QPushButton* confirmBtn = new QPushButton("＋ Add User");

    cancelBtn->setFixedHeight(38);
    confirmBtn->setFixedHeight(38);


    cancelBtn->setStyleSheet(
        "QPushButton { background-color: #1c2237; color: #a0a7b5; border-radius: 8px;"
        "   border: 1px solid #33475B; font-size: 13px; }"
        "QPushButton:hover { background-color: #2a3350; color: white; }"
    );
    confirmBtn->setStyleSheet(
        "QPushButton { background-color: #5c67ff; color: white; font-weight: bold;"
        "   border-radius: 8px; font-size: 13px; border: none; }"
        "QPushButton:hover { background-color: #4a55e0; }"
    );

    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(confirmBtn);
    dlgLayout->addLayout(btnRow);


    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::reject);

    connect(confirmBtn, &QPushButton::clicked, dialog, [=]() {

       


        if (idEdit->text().trimmed().isEmpty() ||
            nameEdit->text().trimmed().isEmpty() ||
            roleEdit->text().trimmed().isEmpty() ||
            emailEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(dialog, "Missing Info", "Please fill in all fields.");
            return;
        }

        int insertRow = 0;
        for (int i = 0; i < usersTable->rowCount(); i++) {
            QTableWidgetItem* it = usersTable->item(i, 0);
            if (it && !it->text().isEmpty())
                insertRow = i + 1;
            else
                break;
        }

        usersTable->setItem(insertRow, 0,
            new QTableWidgetItem(idEdit->text().trimmed()));
        usersTable->setItem(insertRow, 1,
            new QTableWidgetItem(nameEdit->text().trimmed()));
        usersTable->setItem(insertRow, 2,
            new QTableWidgetItem(roleEdit->text().trimmed()));
        usersTable->setItem(insertRow, 3,
            new QTableWidgetItem(emailEdit->text().trimmed()));



        usersTable->scrollToItem(usersTable->item(insertRow, 0));

        setupEnterNavigation({ idEdit, nameEdit, roleEdit, emailEdit });//------> enter pressed

        QMessageBox::information(dialog, "Success",
            "✅  User "" + nameEdit->text().trimmed() + "" added successfully!");
        dialog->accept();
        });

    dialog->exec();

}




void AdminPage::setupLayout() {
    this->setStyleSheet("QLabel { color: #a0a7b5; font-family: 'Segoe UI'; background: transparent; }");

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor("#020617"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);



    QFrame* sidebarFrame = new QFrame();
    sidebarFrame->setFixedWidth(250);
    sidebarFrame->setStyleSheet("background-color: #0b101e; border: none;");

    QVBoxLayout* sideMenuLayout = new QVBoxLayout(sidebarFrame);
    sideMenuLayout->setContentsMargins(10, 40, 10, 20);
    sideMenuLayout->setSpacing(15);

    logoLabel = new QLabel("✈️ TEER ENTA");
    logoLabel->setStyleSheet("font: bold 20px; color: #7d85f5; padding: 10px;");
    logoLabel->setAlignment(Qt::AlignCenter);
    sideMenuLayout->addWidget(logoLabel);
    sideMenuLayout->addSpacing(20);




    QWidget* profileWidget = new QWidget();
    QVBoxLayout* profileLayout = new QVBoxLayout(profileWidget);

    profilePicLabel = new QLabel();
    QPixmap pix("admin.png");
    if (pix.isNull()) {
        profilePicLabel->setText("👤");
        profilePicLabel->setStyleSheet("font-size: 45px; background-color: #1c2237; border-radius: 50px; color: #5c67ff; border: 2px solid #5c67ff;");
    }
    else {
        profilePicLabel->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        profilePicLabel->setStyleSheet("border-radius: 50px; border: 2px solid #5c67ff;");
    }

    profilePicLabel->setFixedSize(100, 100);
    profilePicLabel->setAlignment(Qt::AlignCenter);

    adminNameLabel = new QLabel("Admin Name");
    adminNameLabel->setStyleSheet("font: bold 16px; color: #ffffff; margin-top: 10px;");
    adminNameLabel->setAlignment(Qt::AlignCenter);

    profileLayout->addWidget(profilePicLabel, 0, Qt::AlignCenter);
    profileLayout->addWidget(adminNameLabel, 0, Qt::AlignCenter);
    sideMenuLayout->addWidget(profileWidget);
    sideMenuLayout->addSpacing(20);





    QString sideBtnStyle =
        "QPushButton { backgrou nd-color: transparent; color: #8a92a6; border: none; text-align: left; padding: 15px 25px; font-size: 15px; }"
        "QPushButton:hover { background-color: #1c2237; color: #ffffff; border-radius: 10px; }";


    addPlaneBtn = new QPushButton("  🛫 Add Airport"); 
    userManagmentBtn = new QPushButton(" 👤 User Management");
    addAdminBtn = new QPushButton(" 👑 Add Admin");
    viewAdminsBtn = new QPushButton(" ➕ Add Airplane");
    addAirportBtn = new QPushButton(" 👑 View Admins");
    refreshBtn = new QPushButton("🔄 Refresh");
    refreshBtn->setFixedWidth(100);
    refreshBtn->setStyleSheet(
        "QPushButton { background-color: #1c2237; color: #5cd6ff;"
        "border: 1px solid #5cd6ff; border-radius: 8px; padding: 8px; }"
        "QPushButton:hover { background-color: #2a3350; }");

    userManagmentBtn->setStyleSheet(sideBtnStyle);
    addPlaneBtn->setStyleSheet(sideBtnStyle);
    addAdminBtn->setStyleSheet(sideBtnStyle);
    viewAdminsBtn->setStyleSheet(sideBtnStyle);
    addAirportBtn->setStyleSheet(sideBtnStyle);

    logoutBtn = new QPushButton(" 🚪 Log Out");
    logoutBtn->setStyleSheet("QPushButton { background-color: #3d1c1c; color: #ff6b6b; border-radius: 8px; padding: 10px; margin: 10px; font-weight: bold; border: none; }"
        "QPushButton:hover { background-color: #c0392b; color: white; }");
    //logout function 
    connect(logoutBtn, &QPushButton::clicked, this, [=]() {
        Login* loginpage = new Login();
        loginpage->show();
        this->close();
        });


    sideMenuLayout->addWidget(userManagmentBtn);
    sideMenuLayout->addWidget(addPlaneBtn);
    sideMenuLayout->addWidget(viewAdminsBtn);
    sideMenuLayout->addWidget(addAirportBtn);
    sideMenuLayout->addStretch();
    sideMenuLayout->addWidget(logoutBtn);

    tableTitleLabel = new QLabel("USER MANAGEMENT 👤");
    tableTitleLabel->setStyleSheet("font: bold 22px; color: #ffffff;");

    addUserBtn = new QPushButton("Add User");
    addUserBtn->setFixedWidth(130);
    addUserBtn->setStyleSheet("background-color: #5c67ff; color: white; font-weight: bold; border-radius: 8px; padding: 8px; border: none;");

    QHBoxLayout* topBarLayout = new QHBoxLayout();
    topBarLayout->addWidget(tableTitleLabel);
    topBarLayout->addStretch();
    topBarLayout->addWidget(addUserBtn);
    topBarLayout->addWidget(refreshBtn);
    topBarLayout->setContentsMargins(20, 20, 20, 10);

    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->addLayout(topBarLayout);
    contentLayout->addWidget(stackedWidget);
    contentLayout->setContentsMargins(10, 0, 20, 20);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(sidebarFrame);
    mainLayout->addLayout(contentLayout);
}




void AdminPage::setupConnections() {

    connect(refreshBtn, &QPushButton::clicked,
        this, &AdminPage::loadUsersTable);


    connect(userManagmentBtn, &QPushButton::clicked, [this]() {
        loadUsersTable();
        stackedWidget->setCurrentIndex(0);
        tableTitleLabel->setText("USER MANAGEMENT 👤");
        addUserBtn->setVisible(true);
        });

    //connect(flightMonitorBtn, &QPushButton::clicked, [this]() {
    //    stackedWidget->setCurrentIndex(1);
    //    tableTitleLabel->setText("FLIGHT MONITOR ✈️");
    //    addUserBtn->setVisible(false);
    //    });

    connect(addPlaneBtn, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(3);
        tableTitleLabel->setText("ADD AIRPORT ✈️");
        addUserBtn->setVisible(false);
        });
    connect(viewAdminsBtn, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(2);
        tableTitleLabel->setText("ADD PLANE ✈️");
        addUserBtn->setVisible(false);
        });
    connect(addAirportBtn, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentIndex(1);
        tableTitleLabel->setText("ADD ADMIN 👑");
        addUserBtn->setVisible(false);
        });
    connect(addAdminBtn, &QPushButton::clicked, this, &AdminPage::showAddAdminDialog);
    connect(addUserBtn, &QPushButton::clicked, this, &AdminPage::showAddUserDialog);
    connect(logoutBtn, &QPushButton::clicked, this, &AdminPage::logoutRequested);
}






void AdminPage::setAdminInfo(QString name) {
    adminNameLabel->setText("Admin: " + name);
}




void AdminPage::setupEnterNavigation(QList<QLineEdit*> fields) {
    for (int i = 0; i < fields.size(); i++) {
        QLineEdit* current = fields[i];
        QLineEdit* next = (i + 1 < fields.size()) ? fields[i + 1] : nullptr;

        connect(current, &QLineEdit::returnPressed, this, [=]() {
            if (next)
                next->setFocus();
            else
                current->clearFocus();
            });
    }
}





void AdminPage::showAddAdminDialog() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Add New Admin");
    dialog->setFixedSize(420, 280);
    dialog->setStyleSheet(
        "QDialog { background-color: #0b101e; }"
        "QLabel  { color: #a0a7b5; font-size: 13px; }"
        "QLineEdit {"
        "   background-color: #1c2237; border: 1px solid #33475B;"
        "   border-radius: 6px; padding: 8px; color: #ffffff; font-size: 13px;"
        "}"
        "QLineEdit:focus { border: 1px solid #5cd6ff; }"
    );

    QVBoxLayout* dlgLayout = new QVBoxLayout(dialog);
    dlgLayout->setContentsMargins(30, 25, 30, 25);
    dlgLayout->setSpacing(16);

    QLabel* dlgTitle = new QLabel("👤  Add New Admin");
    dlgTitle->setStyleSheet("font: bold 18px; color: #ffffff;");
    dlgLayout->addWidget(dlgTitle);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(12);
    form->setLabelAlignment(Qt::AlignRight);

    QLineEdit* nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Full Name");
    nameEdit->setFixedHeight(36);

    QLineEdit* passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setFixedHeight(36);

    form->addRow("Name :", nameEdit);
    form->addRow("Password :", passwordEdit);
    dlgLayout->addLayout(form);

    QHBoxLayout* btnRow = new QHBoxLayout();
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QPushButton* confirmBtn = new QPushButton("＋ Add Admin");

    cancelBtn->setFixedHeight(38);
    confirmBtn->setFixedHeight(38);

    cancelBtn->setStyleSheet(
        "QPushButton { background-color: #1c2237; color: #a0a7b5; border-radius: 8px;"
        "   border: 1px solid #33475B; font-size: 13px; }"
        "QPushButton:hover { background-color: #2a3350; color: white; }"
    );
    confirmBtn->setStyleSheet(
        "QPushButton { background-color: #5c67ff; color: white; font-weight: bold;"
        "   border-radius: 8px; font-size: 13px; border: none; }"
        "QPushButton:hover { background-color: #4a55e0; }"
    );

    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(confirmBtn);
    dlgLayout->addLayout(btnRow);

    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::reject);

    connect(confirmBtn, &QPushButton::clicked, dialog, [=]() {
        if (nameEdit->text().trimmed().isEmpty() ||
            passwordEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(dialog, "Missing Info", "Please fill in all fields.");
            return;
        }

        emit addAdminRequested(nameEdit->text().trimmed(), passwordEdit->text().trimmed());



        
        int row = adminsTable->rowCount();
        adminsTable->insertRow(row);
        adminsTable->setItem(row, 0, new QTableWidgetItem(nameEdit->text().trimmed()));

        QMessageBox::information(dialog, "Success",
            "✅  Admin "" + nameEdit->text().trimmed() + "" added successfully!");
        dialog->accept();
        });

    dialog->exec();
}



void AdminPage::setupAddAirportForm() {
    addAirportWidget = new QWidget();
    addAirportWidget->setStyleSheet("background-color: #0b101e;");

    QVBoxLayout* mainLay = new QVBoxLayout(addAirportWidget);
    mainLay->setContentsMargins(40, 40, 40, 40);
    mainLay->setSpacing(20);

    QLabel* title = new QLabel("ADD NEW AIRPORT 🛫 ");
    title->setStyleSheet("font: bold 22px; color: #ffffff;");
    mainLay->addWidget(title);

    QFrame* card = new QFrame();
    card->setStyleSheet(
        "QFrame { background-color: #0d1f2d; border-radius: 12px; border: 1px solid #00AEEF; }"
        "QLabel { color: #a0a7b5; font-size: 13px; border: none; }"
        "QComboBox { background-color: #020617; border: 1px solid #00AEEF;"
        "   border-radius: 6px; padding: 8px; color: #ffffff; font-size: 13px; }"
        "QComboBox:focus { border: 2px solid #00AEEF; }"
        "QComboBox QAbstractItemView { background-color: #1c2237; color: white; }"
        "QLineEdit { background-color: #020617; border: 1px solid #00AEEF;"
        "   border-radius: 6px; padding: 8px; color: white; font-size: 13px; }"
        "QLineEdit:focus { border: 2px solid #00AEEF; }");
    card->setMaximumWidth(520);
    card->setMinimumWidth(400);

    QFormLayout* form = new QFormLayout(card);
    form->setContentsMargins(30, 30, 30, 30);
    form->setSpacing(18);
    form->setLabelAlignment(Qt::AlignRight);
    airportCombo = new QComboBox();
    airportCombo->setFixedHeight(38);


    QString destinations[] = {
        "Cairo, Egypt", "Alexandria, Egypt", "Luxor, Egypt",
        "Aswan, Egypt", "Sharm El Sheikh, Egypt", "Hurghada, Egypt",
        "Marsa Alam, Egypt", "Sohag, Egypt", "Dubai, UAE",
        "Riyadh, KSA", "Jeddah, KSA", "Kuwait City, Kuwait",
        "London, UK", "Paris, France", "Rome, Italy",
        "Istanbul, Turkey", "New York, USA", "Tokyo, Japan",
        "Berlin, Germany", "Madrid, Spain"
    };

    for (const QString& dest : destinations)
        airportCombo->addItem(dest);

    form->addRow("Location :", airportCombo);

    airportEmailEdit = new QLineEdit();
    airportEmailEdit->setPlaceholderText("airport@gmail.com");
    airportEmailEdit->setFixedHeight(38);
    form->addRow("Email:", airportEmailEdit);

    airportPassEdit = new QLineEdit();
    airportPassEdit->setPlaceholderText("Password");
    airportPassEdit->setEchoMode(QLineEdit::Password);
    airportPassEdit->setFixedHeight(38);
    form->addRow("Password :", airportPassEdit);

    airportConfirmPassEdit = new QLineEdit();
    airportConfirmPassEdit->setPlaceholderText("Confirm Password");
    airportConfirmPassEdit->setEchoMode(QLineEdit::Password);
    airportConfirmPassEdit->setFixedHeight(38);
    form->addRow("Confirm :", airportConfirmPassEdit);

    airportCapacityEdit = new QLineEdit();
    airportCapacityEdit->setPlaceholderText("e.g. 50");
    airportCapacityEdit->setFixedHeight(38);
    form->addRow("Capacity :", airportCapacityEdit);

    QPushButton* addBtn = new QPushButton("➕ Add Airport to System");
    addBtn->setFixedHeight(42);
    addBtn->setStyleSheet(
        "QPushButton { background-color: #5c67ff; color: white; font-weight: bold;"
        "   border-radius: 8px; font-size: 14px; border: none; }"
        "QPushButton:hover { background-color: #4a55e0; }"
    );

    connect(addBtn, &QPushButton::clicked, this, &AdminPage::addAirportToSystem);

    form->addRow(addBtn);
    mainLay->addWidget(card);
    mainLay->addSpacing(20);

    QLabel* addedTitle = new QLabel("Airports added to system");
    addedTitle->setStyleSheet("font: bold 16px; color: #5cd6ff;");
    mainLay->addWidget(addedTitle);

    airportsContainer = new QWidget();
    airportsContainer->setStyleSheet("background: transparent;");
    airportsCardsLayout = new QVBoxLayout(airportsContainer);
    airportsCardsLayout->setSpacing(10);
    airportsCardsLayout->setContentsMargins(5, 5, 5, 5);
    airportsCardsLayout->setAlignment(Qt::AlignTop);

    mainLay->addWidget(airportsContainer);


    QScrollArea* airportScroll = new QScrollArea();
    airportScroll->setWidget(addAirportWidget);
    airportScroll->setWidgetResizable(true);
    airportScroll->setStyleSheet("border: none; background-color: #0b101e;");
    stackedWidget->addWidget(airportScroll);

}


void AdminPage::loadUsersTable() {
    usersTable->setRowCount(0);
    for (int i = 0; i < users.size; i++) {
        int row = usersTable->rowCount();

        usersTable->insertRow(row);


        usersTable->setItem(row, 0, new QTableWidgetItem(
            QString::number(users[i].user_ID)));

        usersTable->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(users[i].username)));

        usersTable->setItem(row, 2, new QTableWidgetItem("User"));

        usersTable->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(users[i].email)));
    }
}

void AdminPage::addAirportToSystem() {



    if (airportEmailEdit->text().isEmpty() ||
        airportPassEdit->text().isEmpty() ||
        airportConfirmPassEdit->text().isEmpty() ||
        airportCapacityEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please fill all fields");
        return;
    }






    bool ok;//! to check the user entered intiger number

    int capacity = airportCapacityEdit->text().toInt(&ok);

    if (!ok || capacity <= 0) {
        QMessageBox::warning(this, "Invalid", "Capacity must be a positive number.");
        return;
    }

    string selected = airportCombo->currentText().toStdString();
    City selectedCity;

    bool found = false;

    for (int i = 0; i < cities.size; i++) {
        if (cities[i].location == selected)
        {
            selectedCity = cities[i];
            found = true;

            break;
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Error", "City already has an airport or not found."); return;
    }

    CREATE_AIRPORT result = create_airport(selectedCity, airportEmailEdit->text().toStdString(), airportPassEdit->text().toStdString(), airportConfirmPassEdit->text().toStdString(), capacity);

    if (result == AIRPORT_SUCCESS) {


        QFrame* airportCard = new QFrame();
        airportCard->setStyleSheet(
            "QFrame { background-color: #1c2237; border-radius: 10px; border: 1px solid #00AEEF; }"
            "QLabel { color: white; border: none; }"
        );
        airportCard->setMinimumHeight(90);

        QVBoxLayout* cardLayout = new QVBoxLayout(airportCard);

        QLabel* cityLbl = new QLabel("🛫 " + airportCombo->currentText());
        cityLbl->setStyleSheet("font: bold 15px; color: #5cd6ff;");

        QLabel* emailLbl = new QLabel(airportEmailEdit->text());
        emailLbl->setStyleSheet("color: #a0a7b5;");

        QLabel* capLbl = new QLabel("Capacity: " + airportCapacityEdit->text());
        capLbl->setStyleSheet("color: #a0a7b5;");

        cardLayout->addWidget(cityLbl);
        cardLayout->addWidget(emailLbl);
        cardLayout->addWidget(capLbl);

        airportsCardsLayout->addWidget(airportCard);
        airportsContainer->adjustSize();
        airportEmailEdit->clear();
        airportPassEdit->clear();
        airportConfirmPassEdit->clear();
        airportCapacityEdit->clear();

        QMessageBox::information(this, "Success", "✅ Airport added successfully!");
    }

    else if (result == AIRPORT_PASSWORD_MISMATCH)
        QMessageBox::warning(this, "Error", "Passwords don't match.");
    else if (result == AIRPORT_WEAK_PASSWORD)
        QMessageBox::warning(this, "Error", "Password too weak.");
    else if (result == AIRPORT_EMAIL_USED)
        QMessageBox::warning(this, "Error", "Email already used.");
}

void AdminPage::addPlaneToSystem() {
    QString seats = planeSeatsEdit->text().trimmed();
    QString speed = planeSpeedEdit->text().trimmed();
    QString fuel = planeFuelEdit->text().trimmed();

    if (seats.isEmpty() || speed.isEmpty() || fuel.isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please fill in all fields.");
        return;
    }
    bool ok1, ok2, ok3;

    int seatsNum = seats.toInt(&ok1);
    double speedVal = speed.toDouble(&ok2);
    double fuelVal = fuel.toDouble(&ok3);

    if (!ok1 || seatsNum <= 0 || !ok2 || !ok3) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid numbers.");
        return;
    }

    string mfr = manufacturerCombo->currentText().toStdString();
    string model = modelCombo->currentText().toStdString();

    for (int i = 0; i < models.size; i++) {
        if (models[i].manufacturer == mfr && models[i].model == model) {
            QMessageBox::warning(this, "Duplicate", "This model already exists!");
            return;
        }
    }
    emit addPlaneModelRequested(
        manufacturerCombo->currentText(),
        modelCombo->currentText(),
        seatsNum, speedVal, fuelVal
    );
    int row = planesTable->rowCount();
    planesTable->insertRow(row);
    planesTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
    planesTable->setItem(row, 1, new QTableWidgetItem(manufacturerCombo->currentText()));
    planesTable->setItem(row, 2, new QTableWidgetItem(modelCombo->currentText()));
    planesTable->setItem(row, 3, new QTableWidgetItem(seats));
    planesTable->setItem(row, 4, new QTableWidgetItem(speed + " km/h"));

    planeSeatsEdit->clear();
    planeSpeedEdit->clear();
    planeFuelEdit->clear();

    bool success = add_plane_to_system(mfr, model, seatsNum, speedVal, fuelVal);
    cout << success << endl;
    QMessageBox::information(this, "Success", "✅ Model added successfully!");

}
void AdminPage::updatePlaneModels() {
    modelCombo->clear();
    QString mfr = manufacturerCombo->currentText();
    if (mfr == "Airbus")
        modelCombo->addItems({ "A220", "A319", "A320", "A321", "A330", "A350", "A380" });
    else if (mfr == "Boeing")
        modelCombo->addItems({ "737-800", "737 MAX", "747-400", "767", "777", "787 Dreamliner" });
    else if (mfr == "Embraer")
        modelCombo->addItems({ "E170", "E175", "E190", "E195" });
}