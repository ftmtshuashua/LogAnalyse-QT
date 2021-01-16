#pragma once

#include <QObject>
#include "Utils.h"

QString From8Bit(const QByteArray& str) {
	return QString::fromLocal8Bit(str);
}
//过滤算法
bool isContainsAnd(QString& data, QString& compare)
{
	if (data.isEmpty()) return false;
	if (compare.isEmpty())return true;

	QStringList list = compare.split(";");
	for (int i = 0; i < list.size(); i++) {
		QString str = list.at(i);
		if (!data.contains(str)) {
			return false;
		}
	}
	return true;
}
//过滤算法
bool isContainsOr(QString& data, QString& compare)
{
	if (data.isEmpty()) return false;
	if (compare.isEmpty())return true;

	QStringList list = compare.split(";");
	for (int i = 0; i < list.size(); i++) {
		QString str = list.at(i);
		if (data.contains(str)) {
			return true;
		}
	}
	return false;

}


//高亮显示
QString getBrightString(QString& data, QString& compare)
{

	QStringList list = compare.split(";");
	for (int i = 0; i < list.size(); i++) {
		QString item = list.at(i);
		if (!item.isEmpty() && data.contains(item)) {
			//data = data.replace(item, QObject::tr("<labe style=\"color:red;\">%1</labe>").arg(item));
			data = data.replace(item, QObject::tr("<font color=\"red\">%1</font>").arg(item));
		}
	}
	//    return tr("<html>%1</html>").arg(data);
	return data;
}