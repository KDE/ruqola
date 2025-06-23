/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolescopecombobox.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

RoleScopeComboBox::RoleScopeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem(i18n("Rooms"), u"Subscriptions"_s);
    addItem(i18n("Global"), u"Users"_s);
}

RoleScopeComboBox::~RoleScopeComboBox() = default;

#include "moc_rolescopecombobox.cpp"
