/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webrtcsettingswidgettest.h"
#include "administratorsettingsdialog/webrtc/webrtcsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QFormLayout>
#include <QTest>

QTEST_MAIN(WebRtcSettingsWidgetTest)

WebRtcSettingsWidgetTest::WebRtcSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WebRtcSettingsWidgetTest::shouldHaveDefaultValues()
{
    WebRtcSettingsWidget w(nullptr);
    // TODO
}
