#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include<QDebug>
#include <QString>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRecord>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <color.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Главное меню");
    resize(QDesktopWidget().availableGeometry(this).size() * 0.8);
    this->setWindowIcon(QIcon(":/icon/Новая папка/coin.png"));
    start = new Start(this);
    connect(start,&Start::mainwin,this,&MainWindow::show);
    start->show();

    db = QSqlDatabase :: addDatabase("QSQLITE");
    db.setDatabaseName("./data.db");
    if(db.open()){
        qInfo()<< "Вы успешно подключились к базе данных: " << db.databaseName();
        ui->statusbar->showMessage("База данных: "+ db.databaseName());
        model= new QSqlTableModel(this,db);
        SubHider();

    }
    else{
        QString err = db.lastError().databaseText();
        qCritical()<<"При подключении базы данных возникла ошибка: "<<err;

    }
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::SubHider(){
    model->setTable("Residents");
    model->select();
    model->setHeaderData(1,Qt::Horizontal,"Фамилия",Qt::DisplayRole);
    model->setHeaderData(2,Qt::Horizontal,"Имя",Qt::DisplayRole);
    model->setHeaderData(3,Qt::Horizontal,"Отчество",Qt::DisplayRole);
    model->setHeaderData(4,Qt::Horizontal,"Комната",Qt::DisplayRole);
    model->setHeaderData(5,Qt::Horizontal,"Часы отработок",Qt::DisplayRole);
    model->setHeaderData(6,Qt::Horizontal,"Дополнительная информация",Qt::DisplayRole);
    ui->table->setModel(model);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table->setColumnHidden(0,true);

    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setSortingEnabled(true);
    int columnCount = model->record().count(); // Получаем количество столбцов
    countcoll = columnCount;
    countcoll -= 6;
    for(int i = 7; i < 7+countcoll; i++){
        ui->table->setColumnHidden(i,true);
    }
    ui->btnSubAdd->hide();
    ui->btnSubDel->hide();
    ui->btnAdd->show();
    ui->btnRemove->show();

    QFont font = ui->table->font();
        font.setPointSize(16);
        ui->table->setFont(font);
}


void MainWindow::SubShower(){
    ui->table->setColumnHidden(6, true);
    ui->table->setColumnHidden(5, true);
    ui->table->setColumnHidden(3, true);
    for(int i = 7; i < 7+countcoll; i++){
        ui->table->setColumnHidden(i,false);
    }
    ui->table->setSelectionBehavior(QAbstractItemView::SelectColumns);

    ui->btnSubAdd->show();
    ui->btnSubDel->show();
    ui->btnAdd->hide();
    ui->btnRemove->hide();

}

void MainWindow::on_btnAdd_clicked()
{
    model->insertRow(model->rowCount());
}


void MainWindow::on_btnRemove_clicked()
{
    // Показываем диалоговое окно с подтверждением действия
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите удалить эту запись?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Удаляем строку из модели и обновляем ее
        model->removeRow(currentRow);
        model->select();
    }
}

void MainWindow::on_table_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void MainWindow::on_btnExit_clicked()
{
    start->show();
    this->hide();
}


void MainWindow::on_remove_clicked()
{
    model->select();
    ui->table->selectRow(currentRow);
}


void MainWindow::on_btnSub_clicked()
{
    if (ui->btnSub->text() == "Показать субботники") {
        ui->btnSub->setText("Скрыть субботники");
        SubShower();
    }
     else {
        // Установить текст кнопки "Показать субботники"
        ui->btnSub->setText("Показать субботники");

        SubHider();
    }
}

void MainWindow::on_table_doubleClicked(const QModelIndex &index)
{
    int subCol = index.column();
    if (subCol >= 7) { // Проверяем, что клик произошел в столбце с субботниками
        QString currentValue = index.data(Qt::DisplayRole).toString();
        QString newValue = (currentValue == "+") ? "-" : "+"; // Переключаем значение
        model->setData(index, newValue, Qt::EditRole); // Обновляем значение в модели данных
    }
}



void MainWindow::on_btnSubAdd_clicked()
{
    // Запрос даты у пользователя
    QString date = QInputDialog::getText(this, tr("Добавление нового столбца"), tr("Введите дату (дд/мм/гггг):"));

    // Проверка, что пользователь ввел дату
    if (date.isEmpty()) {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Дата не введена."));
        return;
    }

    // Выполнение запроса на добавление нового столбца в базу данных
    QSqlQuery query;
    QString addColumnQuery = QString("ALTER TABLE Residents ADD COLUMN '%1' TEXT").arg(date);

    if (query.exec(addColumnQuery)) {
        // Если запрос выполнен успешно, обновляем отображение модели
        qInfo() << "Столбец успешно добавлен: " << date;

        // Обновляем отображение модели
        model->select();

        // Увеличиваем значение countcoll
        countcoll++;

        // Переопределяем заголовок нового столбца
        int columnIndex = 7 + countcoll - 1; // Индекс нового столбца
        ui->table->setColumnHidden(columnIndex, false);
        model->setHeaderData(columnIndex, Qt::Horizontal, date, Qt::DisplayRole);
    } else {
        // Если запрос не удалось выполнить, выводим сообщение об ошибке
        qCritical() << "Ошибка при добавлении столбца: " << query.lastError().text();
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось добавить столбец в базу данных."));
    }
    SubHider();
    SubShower();
}


