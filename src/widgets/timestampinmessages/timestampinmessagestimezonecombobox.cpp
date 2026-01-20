/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagestimezonecombobox.h"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;

TimeStampInMessagesTimeZoneComboBox::TimeStampInMessagesTimeZoneComboBox(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
}

TimeStampInMessagesTimeZoneComboBox::~TimeStampInMessagesTimeZoneComboBox() = default;

void TimeStampInMessagesTimeZoneComboBox::initialize()
{
    addItem(i18n("Locale"), QString());
    for (int i = -12; i <= 12; ++i) {
        addItem(u"UTC%1"_s.arg(QString::number(i)), u"%1:00"_s.arg(QString::number(i)));
    }
}

QString TimeStampInMessagesTimeZoneComboBox::currentTimeZone() const
{
    return currentData().toString();
}

#include "moc_timestampinmessagestimezonecombobox.cpp"
