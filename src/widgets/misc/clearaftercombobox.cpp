/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clearaftercombobox.h"
#include <KLocalizedString>

ClearAfterCombobox::ClearAfterCombobox(QWidget *parent)
    : QComboBox(parent)
{
    init();
    setSizeAdjustPolicy(QComboBox::AdjustToContents);

    // TODO connect to activated
}

ClearAfterCombobox::~ClearAfterCombobox() = default;

void ClearAfterCombobox::init()
{
    addItem(i18n("Do Not Clear"), QVariant::fromValue(ClearAfterCombobox::Expire::DontClear));
    addItem(i18n("30 Minutes"), QVariant::fromValue(ClearAfterCombobox::Expire::HalfHour));
    addItem(i18n("1 Hour"), QVariant::fromValue(ClearAfterCombobox::Expire::OneHour));
    addItem(i18n("Custom…"), QVariant::fromValue(ClearAfterCombobox::Expire::Custom));
}
#include "moc_clearaftercombobox.cpp"
