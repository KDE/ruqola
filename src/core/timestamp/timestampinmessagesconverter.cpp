/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "timestampinmessagesconverter.h"
#include <QRegularExpression>
using namespace Qt::Literals::StringLiterals;

TimeStampInMessagesConverter::TimeStampInMessagesConverter() = default;

TimeStampInMessagesConverter::~TimeStampInMessagesConverter() = default;

QString TimeStampInMessagesConverter::generateTimeStamp(const QString &str) const
{
    if (str.isEmpty()) {
        return {};
    }

    static QRegularExpression reg(u"<t:([^>]*?)(?::([tTdDFfR]))?>"_s);

    QRegularExpressionMatchIterator roomIterator = reg.globalMatch(str);
    while (roomIterator.hasNext()) {
        const QRegularExpressionMatch match = roomIterator.next();
        const QStringView format = match.captured(1);
        const QStringView word = match.captured(2);
        qDebug() << "word " << word << " format " << format;
    }

    // TODO
    // TODO extract date info
    // TODO use regularexpression <t:<date>:<format>>
    return {};
}
