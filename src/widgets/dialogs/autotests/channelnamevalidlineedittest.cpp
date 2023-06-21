/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlineedittest.h"
#include "dialogs/channelnamevalidlineedit.h"
#include <QTest>
QTEST_MAIN(ChannelNameValidLineEditTest)
ChannelNameValidLineEditTest::ChannelNameValidLineEditTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelNameValidLineEditTest::shouldHaveDefaultValue()
{
    ChannelNameValidLineEdit d(nullptr);
    QVERIFY(d.isClearButtonEnabled());
}

#include "moc_channelnamevalidlineedittest.cpp"
