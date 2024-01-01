/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolescopecombobox.h"
#include <KLocalizedString>

RoleScopeComboBox::RoleScopeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem(i18n("Rooms"), QStringLiteral("Subscriptions"));
    addItem(i18n("Global"), QStringLiteral("Users"));
}

RoleScopeComboBox::~RoleScopeComboBox() = default;

#include "moc_rolescopecombobox.cpp"
