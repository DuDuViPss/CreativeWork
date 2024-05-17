#include "start.h"
#include "ui_start.h"
#include <QString>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QIcon>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
Start::Start(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.3);
    this->setWindowTitle("Авторизация");
    this->setWindowIcon(QIcon(":/icon/Новая папка/Login_37128.png"));
    QPixmap pix(":/icon/Новая папка/logo.jpg");
    int w = 180;
    int h = 180;
    ui->label_3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    // Подключение к базе данных
    db = QSqlDatabase::database();
}

Start::~Start()
{
    delete ui;
}

void Start::on_buttonBox_accepted()
{
    QString login = ui->login->text();
    QString pass = ui->pass->text();

    // Проверка логина и пароля в базе данных
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE login = :login AND password = :pass");
    query.bindValue(":login", login);
    query.bindValue(":pass", pass);
    if(query.exec() && query.next()){
        this->close();
        emit mainwin();
        QMessageBox::information(this, "Ура", "Вы успешно авторизировались");
        ui->login->clear();
        ui->pass->clear();
    }
    else{
        QMessageBox::critical(this, "Ошибка", "Неверный логин или пароль");
        ui->login->clear();
        ui->pass->clear();
    }
}

void Start::on_buttonBox_rejected()
{
    QApplication::exit();
}
