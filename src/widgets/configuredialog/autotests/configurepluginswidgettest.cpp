/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurepluginswidgettest.h"
#include "configuredialog/configurepluginswidget.h"
#include <QTest>
QTEST_MAIN(ConfigurePluginsWidgetTest)

ConfigurePluginsWidgetTest::ConfigurePluginsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigurePluginsWidgetTest::shouldHaveDefaultValues()
{
    ConfigurePluginsWidget w;
    // TODO
}

#include "moc_configurepluginswidgettest.cpp"
