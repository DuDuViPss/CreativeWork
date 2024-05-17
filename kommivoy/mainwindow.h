#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void NewSearch();
    void Hider();
    void Shower();

private slots:
    void on_go_clicked();
    void on_gotab_clicked();
    void on_build_clicked();
    void on_gotab_pressed();
    void on_build_pressed();
    void on_go_pressed();




private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent*) override;
};
#endif // MAINWINDOW_H
