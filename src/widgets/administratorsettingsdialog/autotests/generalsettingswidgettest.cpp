/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidgettest.h"
#include "administratorsettingsdialog/general/generalsettingswidget.h"
#include <QTest>
QTEST_MAIN(GeneralSettingsWidgetTest)
GeneralSettingsWidgetTest::GeneralSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void GeneralSettingsWidgetTest::shouldHaveDefaultValues()
{
    GeneralSettingsWidget w(nullptr);
    // TODO
}
