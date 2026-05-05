

#pragma once
#include<QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QMessageBox>
#include<QPushButton>
#include<QMainWindow>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QGridLayout>
#include<QGroupBox>
#include<QTextEdit>
#include<QFormLayout>
#include<QComboBox>
#include<QSpinBox>
#include<QCheckBox>
#include<QIcon>
#include<QToolButton>
#include<QTableWidget>
#include<QHeaderView>


#include "ui_Qtprojecto.h"

class Qtprojecto : public QMainWindow
{
    Q_OBJECT

public:
    Qtprojecto(QWidget *parent = nullptr);
    ~Qtprojecto();

private:
    Ui::QtprojectoClass ui;
};
