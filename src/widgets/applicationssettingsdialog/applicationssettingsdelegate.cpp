/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdelegate.h"

ApplicationsSettingsDelegate::ApplicationsSettingsDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

ApplicationsSettingsDelegate::~ApplicationsSettingsDelegate() = default;

#include "moc_applicationssettingsdelegate.cpp"
