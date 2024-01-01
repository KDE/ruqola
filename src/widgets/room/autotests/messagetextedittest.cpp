/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetextedittest.h"
#include "room/messagetextedit.h"
#include <QTest>
#include <rocketchataccount.h>

QTEST_MAIN(MessageTextEditTest)

MessageTextEditTest::MessageTextEditTest(QObject *parent)
    : QObject(parent)
{
}

void MessageTextEditTest::shouldHaveDefautValues()
{
    MessageTextEdit w;
    RocketChatAccount account;
    w.setCurrentRocketChatAccount(&account, false);
    w.insertEmoji(QStringLiteral(":emoji:"));
    QCOMPARE(w.text(), QStringLiteral(":emoji: "));
    QVERIFY(!w.acceptRichText());
}

#include "moc_messagetextedittest.cpp"
