/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createupdateuserinfotest.h"
#include "createupdateuserinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(CreateUpdateUserInfoTest)
CreateUpdateUserInfoTest::CreateUpdateUserInfoTest(QObject *parent)
    : QObject(parent)
{
}

void CreateUpdateUserInfoTest::shouldHaveDefaultValues()
{
    RocketChatRestApi::CreateUpdateUserInfo info;
    QVERIFY(info.mRoles.isEmpty());
    QVERIFY(info.mUserId.isEmpty());
    QVERIFY(info.mEmail.isEmpty());
    QVERIFY(info.mName.isEmpty());
    QVERIFY(info.mUserName.isEmpty());
    QVERIFY(info.mPassword.isEmpty());
    QVERIFY(info.mStatusText.isEmpty());
    QVERIFY(info.mBio.isEmpty());
    QVERIFY(info.mNickName.isEmpty());
    QVERIFY(!info.mJoinDefaultChannels);
    QVERIFY(!info.mRequirePasswordChange);
    QVERIFY(!info.mSendWelcomeEmail);
    QVERIFY(!info.mSetRandomPassword);
    QVERIFY(!info.mVerified);
    QCOMPARE(info.mTypeInfo, RocketChatRestApi::CreateUpdateUserInfo::Create);
}
