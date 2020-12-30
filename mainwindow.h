#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDropEvent>
#include <QMimeData>
#include <QString>
#include <QTextCodec>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DialogJsonList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //包含
    bool isContainsAnd(QString &data,QString &compare);
    //包含
    bool isContainsOr(QString &data,QString &compare);
    //排除
    bool isExclude(QString &data,QString &compare);

    //高亮显示
    QString setBright(QString &data , QString &compare);

    void setIf(QString &if_and,QString &if_or ,QString &if_E);



    QJsonDocument getJsonData() const;
    void setJsonData(const QJsonDocument &jsonData);


    void dropEvent(QDropEvent *event);


private slots:
    void on_pushButton_preview_clicked();

    void on_pushButton_Analyse_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_open_clicked();

    void andOrExcudeUpdated(QString if_and, QString if_or, QString if_excude);

private:
    Ui::MainWindow *ui;
    DialogJsonList * mDialogJsonList;
    QString jsonpath;
};


#endif // MAINWINDOW_H

