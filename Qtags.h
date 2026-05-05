#ifndef TAGS_H
#define TAGS_H

#pragma once
#include "Qtprojecto.h"
#include "ui_Qtags.h"

class Qtags : public QWidget
{
	Q_OBJECT

public:
	Qtags(QWidget *parent = nullptr);
	

private:
	QTableWidget* names_table;
	QVBoxLayout* layout;
	QPushButton* BackBtn;
};

#endif