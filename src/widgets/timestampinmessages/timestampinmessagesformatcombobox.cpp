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
    addItem(i18n("Short Time"), u"t"_s);
    addItem(i18n("Long Time"), u"T"_s);
    addItem(i18n("Short Date"), u"d"_s);
    addItem(i18n("Long Date"), u"D"_s);
    addItem(i18n("Full Date and Time"), u"f"_s);
    addItem(i18n("Full Date and Time (long)"), u"F"_s);
    addItem(i18n("Relative time"), u"R"_s);
}

QString TimeStampInMessagesFormatComboBox::currentFormat() const
{
    return currentData().toString();
}

#include "moc_timestampinmessagesformatcombobox.cpp"
