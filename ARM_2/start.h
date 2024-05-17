#ifndef START_H
#define START_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class Start;
}

class Start : public QDialog
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = nullptr);
    ~Start();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void mainwin();

private:
    Ui::Start *ui;
    QSqlDatabase db;
};

#endif // START_H
