#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H
#include <QStackedWidget>
#include <string>

class Login;
class AdminPage;
class SignupPage;
;

class AppController : public QStackedWidget {
    Q_OBJECT
public:
    explicit AppController(QWidget* parent = nullptr);
private:
    Login* loginPage;
    AdminPage* adminPage;
    SignupPage* signupPage;
    

    void setupConnections();

};
#endif