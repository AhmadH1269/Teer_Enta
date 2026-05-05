#ifndef ADMINPAGE_H
#define ADMINPAGE_H
#include<QLabel>
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <string>
#include <QMessageBox>
#include <QHeaderView>
#include<QDialog>
#include<QFrame>
#include<QLineEdit>
#include <QList>
#include "structs.h"
#include<QScrollArea>
#include <QComboBox>
#include "functions.h"
#include "data_base.h"



class AdminPage : public QWidget {
    Q_OBJECT // We tell qt that is a smart object that ues smart features like signalls and slots

public:
    explicit AdminPage(QWidget* parent = nullptr); // Constructor
  
    ~AdminPage() {
        SAVE_DATA();
    }

    void setAdminInfo(QString name);
    void loadUsersTable();
signals:
    void logoutRequested(); // Signal to notify when logout is requested
    void addAdminRequested(QString name, QString password);
    void addPlaneModelRequested(QString manufacturer, QString model, int seats, double speed, double fuel);
private slots:
    // To use when we want to update the array after editing the users table
    void showAddUserDialog();
    void addPlaneToSystem();
    void showAddAdminDialog();
    void addAirportToSystem();
    void updatePlaneModels();


private:

    int actualUsersCount = 0;
    int planesCount = 0;
    int airportsCount = 0;
    std::string planesInSystem[200][4];

    // Declaraation
    QLabel* logoLabel;
    QTableWidget* usersTable;
    QTableWidget* adminsTable;
    QWidget* addPlaneWidget;
    QStackedWidget* stackedWidget;
    QWidget* adminsWidget;
    QPushButton* viewAdminsBtn;
    QWidget* addAirportWidget;
    QPushButton* addAirportBtn;
    QWidget* airportsContainer;
    QVBoxLayout* airportsCardsLayout;
    QScrollArea* airportsScrollArea;
    QPushButton* refreshBtn;
    QComboBox* airportCombo;
    QLineEdit* airportEmailEdit;
    QLineEdit* airportPassEdit;
    QLineEdit* airportConfirmPassEdit;
    QLineEdit* airportCapacityEdit;
    std::string airportsInSystem[20][2];


    QPushButton* addPlaneBtn;
    QPushButton* userManagmentBtn;
    QPushButton* addUserBtn; // Not in the left sidebar 
    QPushButton* addAdminBtn;
    QPushButton* logoutBtn;


    QLabel* tableTitleLabel;
    QLabel* profilePicLabel;
    QLabel* adminNameLabel;
  
      // QWidget* addPlaneWidget;
    QComboBox* manufacturerCombo;
    QComboBox* modelCombo;
    QLineEdit* planeSeatsEdit;
    QLineEdit* planeSpeedEdit;
    QLineEdit* planeFuelEdit;
    QTableWidget* planesTable;

    // Methods
    void setupTables();
    void setupLayout();
    void setupAddPlaneForm();
    // void updateArray(int row, int column);---> for debuging
    void setupConnections();
    void setupEnterNavigation(QList<QLineEdit*> fields); // For better UI/UX 
    void setupAddAirportForm();
};

#endif 