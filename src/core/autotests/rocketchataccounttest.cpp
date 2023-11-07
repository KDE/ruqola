/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccounttest.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/commonmessagesmodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/discussionsmodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include "model/listmessagesfilterproxymodel.h"
#include "model/listmessagesmodel.h"
#include "rocketchataccount.h"
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(RocketChatAccountTest)

RocketChatAccountTest::RocketChatAccountTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RocketChatAccountTest::shouldHaveDefaultValue()
{
    RocketChatAccount w;
    QVERIFY(w.emojiManager());
    QVERIFY(w.roomModel());
    QVERIFY(w.settings());
    QVERIFY(!w.ruqolaLogger());
    QVERIFY(w.ruqolaServerConfig());
    QVERIFY(w.userCompleterFilterModelProxy());
    QVERIFY(w.loginMethodModel());
    QVERIFY(w.inputTextManager());
    QVERIFY(w.inputThreadMessageTextManager());
    QVERIFY(w.searchMessageModel());
    QCOMPARE(w.searchMessageFilterProxyModel()->sourceModel(), w.searchMessageModel());
    QVERIFY(w.receiveTypingNotificationManager());
    QVERIFY(!w.editingMode());
    QVERIFY(w.filesModelForRoom());
    QVERIFY(w.filesForRoomFilterProxyModel());
    QCOMPARE(w.filesForRoomFilterProxyModel()->sourceModel(), w.filesModelForRoom());
    QVERIFY(w.discussionsFilterProxyModel());
    QCOMPARE(w.discussionsFilterProxyModel()->sourceModel(), w.discussionsModel());

    QVERIFY(w.listMessageModel());
    QCOMPARE(w.listMessagesFilterProxyModel()->sourceModel(), w.listMessageModel());

    QVERIFY(w.autoTranslateLanguagesModel());
    QVERIFY(w.accountRoomSettings());
    QVERIFY(w.commandsModel());
    QVERIFY(!w.allowAvatarChanged());
    QVERIFY(w.customSoundManager());

    QVERIFY(!w.twoFactorAuthenticationEnabled());
    QVERIFY(!w.twoFactorAuthenticationByEmailEnabled());
    QVERIFY(!w.twoFactorAuthenticationByTOTPEnabled());

    QVERIFY(!w.twoFactorAuthenticationEnforcePasswordFallback());

    QVERIFY(w.searchListCompletion().isEmpty());
    QVERIFY(w.uploadFileManager());
    QVERIFY(!w.allowPasswordReset());
    QVERIFY(w.bannerInfos().isEmpty());
    QVERIFY(w.videoConferenceManager());
    QVERIFY(w.videoConferenceMessageInfoManager());
}

#include "moc_rocketchataccounttest.cpp"
