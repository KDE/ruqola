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
    // TODO
}
