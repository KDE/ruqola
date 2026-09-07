/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperimagetest.h"

#include "messages/messageattachment.h"
#include "room/delegate/messageattachmentdelegatehelperimage.h"
#include "testdata.h"

#include <QListView>
#include <QStandardPaths>
#include <QStyleOptionViewItem>
#include <QTest>
#include <memory>

QTEST_MAIN(MessageDelegateHelperImageTest)

using namespace Qt::Literals::StringLiterals;
MessageDelegateHelperImageTest::MessageDelegateHelperImageTest(QObject *parent)
    : QObject(parent)
{
    initTestAccount();
}

void MessageDelegateHelperImageTest::shouldExtractMessageData()
{
    std::unique_ptr<QListView> listView(new QListView());
    TextSelectionImpl textSelection;
    const MessageAttachmentDelegateHelperImage helper(nullptr, listView.get(), &textSelection);
    QStyleOptionViewItem option;
    const QWidget fakeWidget;
    option.widget = &fakeWidget;
    const MessageAttachment msgAttach = testAttachment();

    const MessageAttachmentDelegateHelperImage::ImageLayout layout = helper.layoutImage(msgAttach, option, 500, 500);
    QCOMPARE(layout.title, msgAttach.title());
    QCOMPARE(layout.hasDescription, msgAttach.hasDescription());
    QVERIFY(layout.isShown);
    QVERIFY(!layout.isAnimatedImage);
    QVERIFY(layout.imagePreviewPath.endsWith("/testfile.png"_L1));
    QVERIFY(qAbs(layout.imageSize.height() - layout.imageSize.width() * 2) <= 1); // aspect ratio is preserved, allow for one pixel rounding diff
    QCOMPARE(layout.pixmap.devicePixelRatioF(), fakeWidget.devicePixelRatioF());
    QVERIFY(layout.imageSize.height() < 500 * fakeWidget.devicePixelRatioF());
}

#include "moc_messageattachmentdelegatehelperimagetest.cpp"
