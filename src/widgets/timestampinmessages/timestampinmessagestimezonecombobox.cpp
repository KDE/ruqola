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
    // TODO add more
}

QString TimeStampInMessagesTimeZoneComboBox::currentTimeZone() const
{
    return currentData().toString();
}
