/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelscombobox.h"
#include <KLocalizedString>
TeamChannelsComboBox::TeamChannelsComboBox(QWidget *parent)
    : QComboBox(parent)
{
    init();
}

TeamChannelsComboBox::~TeamChannelsComboBox() = default;

void TeamChannelsComboBox::init()
{
    addItem(i18n("All"));
    addItem(i18n("Autojoin"));
}
