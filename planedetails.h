#pragma once

#include <QDialog>

namespace Ui {
class PlaneDetails;
}

class PlaneDetails : public QDialog
{
    Q_OBJECT

public:
    void setTargetRow(int row);

    explicit PlaneDetails(QWidget *parent = nullptr);
    ~PlaneDetails();

private:
    int m_row;
    Ui::PlaneDetails *ui;
};


