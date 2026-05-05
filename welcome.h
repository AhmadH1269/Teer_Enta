#ifndef WELCOME_H
#define WELCOME_H


#pragma once
#include "Qtprojecto.h"
#include <QMainWindow>
#include "ui_welcome.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPixmap>

class welcome : public QWidget
{
	Q_OBJECT

public:
	welcome(QWidget *parent = nullptr);
	

private:
	QGridLayout* layout;
	QLabel* image;
	QLabel* name;
	QPushButton* continueBtn;
	QPushButton* tagsBtn;

	void WapplyStyles(); // wlcome page function for style 

};

#endif