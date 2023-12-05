/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdownloadandsavejobtest.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include <QTest>
QTEST_MAIN(MessageAttachmentDownloadAndSaveJobTest)

MessageAttachmentDownloadAndSaveJobTest::MessageAttachmentDownloadAndSaveJobTest(QObject *parent)
    : QObject{parent}
{
}

void MessageAttachmentDownloadAndSaveJobTest::shouldHaveDefaultValues()
{
    MessageAttachmentDownloadAndSaveJob w;
    QVERIFY(!w.rocketChatAccount());
    QVERIFY(!w.canStart());
    QVERIFY(!w.info().isValid());
}

void MessageAttachmentDownloadAndSaveJobTest::shouldMessageAttachmentDownloadJobInfoHaveDefaultValues()
{
    MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
    QVERIFY(info.attachmentPath.isEmpty());
    QVERIFY(!info.needToDownloadAttachment);
    QVERIFY(!info.parentWidget);
    QVERIFY(!info.isValid());
    QCOMPARE(info.attachmentType, MessageAttachmentDownloadAndSaveJob::AttachmentType::Unknown);
    QCOMPARE(info.actionType, MessageAttachmentDownloadAndSaveJob::ActionType::Unknown);
}

#include "moc_messageattachmentdownloadandsavejobtest.cpp"
