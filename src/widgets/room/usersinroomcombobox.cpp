/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomcombobox.h"
using namespace Qt::Literals::StringLiterals;

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
    insertSeparator(1);
    addItem(QIcon::fromTheme(u"user-online"_s), i18n("Connected"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Online));
    addItem(QIcon::fromTheme(u"user-offline"_s), i18n("Disconnected"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Offline));
    addItem(QIcon::fromTheme(u"user-away"_s), i18n("Away"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Away));
    addItem(QIcon::fromTheme(u"user-busy"_s), i18n("Busy"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Busy));
    insertSeparator(6);
    if (hasOwnerSupport) {
        addItem(i18n("Owners"), QVariant::fromValue(UsersForRoomFilterProxyModel::FilterUserType::Owners));
    }
}

#include "moc_usersinroomcombobox.cpp"
