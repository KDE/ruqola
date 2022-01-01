/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechtextplugintest.h"
#include "../texttospeechtextplugin.h"
#include "room/plugins/plugintextinterface.h"
#include <QTest>
QTEST_MAIN(TextToSpeechTextPluginTest)

TextToSpeechTextPluginTest::TextToSpeechTextPluginTest(QObject *parent)
    : QObject(parent)
{
}

void TextToSpeechTextPluginTest::shouldHaveDefaultValues()
{
    TextToSpeechTextPlugin w;
    auto *interface = w.createInterface(nullptr);
    QVERIFY(interface);
    delete interface;
}
