/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallsettingswidgettest.h"
#include "administratorsettingsdialog/conferencecall/conferencecallsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QFormLayout>
#include <QTest>

QTEST_MAIN(ConferenceCallSettingsWidgetTest)
ConferenceCallSettingsWidgetTest::ConferenceCallSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceCallSettingsWidgetTest::shouldHaveDefaultValues()
{
    ConferenceCallSettingsWidget w(nullptr);
    auto mDefaultProvider = w.findChild<QComboBox *>(QStringLiteral("mDefaultProvider"));
    QVERIFY(mDefaultProvider);
    QVERIFY(mDefaultProvider->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDefaultProvider), QStringLiteral("VideoConf_Default_Provider"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("VideoConf_Default_Provider"));
}
