#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LogAnalyse.h"
#include "AnalyseThread.h"
#include "Utils.h"

#include <QString>
#include <QList>
#include <qDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QListWidgetItem>
#include <QTextCodec>
#include <QTime> 

//过滤器
class Filter {
public:
	QString text_title;
	QString text_and;
	QString text_or;
	QString text_exclude;

	Filter(QString title, QString t_and, QString t_or, QString t_exclude);

	QString toString();

	bool operator== (const Filter& other) const;

private:

};


class LogAnalyse : public QMainWindow
{
	Q_OBJECT

public:
	LogAnalyse(QWidget* parent = Q_NULLPTR);

protected:

	void writeFilterFile();
	void readFilterFile();

	void insertFilterToListWidget(int index, Filter& filter);

	void on_thread_end();

	//清理分析结果
	void clear_AnalyseResult();
	//开始分析
	void analyse(QString path, QString If_And, QString If_Or, QString If_Exclude) {
		/*if (mThread != nullptr) {
			mThread->setStop();
			delete mThread;
		}*/
		mThread->setStop();

		mThread = new AnalyseThread(this);
		connect(mThread, &AnalyseThread::on_analyse_log, this, &LogAnalyse::on_analyse_log);
		connect(mThread, &AnalyseThread::on_analyse_runing, this, &LogAnalyse::on_analyse_runing);
		connect(mThread, &AnalyseThread::on_analyse_end, this, &LogAnalyse::on_analyse_end);

		clear_AnalyseResult();

		mThread->setFilePath(path);
		mThread->setIf(ui.BT_CheckCode->currentText()  , If_And, If_Or , If_Exclude);
		mThread->start();
	}


private slots:
	void on_BT_CheckFile_clicked();
	void on_BT_Create_clicked();
	void on_BT_Analyze_clicked();
	void on_BT_Clear_clicked();
	void on_List_Filter_itemDoubleClicked(QListWidgetItem* item);

	void on_analyse_runing(const int& count, const long& elapsed); //运行中
	void on_analyse_end(const int& count, const long& elapsed);  //当线程执行结束时
	void on_analyse_log(const QString& log); //当匹配到一条日志的时候



private:
	Ui::LogAnalyseClass ui;
	QList<Filter> mFilterArray;
	QString mFilterFilePath;

	AnalyseThread* mThread;
	 
};
