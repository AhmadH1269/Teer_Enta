#include "AppController.h"
#include "login.h"
#include "adminPage.h"
#include "signupPage.h""
#include "structs.h"
#include <QPalette>
#include <QVBoxLayout>

AppController::AppController(QWidget* parent) : QStackedWidget(parent) {
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor("#020617"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    loginPage = new Login();
    //adminPage = new AdminPage();
   
    signupPage = new SignupPage();
    

    this->addWidget(loginPage);   
    this->addWidget(adminPage);   
    this->addWidget(signupPage);  
       

    this->setCurrentWidget(loginPage);
    setupConnections();
    this->resize(1400, 1000);
    this->showMaximized();
}

void AppController::setupConnections() {

    connect(loginPage, &Login::loginSuccessful, this, [=](std::string username, LOGIN userType) {
         if (userType == ADMIN) {
            adminPage->setAdminInfo(QString::fromStdString(username));
            this->setCurrentWidget(adminPage);
        }
  
    });

    connect(adminPage, &AdminPage::logoutRequested, this, [=]() {
        loginPage->clearFields();
        this->setCurrentWidget(loginPage);
        });

    connect(loginPage, &Login::goToSignup, this, [=]() {
        this->setCurrentWidget(signupPage);
        });

    connect(signupPage, &SignupPage::backToLogin, this, [=]() {
        this->setCurrentWidget(loginPage);
        });

}
