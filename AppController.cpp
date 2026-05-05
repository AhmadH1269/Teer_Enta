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
    adminPage = new AdminPage();
   
    signupPage = new SignupPage();
    

    this->addWidget(loginPage);   // 0
    this->addWidget(adminPage);   // 1
    this->addWidget(signupPage);  // 2
       

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

    

    connect(adminPage, &AdminPage::addAdminRequested,
        this, [](QString name, QString password) {
            create_admin(
                name.toStdString(),
                name.toStdString() + "@system.com",
                password.toStdString(),
                password.toStdString()
            );
        }
    );

    connect(adminPage, &AdminPage::addPlaneModelRequested,
        this, [this](QString mfr, QString mdl, int seats, double speed, double fuel) {
            Airplane newModel = {
                number_of_planes++, "", seats,
                mfr.toStdString(), mdl.toStdString(),
                0, speed, fuel
            };
            models.push_back(newModel);
        }
    );

    connect(loginPage, &Login::goToSignup, this, [=]() {
        this->setCurrentWidget(signupPage);
        });

    connect(signupPage, &SignupPage::backToLogin, this, [=]() {
        this->setCurrentWidget(loginPage);
        });

}