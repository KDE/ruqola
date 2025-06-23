/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/webdav/webdavsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>

QTEST_MAIN(WebDavSettingsWidgetTest)
WebDavSettingsWidgetTest::WebDavSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WebDavSettingsWidgetTest::shouldHaveDefaultValues()
{
    WebDavSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(u"mEnabled"_s);
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), u"Webdav_Integration_Enabled"_s);
}

#include "moc_webdavsettingswidgettest.cpp"
