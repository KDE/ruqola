/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetextedittest.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include "room/messagetextedit.h"
#include <QTest>

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
    w.insertEmoji(u":emoji:"_s);
    QCOMPARE(w.text(), u":emoji: "_s);
    QVERIFY(!w.acceptRichText());
}

#include "moc_messagetextedittest.cpp"
