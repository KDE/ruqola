/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavsettingswidgettest.h"
#include "administratorsettingsdialog/webdav/webdavsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QTest>

QTEST_MAIN(WebDavSettingsWidgetTest)
WebDavSettingsWidgetTest::WebDavSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WebDavSettingsWidgetTest::shouldHaveDefaultValues()
{
    WebDavSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("Webdav_Integration_Enabled"));
}

#include "moc_webdavsettingswidgettest.cpp"
