/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingssettingswidgettest.h"
#include "applicationssettingsdialog/applicationssettingssettingswidget.h"

#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ApplicationsSettingsSettingsWidgetTest)
ApplicationsSettingsSettingsWidgetTest::ApplicationsSettingsSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsSettingsWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsSettingsWidget d(nullptr);

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
}

#include "moc_applicationssettingssettingswidgettest.cpp"
