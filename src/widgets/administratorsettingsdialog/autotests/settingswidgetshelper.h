/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QCheckBox>
#include <QLabel>
#include <QString>
#include <QTest>
#include <QToolButton>

namespace SettingsWidgetHelper
{
QString widgetSettingsName(QWidget *widget)
{
    if (!widget) {
        return {};
    }
    return widget->property("settings_name").toString();
}

void checkLabelToolButton(QWidget *widget, const QString &variableName)
{
    auto toolButton = widget->findChild<QToolButton *>(QStringLiteral("toolbutton_%1").arg(variableName));
    QVERIFY(toolButton);
    QVERIFY(!toolButton->isEnabled());

    auto label = widget->findChild<QLabel *>(QStringLiteral("label_%1").arg(variableName));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}
}
