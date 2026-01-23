/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "timestampinmessagesconverter.h"
#include <QRegularExpression>

TimeStampInMessagesConverter::TimeStampInMessagesConverter() = default;

TimeStampInMessagesConverter::~TimeStampInMessagesConverter() = default;

QString TimeStampInMessagesConverter::generateTimeStamp(const QString &str) const
{
    // TODO extract date info
    // TODO use regularexpression <t:<date>:<format>>
    return {};
}
