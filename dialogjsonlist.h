#ifndef DIALOGJSONLIST_H
#define DIALOGJSONLIST_H

#include <QDialog>
#include "mainwindow.h"
#include <QTableWidgetItem>

namespace Ui {
class DialogJsonList;
}

class MainWindow;

class DialogJsonList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogJsonList(QWidget *parent = nullptr);
    ~DialogJsonList();


    void setMainWindow(MainWindow *mainWindow);

signals:
    void andOrExcudeUpdated(QString if_and, QString if_or, QString if_excude);

private slots:
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::DialogJsonList *ui;
};

#endif // DIALOGJSONLIST_H
