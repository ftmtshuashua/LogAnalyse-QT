#pragma once

//#include "Utils.hpp"
#include <QThread>
#include <qDebug>
#include "Utils.h"
#include <QTime>
#include <QFile>
#include <QList>
#include <QTextCodec>

class AnalyseThread : public QThread
{
	Q_OBJECT

public:
	AnalyseThread(QObject* parent = nullptr) {

	}

	//设置文件地址
	void setFilePath(QString path) {
		mPath = path;
	}

	//设置判断条件
	void setIf(QString code, QString If_And, QString If_Or, QString If_Exclude) {
		AnalyseThread::If_And = If_And;
		AnalyseThread::If_Or = If_Or;
		AnalyseThread::If_Exclude = If_Exclude;
		AnalyseThread::Code = code;
	}

	//主动停止线程
	void setStop() {
		isRuning = false;
	}

signals:
	void on_analyse_runing(const int& count, const long& elapsed); //运行中
	void on_analyse_end(const int& count, const long& elapsed);  //当线程执行结束时
	void on_analyse_log(const QList<QString>& log); //当匹配到一条日志的时候

protected:
	//QThread的虚函数
	//线程处理函数
	//不能直接调用，通过start()间接调用
	void run() {
		//数据行数
		int count_line = 0;
		//开始时间
		QTime startTime = QTime::currentTime();


		QFile file(mPath);
		if (file.open(QIODevice::ReadOnly)) {

			//读取缓存
			QList<QString> log_array;

			//用于存放文件中读取的数据
			QByteArray array;

			//高亮的文案
			QString bright = If_And + ";" + If_Or;

			QTextCodec* mCodec = QTextCodec::codecForName(Code.toStdString().c_str());
			QTime time_segment = QTime::currentTime();

			while (!(array = file.readLine()).isEmpty() && isRuning) {


				QString data_trimmed = mCodec->toUnicode(array);
				//QString data_trimmed(array);

				if (!If_And.isEmpty() && !isContainsAnd(data_trimmed, If_And)) continue;
				if (!If_Or.isEmpty() && !isContainsOr(data_trimmed, If_Or)) continue;
				if (!If_Exclude.isEmpty() && isContainsOr(data_trimmed, If_Exclude)) continue;

				log_array.append(getBrightString(data_trimmed, bright));

				count_line++;

				int elapsed = time_segment.msecsTo(QTime::currentTime()); //耗时 ms
				if (elapsed >= 13) {
					on_analyse_runing(count_line, startTime.msecsTo(QTime::currentTime()));
					time_segment = QTime::currentTime();
					msleep(1);
				}
			}
			file.close();
			if (isRuning && !log_array.isEmpty()) {
				//on_analyse_log(log_array.join("<br/>"));
				on_analyse_log(log_array);
				on_analyse_end(count_line, startTime.msecsTo(QTime::currentTime()));
			}
		}

	}

private:
	bool isRuning = true;
	QString mPath;
	QString If_And;
	QString If_Or;
	QString If_Exclude;
	QString Code;


};