void MainWindow::on_btnSubDel_clicked()
{
    // Получаем индекс столбца субботника
    int columnIndexToRemove = ui->table->currentIndex().column();
    if (columnIndexToRemove < 7 || columnIndexToRemove >= 7 + countcoll) {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Выберите столбец субботника для удаления."));
        return;
    }

    // Получаем название столбца субботника
    QString columnName = model->headerData(columnIndexToRemove, Qt::Horizontal, Qt::DisplayRole).toString();

    // Показываем диалоговое окно с подтверждением действия
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Подтверждение"), tr("Вы уверены, что хотите удалить выбранный субботник?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    // Выполняем запрос на удаление столбца из базы данных
    QSqlQuery query;
    QString removeColumnQuery = QString("ALTER TABLE Residents DROP COLUMN '%1'").arg(columnName);
    if (!query.exec(removeColumnQuery)) {
        qCritical() << "Ошибка при удалении столбца из базы данных: " << query.lastError().text();
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось удалить столбец из базы данных."));
        return;
    }

    // Обновляем модель данных и отображение таблицы
    model->select();
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Уменьшаем значение countcoll
    countcoll--;

    // Скрываем удаленный столбец
    ui->table->setColumnHidden(columnIndexToRemove, true);
    model->select();
}


void MainWindow::on_btnAtt_clicked()
{
    if (ui->btnAtt->text() == "Показать аттестацию") {
        ui->btnAtt->setText("Скрыть аттестацию");

        MultiColorDelegate *delegate = new MultiColorDelegate;
        for(int i = 0; i < model->rowCount(); i++) {
            int totalHours = 0;
            for (int j = 7; j < model->columnCount(); j++) {
                QModelIndex index = ui->table->model()->index(i, j); // Индекс элемента в столбце j
                QString data = ui->table->model()->data(index).toString();
                totalHours += data.count('-') * 5; // Учитываем количество "+" в ячейке
            }
            QModelIndex index = ui->table->model()->index(i, 5); // Индекс элемента в 5 столбце
            QVariant data = ui->table->model()->data(index);
            int value = data.toInt() + totalHours; // Учитываем общее количество часов и "+" во всех столбцах
            if (value >= 15) {
                delegate->setRowColor(i, Qt::red);
            } else {
                delegate->setRowColor(i, Qt::green);
            }
        }
        ui->table->setItemDelegate(delegate);
        ui->table->setStyleSheet("QTableView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5, stop: 0 #647DEE, stop: 1 #7F53AC); }");
    } else {
        ui->btnAtt->setText("Показать аттестацию");

        ui->table->setItemDelegate(new QStyledItemDelegate);

        ui->table->viewport()->setAutoFillBackground(true);
        ui->table->viewport()->setBackgroundRole(QPalette::Base);
        ui->table->viewport()->setPalette(QPalette(Qt::white));
        ui->table->setStyleSheet("color: black;");
        ui->table->setStyleSheet("QTableView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5, stop: 0 #647DEE, stop: 1 #7F53AC); }");
    }
}




void MainWindow::on_btnAct_clicked()
{
    // Проверяем, что текущая строка выбрана
    if (currentRow == -1) {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Не выбрана ни одна строка."));
        return;
    }

    // Получаем значение из комбобокса
    int comboBoxIndex = ui->comboBox->currentIndex();

    // Получаем индекс столбца с часами
    QModelIndex hoursIndex = model->index(currentRow, 5);

    // Получаем текущее значение часов
    int currentValue = model->data(hoursIndex, Qt::EditRole).toInt();

    // Определяем, на сколько изменить текущее значение в зависимости от выбранного индекса
    int newValue = currentValue;
    switch(comboBoxIndex) {
        case 0:
            newValue += 5;
            break;
        case 1:
            newValue += 10;
            break;
        case 2:
        case 3:
            newValue += 5;
            break;
        case 4:
            newValue -= 1;
            break;
        case 5:
            newValue -= 2;
            break;
        case 6:
            newValue -= 5;
            break;
        case 7:
            newValue -= 3;
            break;
    }

    // Значение часов не может быть меньше 0
    if (newValue < 0) {
        newValue = 0;
    }

    // Устанавливаем новое значение в модели
    model->setData(hoursIndex, newValue, Qt::EditRole);

    // Применяем все изменения к базе данных
    if (!model->submitAll()) {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось сохранить изменения в базе данных."));
    }
}



