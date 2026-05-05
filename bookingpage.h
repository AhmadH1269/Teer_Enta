#ifndef BOOKINGPAGE_H
#define BOOKINGPAGE_H

#include <QDialog>

namespace Ui {
    class BookingPage;
}

class BookingPage : public QDialog
{
    Q_OBJECT

public:
    explicit BookingPage(QWidget* parent = nullptr);
    ~BookingPage();

private:
    Ui::BookingPage* ui;
};

#endif // BOOKINGPAGE_H
