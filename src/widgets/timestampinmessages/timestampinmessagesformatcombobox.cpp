/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesformatcombobox.h"
#include "timestamp/timestampinmessagesconverter.h"

#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesFormatComboBox::TimeStampInMessagesFormatComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
}

TimeStampInMessagesFormatComboBox::~TimeStampInMessagesFormatComboBox() = default;

void TimeStampInMessagesFormatComboBox::initialize()
{
    const QDateTime dt(QDate(2026, 02, 12), QTime(7, 8, 36));
    addItem(i18n("Short Time (%1)", TimeStampInMessagesConverter::convertTimeStamp(dt, TimeStampInMessagesUtils::FormatType::ShortTime, false)),
            QVariant::fromValue(TimeStampInMessagesUtils::FormatType::ShortTime));
    addItem(i18n("Long Time (%1)", TimeStampInMessagesConverter::convertTimeStamp(dt, TimeStampInMessagesUtils::FormatType::LongTime, false)),
            QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongTime));
    addItem(i18n("Short Date (%1)", TimeStampInMessagesConverter::convertTimeStamp(dt, TimeStampInMessagesUtils::FormatType::ShortDate, false)),
            QVariant::fromValue(TimeStampInMessagesUtils::FormatType::ShortDate));
    addItem(i18n("Long Date (%1)", TimeStampInMessagesConverter::convertTimeStamp(dt, TimeStampInMessagesUtils::FormatType::LongDate, false)),
            QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongDate));
    addItem(i18n("Full Date and Time (%1)", TimeStampInMessagesConverter::convertTimeStamp(dt, TimeStampInMessagesUtils::FormatType::FullDateTime, false)),
            QVariant::fromValue(TimeStampInMessagesUtils::FormatType::FullDateTime));
    addItem(i18n("Full Date and Time (long) (%1)",
                 TimeStampInMessagesConverter::convertTimeStamp(dt, TimeStampInMessagesUtils::FormatType::LongFullDateTime, false)),
            QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongFullDateTime));
    addItem(i18n("Relative time (%1)", TimeStampInMessagesConverter::convertTimeStamp(dt, TimeStampInMessagesUtils::FormatType::RelativeTime, false)),
            QVariant::fromValue(TimeStampInMessagesUtils::FormatType::RelativeTime));
}

TimeStampInMessagesUtils::FormatType TimeStampInMessagesFormatComboBox::currentFormat() const
{
    return currentData().value<TimeStampInMessagesUtils::FormatType>();
}

#include "moc_timestampinmessagesformatcombobox.cpp"
