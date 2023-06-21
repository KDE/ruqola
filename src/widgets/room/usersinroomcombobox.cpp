/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomcombobox.h"
#include <KLocalizedString>

UsersInRoomComboBox::UsersInRoomComboBox(QWidget *parent)
    : QComboBox(parent)
{
    fillCombobox();
}

UsersInRoomComboBox::~UsersInRoomComboBox() = default;

void UsersInRoomComboBox::fillCombobox()
{
    addItem(i18n("All"), QString());
    addItem(i18n("Connected"), QStringLiteral("online"));
    addItem(i18n("Disconnected"), QStringLiteral("offline"));
    addItem(i18n("Away"), QStringLiteral("away"));
    addItem(i18n("Busy"), QStringLiteral("busy"));
}

#include "moc_usersinroomcombobox.cpp"
