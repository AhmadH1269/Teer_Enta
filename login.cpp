#include "login.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <string>
#include "functions.h" 
#include "adminpage.h" 
#include "QtAirport.h" 
#include "userpage.h" 
#include "welcome.h" 
#include "signUpPage.h" 


using namespace std;



Login::Login(QWidget* parent) : QWidget(parent)
{
    // Updated background to match Admin Page
    this->setStyleSheet(
        "QLabel { background: transparent; color: #a0a7b5; font-family: 'Segoe UI'; }"
    );

    //?to avoid being bright or dark
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor("#020617"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    //?to aviod bieng bright or dark 

    setFixedSize(500, 600);

    tm = new QLabel("Teer_Enta", this);

    tm->setStyleSheet("color:#00AEEF; font: bold 24px; background: transparent;");//to avoid element style propagatio


    QString inputStyle =
        "QLineEdit {"
        "   background-color: #1A2634;"
        "   border: 1px solid #33475B;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "   color: white;"
        "   font: bold 14px;"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #00AEEF;"
        "}";

    name_edit = new QLineEdit(this);
    name_edit->setPlaceholderText("اسمك و اسم الحج");
    name_edit->setFixedSize(250, 40);

    name_edit->setStyleSheet(inputStyle);

    name = new QLabel("Emial/Username", this);
    name->setStyleSheet("color:#8A99A8; font: bold 14px;");


    password_edit = new QLineEdit(this);
    password_edit->setPlaceholderText("كلمة السر 😉");
    password_edit->setEchoMode(QLineEdit::Password);
    password_edit->setFixedSize(250, 40);

    password_edit->setStyleSheet(inputStyle);

    passwordlbl = new QLabel("Password", this);
    passwordlbl->setStyleSheet("color: #8A99A8; font: bold 14px;");


    showPasswordCheck = new QCheckBox("Show Password", this);

    showPasswordCheck->setStyleSheet("QCheckBox { color: #8A99A8; font: bold 12px; background: transparent; } "
        "QCheckBox::indicator { width: 15px; height: 15px; }");

    // Button Styles
    QString loginBtnStyle =
        "QPushButton {"
        "   background-color: #00AEEF;"
        "   color: white;"
        "   border-radius: 5px;"
        "   font: bold 15px;"
        "}"
        "QPushButton:hover { background-color: #008CBA; }";

    QString signupBtnStyle =
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #00AEEF;"
        "   border: 1px solid #00AEEF;"
        "   border-radius: 5px;"
        "   font: bold 15px;"
        "}"
        "QPushButton:hover { background-color: rgba(0, 174, 239, 0.1); }";

    buttonLog = new QPushButton("يا هلا يا هلااا", this);
    buttonLog->setFixedSize(110, 40);
    buttonLog->setStyleSheet(loginBtnStyle);

    buttonSignup = new QPushButton("اسم الكريم", this);
    buttonSignup->setFixedSize(110, 40);

    buttonSignup->setStyleSheet(signupBtnStyle);

    connect(buttonSignup, &QPushButton::clicked, this, [=]() {
        SignupPage* sighnWin = new SignupPage();
        sighnWin->show();
        this->close();
        });


    // Connections
    connect(showPasswordCheck, &QCheckBox::toggled, this, [=](bool checked) {
        password_edit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
        });

    connect(name_edit, &QLineEdit::returnPressed, this, &Login::EnterPressed);

    connect(password_edit, &QLineEdit::returnPressed, this, &Login::EnterPressed);

    connect(buttonLog, &QPushButton::clicked, this, &Login::LoginValid);

    connect(buttonSignup, &QPushButton::clicked, this, &Login::goToSignup);

    loginLayout();
}

void Login::LoginValid() {

    if (name_edit->text().isEmpty() || password_edit->text().isEmpty()) {
        QMessageBox::warning(this, "Wait!", "Please enter both name and password");
        return;
    }


    string username = name_edit->text().toStdString();
    string pass = password_edit->text().toStdString();

    Pair<LOGIN, int> result = login(username, pass);

    if (result.first == ADMIN) {
        emit loginSuccessful(username, result.first);

        
        connect(buttonLog, &QPushButton::clicked, this, [=]() {
            AdminPage* AdminWin = new AdminPage(result.second);
            AdminWin->show();
            this->close();
            });


    }

    else if (result.first == AIRPORT) {
        emit loginSuccessful(username, result.first);

        connect(buttonLog, &QPushButton::clicked, this, [=]() {
            QtAirport* AirportWin = new QtAirport(result.second);
            AirportWin->show();
            this->close();
            });

    }
    else if (result.first == USER) {
        emit loginSuccessful(username, result.first);

        connect(buttonLog, &QPushButton::clicked, this, [=]() {
        UserPageMain* userWin = new UserPageMain(result.second);
        userWin->show();
        this->close();
        });
    }

    else if (result.first == WRONG_PASSWORD) {
        QMessageBox::critical(this, "Error", "Wrong password!");
    }

    else {
        QMessageBox::critical(this, "Error", "User not found!");
    }
}

void Login::loginLayout() {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 0);
    mainLayout->setSpacing(15);

    QHBoxLayout* logoLayout = new QHBoxLayout();
    tm->setAlignment(Qt::AlignLeft);
    logoLayout->addWidget(tm);
    logoLayout->addStretch();
    mainLayout->addLayout(logoLayout);

    mainLayout->addSpacing(20);


    QGridLayout* inputGrid = new QGridLayout();
    inputGrid->setSpacing(10);
    inputGrid->setContentsMargins(50, 0, 50, 0);

    inputGrid->addWidget(name, 0, 0);
    inputGrid->addWidget(name_edit, 0, 1);
    inputGrid->addWidget(passwordlbl, 1, 0);
    inputGrid->addWidget(password_edit, 1, 1);

    mainLayout->addLayout(inputGrid);


    QHBoxLayout* checkLayout = new QHBoxLayout();
    checkLayout->setContentsMargins(150, 0, 0, 0);
    checkLayout->addWidget(showPasswordCheck);
    checkLayout->addStretch();
    mainLayout->addLayout(checkLayout);


    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(buttonSignup);
    buttonsLayout->addWidget(buttonLog);
    buttonsLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();// to make space undere the buttons

}

void Login::EnterPressed()
{
    if (sender() == name_edit) {
        password_edit->setFocus();
    }
    else if (sender() == password_edit) {
        LoginValid();
    }
}

void Login::clearFields() {
    name_edit->clear();
    password_edit->clear();
    name_edit->setFocus();
}
