#pragma once
#include<QWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QLabel>
#include "functions.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
class SignupPage : public QWidget
{
	Q_OBJECT

public:

	explicit SignupPage(QWidget* parent = nullptr);
signals:
	void signupSuccessful(int user_ID);
	void backToLogin();

private:
	QLineEdit* usernameEdit;
	QLineEdit* emailEdit;
	QLineEdit* phoneEdit;

	QLineEdit* passwordEdit;
	QLineEdit* confirmPasswordEdit;
	QPushButton* signupBtn;
	QPushButton* BackBtn;

	void setupLayout();
	void setupConnections();
	void handleSignup();
	void EnterPressed();
};