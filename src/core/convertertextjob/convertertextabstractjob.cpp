/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "convertertextabstractjob.h"

ConverterTextAbstractJob::ConverterTextAbstractJob(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAcount(account)
{
}

ConverterTextAbstractJob::~ConverterTextAbstractJob() = default;

QString ConverterTextAbstractJob::messageId() const
{
    return mMessageId;
}

void ConverterTextAbstractJob::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

RocketChatAccount *ConverterTextAbstractJob::rocketChatAcount() const
{
    return mRocketChatAcount;
}
