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
        QString offset;
        if (i > 0) {
            offset = QString(u"+%1"_s.arg(i, 2, 10, u'0') + u":00"_s);
        } else {
            offset = QString(u"-%1"_s.arg(i * -1, 2, 10, u'0') + u":00"_s);
        }
        addItem(u"UTC %1"_s.arg(QString::number(i)), offset);
    }
}

QString TimeStampInMessagesTimeZoneComboBox::currentTimeZone() const
{
    return currentData().toString();
}

#include "moc_timestampinmessagestimezonecombobox.cpp"
