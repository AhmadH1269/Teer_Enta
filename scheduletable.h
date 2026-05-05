#ifndef SCHEDULETABLE_H
#define SCHEDULETABLE_H
#include <QDialog>
#include <QWidget>

namespace Ui {
class ScheduleTable;
}

class ScheduleTable : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleTable(QDialog *parent = nullptr);
    ~ScheduleTable();

private:
    Ui::ScheduleTable *ui;
};

#endif // SCHEDULETABLE_H
