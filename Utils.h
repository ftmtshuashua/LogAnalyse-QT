#pragma once

#include <QString>
#include <QStringList>


//UI±‡¬Î
QString From8Bit(const QByteArray& str);

//À„∑®
bool isContainsAnd(QString& data, QString& compare);
bool isContainsOr(QString& data, QString& compare);


QString getBrightString(QString& data, QString& compare);