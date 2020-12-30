#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogjsonlist.h"
#include <QDebug>

//--------------------

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    jsonpath = qApp->applicationDirPath() + QDir::separator() + "1.json";
    mDialogJsonList = nullptr;



    ui->if_and->setPlaceholderText("必须包含的数据，以;号分割");
    ui->if_or->setPlaceholderText("只要包含其中之一，以;号分割");
    ui->if_exclude->setPlaceholderText("不能包含的数据，以;号分割");
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isContainsAnd(QString &data, QString &compare)
{
    if(data.isEmpty()) return false;
    if(compare.isEmpty())return true;

        QStringList list = compare.split(";");
        for (int i=0;i<list.size() ;i++ ) {
            QString str = list.at(i);
            if(!data.contains(str)){
                return false;
            }
        }
        return true;
}

bool MainWindow::isContainsOr(QString &data, QString &compare)
{
    if(data.isEmpty()) return false;
    if(compare.isEmpty())return true;

    QStringList list = compare.split(";");
    for (int i=0;i<list.size() ;i++ ) {
        QString str = list.at(i);
        if(data.contains(str)){
            return true;
        }
    }
    return false;

}

QString MainWindow::setBright(QString &data, QString &compare)
{

    QStringList list = compare.split(";");
    for (int i=0;i<list.size() ;i++ ) {
        QString item = list.at(i);
        if(!item.isEmpty() && data.contains(item)){
            qDebug()<<"包含:"+item;
           data =  data.replace(item ,tr("<labe style=\"color:red;\">%1</labe>").arg(item));
        }
    }
//    return tr("<html>%1</html>").arg(data);
    return data;
}

//选择文件
void MainWindow::on_pushButton_preview_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开日志文件"),".",tr("All files(*.*)"));
    if (!fileName.isEmpty()) {
       ui->lineEdit_FilePath->setText(fileName);
    }
}

/**
 开始分析
 * @brief MainWindow::on_pushButton_Analyse_clicked
 */
void MainWindow::on_pushButton_Analyse_clicked()
{
    QString fielpath = ui->lineEdit_FilePath->text();
    if(fielpath.isEmpty()) return;
    QFile file(fielpath);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray array;
        ui->plainTextEdit_Result->clear();

        QString if_and = ui->if_and->text();
        QString if_or = ui->if_or->text();
        QString if_exclude = ui->if_exclude->text();

        QString CheckCode = ui->comboBox_code->currentText();
        QTextCodec *mCodec = QTextCodec::codecForName( CheckCode.toStdString().c_str());
        int count=0;
        while (!(array = file.readLine()).isEmpty()) {

            QString data_trimmed =mCodec->toUnicode(array);

            if(!if_and.isEmpty() && !isContainsAnd(data_trimmed , if_and)) continue;
            if(!if_or.isEmpty() &&!isContainsOr(data_trimmed,if_or)) continue;
            if(!if_exclude.isEmpty() &&isContainsOr(data_trimmed,if_exclude)) continue;


            QString parms = if_and +";"+ if_or;
            ui->plainTextEdit_Result->appendHtml(setBright( data_trimmed , parms));
            // ui->plainTextEdit_Result->appendPlainText(array);

            count++;
        }

        ui->plainTextEdit_Result->appendPlainText(tr("共 %1 行").arg(count));
        file.close();
    }
}




//------------------------------------




//保存
void MainWindow::on_pushButton_save_clicked()
{
   QJsonDocument qjd =  getJsonData();
    QJsonArray mJsonArray = qjd.array();
    QJsonObject objs;
    objs.insert("if_and" , ui->if_and->text());
    objs.insert("if_or" , ui->if_or->text());
    objs.insert("if_exclude" , ui->if_exclude->text());
    mJsonArray.insert(0,objs );
    qjd.setArray( mJsonArray);
    setJsonData(qjd);
}

//打开
void MainWindow::on_pushButton_open_clicked()
{
//    QJsonDocument qjd =  getJsonData();
//    qjd.array();

    if (mDialogJsonList) {
        delete mDialogJsonList;
    }
    mDialogJsonList = new DialogJsonList();
    connect(mDialogJsonList, &DialogJsonList::andOrExcudeUpdated, this, &MainWindow::andOrExcudeUpdated);
    mDialogJsonList->setMainWindow(this);
    mDialogJsonList->exec();
}

void MainWindow::andOrExcudeUpdated(QString if_and, QString if_or, QString if_excude)
{
    ui->if_and->setText(if_and);
    ui->if_or->setText(if_or);
    ui->if_exclude->setText(if_excude);

    on_pushButton_Analyse_clicked();
}

QJsonDocument MainWindow::getJsonData() const
{
    QFile file(jsonpath);
    if(file.open(QIODevice::ReadOnly)){
        QJsonParseError * error = new QJsonParseError;
        QJsonDocument jdc = QJsonDocument::fromJson(file.readAll(), error);
        file.close();
        return jdc;
    }

   QJsonDocument qjd = QJsonDocument();
    return qjd;
}

void MainWindow::setJsonData(const QJsonDocument &document)
{
 QFile file(jsonpath);
 qDebug() << jsonpath;
if (file.open(QIODevice::WriteOnly)) {
        file.write(document.toJson(QJsonDocument::Compact));
        file.close();
}
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    qDebug() << fileName;
    if (fileName.isEmpty()) {
        return;
    }
    ui->lineEdit_FilePath->setText(fileName);
}



