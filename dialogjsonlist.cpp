#include "dialogjsonlist.h"
#include "ui_dialogjsonlist.h"

DialogJsonList::DialogJsonList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogJsonList)
{
    ui->setupUi(this);


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//平均分配列宽
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//设置选中单个
    ui->tableWidget->setStyleSheet("QTableWidget::item:selected { background-color: rgb(125, 125, 255) }");
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
}

DialogJsonList::~DialogJsonList()
{
    delete ui;
}

void DialogJsonList::setMainWindow(MainWindow *mainWindow)
{
   QJsonDocument qjd =   mainWindow->getJsonData();
       QJsonArray qjar = qjd.array();

        for(int i=0;i<qjar.size();i++){
            QJsonObject qjo  = qjar.at(i).toObject();
            QString if_and = qjo.value("if_and").toString();
            QString if_or = qjo.value("if_or").toString();
            QString if_exclude = qjo.value("if_exclude").toString();

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(rowCount + 1);//总行数增加1
            int rowIndex = ui->tableWidget->rowCount() - 1;

              QTableWidgetItem *kewwordItem = new QTableWidgetItem(if_and);
              kewwordItem->setTextAlignment(Qt::AlignCenter);
              kewwordItem->setFlags(kewwordItem->flags()&~Qt::ItemIsSelectable);
              ui->tableWidget->setItem(rowIndex, 0, kewwordItem);


              QTableWidgetItem *kewwordItem1 = new QTableWidgetItem(if_or);
              kewwordItem1->setTextAlignment(Qt::AlignCenter);
              kewwordItem1->setFlags(kewwordItem->flags()&~Qt::ItemIsSelectable);
              ui->tableWidget->setItem(rowIndex, 1, kewwordItem1);

              QTableWidgetItem *kewwordItem2 = new QTableWidgetItem(if_exclude);
              kewwordItem2->setTextAlignment(Qt::AlignCenter);
              kewwordItem2->setFlags(kewwordItem->flags()&~Qt::ItemIsSelectable);
              ui->tableWidget->setItem(rowIndex, 2, kewwordItem2);

        }

}

void DialogJsonList::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    int row_index = item->row();
    QString if_and = ui->tableWidget->item(row_index , 0)->text();
    QString if_or = ui->tableWidget->item(row_index , 1)->text();
    QString if_exclude = ui->tableWidget->item(row_index , 2)->text();

    emit andOrExcudeUpdated(if_and, if_or, if_exclude);
    this->close();
}




