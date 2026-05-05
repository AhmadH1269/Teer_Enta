#ifndef QTAIRPORT_H
#define QTAIRPORT_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <string>
#include "structs.h"
#include "data_base.h"
#include "functions.h"


class QtAirport : public QMainWindow {
    Q_OBJECT

public:
    explicit QtAirport(int airportID, QWidget* parent = nullptr);
    void refresh();   // call this after any backend mutation to re sync
    ~QtAirport() {
        SAVE_DATA();
    }

private:
    int airport_ID;


    QWidget* centralWidget;
    QHBoxLayout* mainLayout;
    QFrame* sidebar;
    QVBoxLayout* sidebarLayout;
    QStackedWidget* contentStack;


    //  Buttons
    QPushButton* btnDashboard;
    QPushButton* btnAirplanes;
    QPushButton* btnSchedule;
    QPushButton* btnRequests;
    QPushButton* btnLogout;

    //  dashboard cards
    QLabel* lblTotalLanes;
    QLabel* lblAvailablePlanes;
    QLabel* lblScheduledFlights;
    QLabel* lblPendingRequests;

    // Pages
    QWidget* DashboardPage();
    QWidget* AirplanesPage();
    QWidget* SchedulePage();
    QWidget* FlightRequestsPage();
    
    //tables

    QTableWidget* airplanesTable;
    QTableWidget* scheduleTable;
    QTableWidget* requestsTable;
    QTableWidget* sentRequestsTable;

    //  functions
    QWidget* createStatCard(const QString& title, const QString& value,
                const QString& color, QLabel*& outLabel);
    
    void applyStyles();
    void populateAirplanesTable();
    void populateScheduleTable();
    void populateRequestsTable();
    


 private slots:
        void openBuyAirplane();
        void openScheduleDialog();
        void handleAcceptRequest();
        void handleDenyRequest();
        
};

#endif

//#pragma once
//
//#include <QMainWindow>
//#include <QStackedWidget>
//#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include <QGridLayout>
//#include <QPushButton>
//#include <QLabel>
//#include <QFrame>
//#include <QTableWidget>
//#include <QHeaderView>
//#include <QDialog>
//#include <QLineEdit>
//#include <QComboBox>
//#include <QSpinBox>
//#include <QDoubleSpinBox>
//#include <QDateTimeEdit>
//#include <QString>
//
//// Include your custom backend structures
//#include "structs.h"
//#include "data_base.h"
//
//class QtAirport : public QMainWindow {
//    Q_OBJECT
//
//public:
//    QtAirport(QWidget* parent = nullptr);
//
//private:
//    // --- UI Pointers ---
//    QWidget* centralWidget;
//    QHBoxLayout* mainLayout;
//    QFrame* sidebar;
//    QVBoxLayout* sidebarLayout;
//    QStackedWidget* contentStack;
//
//    QPushButton* btnDashboard;
//    QPushButton* btnAirplanes;
//    QPushButton* btnSchedule;
//    QPushButton* btnRequests;
//    QPushButton* btnLogout;
//
//    // Table Pointers (needed for refreshing)
//    QTableWidget* airplanesTable;
//    QTableWidget* scheduleTable;
//    QTableWidget* requestsTable;
//
//    // Dashboard Label Pointers (needed for refreshing)
//    QLabel* lblTotalLanes;
//    QLabel* lblAvailablePlanes;
//    QLabel* lblScheduledFlights;
//    QLabel* lblPendingRequests;
//
//    // --- State ---
//    // Assuming 0 is the index of the currently logged-in airport
//    int current_airport_index = 0;
//
//    // --- Page Creators ---
//    QWidget* DashboardPage();
//    QWidget* AirplanesPage();
//    QWidget* SchedulePage();
//    QWidget* FlightRequestsPage();
//
//    // --- Helpers ---
//    QWidget* createStatCard(QString title, QLabel* valueLabel, QString color);
//    void applyStyles();
//
//    // --- Refresh Functions ---
//    void refreshDashboard();
//    void refreshAirplanesTable();
//    void refreshScheduleTable();
//    void refreshRequestsTable();
//
//private slots:
//    void openBuyAirplane();
//    void openScheduleDialog();
//    // void handleLogout(); // Uncomment when ready
//};


//#pragma once
//
//#include <QMainWindow>
//#include <QWidget>
//#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include <QGridLayout>
//#include <QStackedWidget>
//#include <QFrame>
//#include <QLabel>
//#include <QPushButton>
//#include <QTableWidget>
//#include <QHeaderView>
//#include <QDialog>
//#include <QLineEdit>
//#include <QComboBox>
//#include <QSpinBox>
//#include <QDoubleSpinBox>
//#include <QDateTimeEdit>
//#include <QMessageBox>
//#include <QString>
//#include <QFormLayout>
//
//#include "structs.h"
//#include "functions.h"
//
//class QtAirport : public QMainWindow {
//    Q_OBJECT
//
//public:
//    explicit QtAirport(int airportID, QWidget* parent = nullptr);
//    void refresh();   // call this after any backend mutation to re sync 
//
//private slots:
//    void openBuyAirplane();
//    void openScheduleDialog();
//    void openAddModelDialog();
//    void handleAcceptRequest();
//    void handleDenyRequest();
//
//private:
//    int airport_ID;
//
//    // main
//    QWidget* centralWidget;
//    QHBoxLayout* mainLayout;
//    QFrame* sidebar;
//    QVBoxLayout* sidebarLayout;
//    QStackedWidget* contentStack;
//
//    // Sidebar buttons
//    QPushButton* btnDashboard;
//    QPushButton* btnAirplanes;
//    QPushButton* btnSchedule;
//    QPushButton* btnRequests;
//    QPushButton* btnAdmin;
//    QPushButton* btnLogout;
//
//    //  dashboard cards
//    QLabel* lblTotalLanes;
//    QLabel* lblAvailablePlanes;
//    QLabel* lblScheduledFlights;
//    QLabel* lblPendingRequests;
//
//    // Tables 
//    QTableWidget* airplanesTable;
//    QTableWidget* scheduleTable;
//    QTableWidget* requestsTable;
//    QTableWidget* modelsTable;
//
//    // Page 
//    QWidget* DashboardPage();
//    QWidget* AirplanesPage();
//    QWidget* SchedulePage();
//    QWidget* FlightRequestsPage();
//    QWidget* AdminPage();
//
//    // functions 
//    QWidget* createStatCard(const QString& title, const QString& value,
//        const QString& color, QLabel*& outLabel);
//    void populateAirplanesTable();
//    void populateScheduleTable();
//    void populateRequestsTable();
//    void populateModelsTable();
//    void applyStyles();
//};