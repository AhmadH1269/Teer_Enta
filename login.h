#pragma once
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include<QCheckBox>
#include"enums.h"
#include <QWidget>


class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

class Login : public QWidget
{

    Q_OBJECT
public:

    explicit Login(QWidget* parent = nullptr);



    void LoginValid();

    void clearFields(); // When log out it clears QLineEdit fields

    void EnterPressed();
signals:

    void loginSuccessful(std::string username, LOGIN userType);
    void goToSignup();

private:

    //*--> is a ptr look at memory adress ()
    QCheckBox* showPasswordCheck;
    QLabel* tm;
    QLabel* name;
    QLabel* passwordlbl;
    QLineEdit* name_edit;
    QLineEdit* password_edit;
    QPushButton* buttonLog;
    QPushButton* buttonSignup;
    int actualUsersCount = 7;
    // QLabel* copyRights;

    void loginLayout();


};