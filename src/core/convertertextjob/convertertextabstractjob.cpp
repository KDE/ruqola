/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "convertertextabstractjob.h"

ConverterTextAbstractJob::ConverterTextAbstractJob(QObject *parent)
    : QObject(parent)
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
