/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messageattachmentactionstest.h"
#include "messages/messageattachmentactions.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageAttachmentActionsTest)
using namespace Qt::Literals::StringLiterals;
MessageAttachmentActionsTest::MessageAttachmentActionsTest(QObject *parent)
    : QObject{parent}
{
}

void MessageAttachmentActionsTest::shouldHaveDefaultValues()
{
    const MessageAttachmentActions acts;
    QCOMPARE(acts.alignment(), MessageAttachmentActions::AlignmentButton::Unknown);
}

void MessageAttachmentActionsTest::shouldParseAlignment()
{
    const MessageAttachmentActions acts;
    QCOMPARE(acts.convertStringToAlignmentButton(u"horizontal"_s), MessageAttachmentActions::AlignmentButton::Horizontal);
    QCOMPARE(acts.convertStringToAlignmentButton(u"vertical"_s), MessageAttachmentActions::AlignmentButton::Horizontal);
    QCOMPARE(acts.convertStringToAlignmentButton(u"foo"_s), MessageAttachmentActions::AlignmentButton::Unknown);
    QCOMPARE(acts.convertStringToAlignmentButton(QString()), MessageAttachmentActions::AlignmentButton::Unknown);
}
#include "moc_messageattachmentactionstest.cpp"
