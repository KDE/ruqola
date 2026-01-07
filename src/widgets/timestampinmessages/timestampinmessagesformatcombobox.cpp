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
    addItem(i18n("Short Time"), u"p"_s);
    addItem(i18n("Long Time"), u"pp"_s);
    addItem(i18n("Short Date"), u"P"_s);
    addItem(i18n("Long Date"), u"Pp"_s);
    addItem(i18n("Full Date and Time"), u"PPPppp"_s);
    addItem(i18n("Full Date and Time (long)"), u"PPPPpppp"_s);
    addItem(i18n("Relative time"), u"relative"_s);
}

QString TimeStampInMessagesFormatComboBox::currentFormat() const
{
    return currentData().toString();
}

#include "moc_timestampinmessagesformatcombobox.cpp"
