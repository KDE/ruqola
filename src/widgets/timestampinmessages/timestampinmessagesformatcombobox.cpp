/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesformatcombobox.h"

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
    addItem(i18n("Short Time"), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::ShortTime));
    addItem(i18n("Long Time"), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongTime));
    addItem(i18n("Short Date"), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::ShortDate));
    addItem(i18n("Long Date"), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongDate));
    addItem(i18n("Full Date and Time"), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::FullDateTime));
    addItem(i18n("Full Date and Time (long)"), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongFullDateTime));
    addItem(i18n("Relative time"), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::RelativeTime));
}

TimeStampInMessagesUtils::FormatType TimeStampInMessagesFormatComboBox::currentFormat() const
{
    return currentData().value<TimeStampInMessagesUtils::FormatType>();
}

#include "moc_timestampinmessagesformatcombobox.cpp"
