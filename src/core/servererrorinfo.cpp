/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfo.h"

#include <QLocale>

quint64 ServerErrorInfo::identifierId = 0;
ServerErrorInfo::ServerErrorInfo()
{
    createUniqueIdentifier();
    setDateTime(QDateTime::currentDateTime());
}

ServerErrorInfo::~ServerErrorInfo() = default;

QDebug operator<<(QDebug d, const ServerErrorInfo &t)
{
    d << " Account Name " << t.accountName();
    d << " Message " << t.message();
    d << " DateTime " << t.dateTime();
    d << " identifier " << t.identifier();
    return d;
}

QString ServerErrorInfo::accountName() const
{
    return mAccountName;
}

void ServerErrorInfo::setAccountName(const QString &newAccountName)
{
    mAccountName = newAccountName;
}

QString ServerErrorInfo::message() const
{
    return mMessage;
}

void ServerErrorInfo::setMessage(const QString &newMessage)
{
    mMessage = newMessage;
}

QDateTime ServerErrorInfo::dateTime() const
{
    return mDateTime;
}

void ServerErrorInfo::setDateTime(const QDateTime &newDateTime)
{
    if (mDateTime != newDateTime) {
        mDateTime = newDateTime;
        QLocale l;
        mDateTimeStr = l.toString(mDateTime, QLocale::ShortFormat);
    }
}

QByteArray ServerErrorInfo::identifier() const
{
    return mIdentifier;
}

void ServerErrorInfo::createUniqueIdentifier()
{
    identifierId++;
    mIdentifier = QByteArray::number(identifierId);
}

QString ServerErrorInfo::dateTimeStr() const
{
    return mDateTimeStr;
}

#include "moc_servererrorinfo.cpp"
