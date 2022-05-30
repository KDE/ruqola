/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QCheckBox>
#include <QString>
#include <QVariant>

namespace SettingsWidgetHelper
{
QString widgetSettingsName(QWidget *widget)
{
    if (!widget) {
        return {};
    }
    return widget->property("settings_name").toString();
}
}
