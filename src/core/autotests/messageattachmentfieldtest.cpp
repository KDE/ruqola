/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentfieldtest.h"
#include "messages/messageattachmentfield.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageAttachmentFieldTest)
MessageAttachmentFieldTest::MessageAttachmentFieldTest(QObject *parent)
    : QObject(parent)
{
}

void MessageAttachmentFieldTest::shouldHaveDefaultValues()
{
    MessageAttachmentField field;
    QVERIFY(field.title().isEmpty());
    QVERIFY(field.value().isEmpty());
}

#include "moc_messageattachmentfieldtest.cpp"
