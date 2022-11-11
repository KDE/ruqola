/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallsettingswidgettest.h"
#include "administratorsettingsdialog/conferencecall/conferencecallsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QTest>

QTEST_MAIN(ConferenceCallSettingsWidgetTest)
ConferenceCallSettingsWidgetTest::ConferenceCallSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}
