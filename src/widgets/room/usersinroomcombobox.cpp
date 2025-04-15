/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomcombobox.h"
#include "model/usersforroomfilterproxymodel.h"
#include <KLocalizedString>

UsersInRoomComboBox::UsersInRoomComboBox(bool hasOwnerSupport, QWidget *parent)
    : QComboBox(parent)
{
    fillCombobox(hasOwnerSupport);
}

UsersInRoomComboBox::~UsersInRoomComboBox() = default;

void UsersInRoomComboBox::fillCombobox(bool hasOwnerSupport)
{
    addItem(i18n("All"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::All));
    addItem(QIcon::fromTheme(QStringLiteral("user-online")), i18n("Connected"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Online));
    addItem(QIcon::fromTheme(QStringLiteral("user-offline")), i18n("Disconnected"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Offline));
    addItem(QIcon::fromTheme(QStringLiteral("user-away")), i18n("Away"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Away));
    addItem(QIcon::fromTheme(QStringLiteral("user-busy")), i18n("Busy"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Busy));
    if (hasOwnerSupport) {
        addItem(i18n("Owners"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Owners));
    }
}

#include "moc_usersinroomcombobox.cpp"
