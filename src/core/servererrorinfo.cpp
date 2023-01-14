/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfo.h"

ServerErrorInfo::ServerErrorInfo() = default;

ServerErrorInfo::~ServerErrorInfo() = default;

QDebug operator<<(QDebug d, const ServerErrorInfo &t)
{
    d << " Account Name " << t.accountName();
    d << " Message " << t.message();
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
