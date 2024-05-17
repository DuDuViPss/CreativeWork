#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <start.h>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SubHider();
    void SubShower();

private slots:
    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_table_clicked(const QModelIndex &index);

    void on_btnExit_clicked();

    void on_remove_clicked();

    void on_btnSub_clicked();

    void on_table_doubleClicked(const QModelIndex &index);

    void on_btnSubAdd_clicked();

    void on_btnSubDel_clicked();

    void on_btnAtt_clicked();

    void on_btnAct_clicked();


private:
    Ui::MainWindow *ui;
    Start *start;
    QSqlDatabase db;
    QSqlTableModel *model;
    int currentRow;
    int countcoll;
};
#endif // MAINWINDOW_H
