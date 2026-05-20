#include "signUpPage.h"
#include"functions.h"
#include <QGridLayout>
#include<QMessageBox>
#include <QPalette>
#include <QColor>
#include "login.h"


SignupPage::SignupPage(QWidget* parent) : QWidget(parent)

{
	setupLayout();
	setupConnections();
}


void SignupPage::handleSignup() {
	if (usernameEdit->text().isEmpty() ||
		emailEdit->text().isEmpty() ||
		phoneEdit->text().isEmpty() ||
		passwordEdit->text().isEmpty()) {
		QMessageBox::warning(this, "Wait!", "Please fill all fields");
		return;
	}

	string username = usernameEdit->text().toStdString();
	string email = emailEdit->text().toStdString();
	string phone = phoneEdit->text().toStdString();
	string pass1 = passwordEdit->text().toStdString();
	string pass2 = confirmPasswordEdit->text().toStdString();

	Pair<SIGN_UP, int> result = signup(email, username, pass1, pass2, phone);

	if (result.first == USER_SUCCESS) {

		emit signupSuccessful(result.second);
	}

	else if (result.first == USER_PASSWORD_MISMATCH) {
		QMessageBox::warning(this, "Error", "Password doesn't match ");

	}
	else if (result.first == USER_WEAK_PASSWORD) {
		QMessageBox::warning(this, "Error", "Password too weak");
	}
	else if (result.first == USER_EMAIL_USED) {
		QMessageBox::warning(this, "Error", "Email already used");

	}


	else if (result.first == USER_USERNAME_TAKEN) {
		QMessageBox::warning(this, "Error", "Username already used");

	}
}



void SignupPage::setupLayout() {
	

	QPalette pal = this->palette();
	pal.setColor(QPalette::Window, QColor("#020617"));
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	setFixedSize(500, 600);


	QString inputStyle =
		"QLineEdit {"
		"   background-color: #1A2634;"
		"   border: 1px solid #33475B;"
		"   border-radius: 5px;"
		"   padding: 8px;"
		"   color: white;"
		"   font: bold 14px;"
		"}"
		"QLineEdit:focus { border: 1px solid #00AEEF; }";

	usernameEdit = new QLineEdit(this);
	usernameEdit->setPlaceholderText("اسمك و اسم الحج");
	usernameEdit->setFixedSize(250, 40);
	usernameEdit->setStyleSheet(inputStyle);



	emailEdit = new QLineEdit(this);
	emailEdit->setPlaceholderText("البريد الإلكتروني");
	emailEdit->setFixedSize(250, 40);
	emailEdit->setStyleSheet(inputStyle);

	phoneEdit = new QLineEdit(this);
	phoneEdit->setPlaceholderText("رقم المحمول");
	phoneEdit->setFixedSize(250, 40);
	phoneEdit->setStyleSheet(inputStyle);

	passwordEdit = new QLineEdit(this);
	passwordEdit->setPlaceholderText("أمن نفسك");
	passwordEdit->setEchoMode(QLineEdit::Password);
	passwordEdit->setFixedSize(250, 40);
	passwordEdit->setStyleSheet(inputStyle);

	confirmPasswordEdit = new QLineEdit(this);
	confirmPasswordEdit->setPlaceholderText("زيادة امان");
	confirmPasswordEdit->setEchoMode(QLineEdit::Password);
	confirmPasswordEdit->setFixedSize(250, 40);
	confirmPasswordEdit->setStyleSheet(inputStyle);

	signupBtn = new QPushButton("عاشت الأسامي", this);
	signupBtn->setFixedSize(110, 40);
	signupBtn->setStyleSheet(
		"QPushButton { background-color: #00AEEF; color: white;"
		"border-radius: 5px; font: bold 15px; }"
		"QPushButton:hover { background-color: #008CBA; }");

	BackBtn = new QPushButton("لف وأرجع تاني ", this);
	BackBtn->setFixedSize(110, 40);
	BackBtn->setStyleSheet(
		"QPushButton { background-color: transparent; color: #00AEEF;"
		"border: 1px solid #00AEEF; border-radius: 5px; font: bold 15px; }"
		"QPushButton:hover { background-color: rgba(0,174,239,0.1); }");
	
	
	connect(BackBtn, &QPushButton::clicked, this, [=]() {
		Login* loginWin = new Login();
		loginWin->show();
		this->close();
		});

	

	QString labelStyle = "color: #8A99A8; font: bold 14px;";

	QLabel* usernameLbl = new QLabel("Username", this);
	QLabel* emailLbl = new QLabel("Email", this);
	QLabel* phoneLbl = new QLabel("Phone", this);
	QLabel* passLbl = new QLabel("Password", this);
	QLabel* confirmLbl = new QLabel("Confirm", this);

	usernameLbl->setStyleSheet(labelStyle);
	emailLbl->setStyleSheet(labelStyle);
	phoneLbl->setStyleSheet(labelStyle);
	passLbl->setStyleSheet(labelStyle);
	confirmLbl->setStyleSheet(labelStyle);


	QGridLayout* grid = new QGridLayout();
	grid->setSpacing(10);
	grid->setContentsMargins(50, 0, 50, 0);

	grid->addWidget(usernameLbl, 0, 0);
	grid->addWidget(usernameEdit, 0, 1);
	grid->addWidget(emailLbl, 1, 0);
	grid->addWidget(emailEdit, 1, 1);
	grid->addWidget(phoneLbl, 2, 0);
	grid->addWidget(phoneEdit, 2, 1);
	grid->addWidget(passLbl, 3, 0);
	grid->addWidget(passwordEdit, 3, 1);
	grid->addWidget(confirmLbl, 4, 0);
	grid->addWidget(confirmPasswordEdit, 4, 1);



	QHBoxLayout* btnLayout = new QHBoxLayout();
	btnLayout->addStretch();
	btnLayout->addWidget(BackBtn);
	btnLayout->addWidget(signupBtn);
	btnLayout->addStretch();

	

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(30, 30, 30, 30);
	mainLayout->setSpacing(20);
	mainLayout->addLayout(grid);
	mainLayout->addLayout(btnLayout);
	mainLayout->addStretch();


}


void SignupPage::setupConnections() {

	connect(usernameEdit, &QLineEdit::returnPressed, this, &SignupPage::EnterPressed);
	connect(emailEdit, &QLineEdit::returnPressed, this, &SignupPage::EnterPressed);
	connect(phoneEdit, &QLineEdit::returnPressed, this, &SignupPage::EnterPressed);
	connect(passwordEdit, &QLineEdit::returnPressed, this, &SignupPage::EnterPressed);
	connect(confirmPasswordEdit, &QLineEdit::returnPressed, this, &SignupPage::EnterPressed);


	connect(signupBtn, &QPushButton::clicked, this, &SignupPage::handleSignup);
	connect(BackBtn, &QPushButton::clicked, this, &SignupPage::backToLogin);
}

void SignupPage::EnterPressed()
{
	if (sender() == usernameEdit)
		emailEdit->setFocus();

	else if (sender() == emailEdit)
		phoneEdit->setFocus();

	else if (sender() == phoneEdit)
		passwordEdit->setFocus();

	else if (sender() == passwordEdit)
		confirmPasswordEdit->setFocus();

	else if (sender() == confirmPasswordEdit)
		handleSignup();
}
