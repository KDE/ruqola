/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasetimestampdelegate.h"

#include <QDateTime>

ExploreDatabaseTimeStampDelegate::ExploreDatabaseTimeStampDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

ExploreDatabaseTimeStampDelegate::~ExploreDatabaseTimeStampDelegate() = default;

QString ExploreDatabaseTimeStampDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    const qint64 timeStamp = value.toLongLong();
    return locale.toString(QDateTime::fromMSecsSinceEpoch(timeStamp), QLocale::LongFormat);
}

#include "moc_exploredatabasetimestampdelegate.cpp"
