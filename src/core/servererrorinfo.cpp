/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfo.h"

qint64 ServerErrorInfo::identifierId = 0;
ServerErrorInfo::ServerErrorInfo()
{
    createUniqueIdentifier();
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
    mDateTime = newDateTime;
}

QString ServerErrorInfo::identifier() const
{
    return mIdentifier;
}

void ServerErrorInfo::createUniqueIdentifier()
{
    identifierId++;
    mIdentifier = QString::number(identifierId);
}
