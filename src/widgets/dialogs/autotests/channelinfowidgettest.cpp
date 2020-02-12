/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "channelinfowidgettest.h"
#include "dialogs/channelinfowidget.h"
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTest>
QTEST_MAIN(ChannelInfoWidgetTest)
ChannelInfoWidgetTest::ChannelInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoWidgetTest::shouldHaveDefaultValues()
{
    ChannelInfoWidget w;
    auto *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);

    //Editable channel
    auto *mEditableChannel = w.findChild<QWidget *>(QStringLiteral("mEditableChannel"));
    QVERIFY(mEditableChannel);

    auto *layout = mEditableChannel->findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mComment = mEditableChannel->findChild<ChangeTextWidget *>(QStringLiteral("mComment"));
    QVERIFY(mComment);

    auto *mAnnouncement = mEditableChannel->findChild<ChangeTextWidget *>(QStringLiteral("mAnnouncement"));
    QVERIFY(mAnnouncement);

    auto *mDescription = mEditableChannel->findChild<ChangeTextWidget *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);

    auto *mPasswordLineEdit = mEditableChannel->findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);

    auto *mArchive = mEditableChannel->findChild<QCheckBox *>(QStringLiteral("mArchive"));
    QVERIFY(mArchive);
    QVERIFY(mArchive->text().isEmpty());

    auto *mReadOnly = mEditableChannel->findChild<QCheckBox *>(QStringLiteral("mReadOnly"));
    QVERIFY(mReadOnly);
    QVERIFY(mReadOnly->text().isEmpty());

    auto *mPrivate = mEditableChannel->findChild<QCheckBox *>(QStringLiteral("mPrivate"));
    QVERIFY(mPrivate);
    QVERIFY(mPrivate->text().isEmpty());

    auto *mDeleteChannel = mEditableChannel->findChild<QPushButton *>(QStringLiteral("mDeleteChannel"));
    QVERIFY(mDeleteChannel);
    QVERIFY(!mDeleteChannel->text().isEmpty());

    //ReadOnly Channel
    auto *mReadOnlyChannel = w.findChild<QWidget *>(QStringLiteral("mReadOnlyChannel"));
    QVERIFY(mReadOnlyChannel);

    auto *layoutreadonly = mReadOnlyChannel->findChild<QFormLayout *>(QStringLiteral("layoutReadOnly"));
    QVERIFY(layoutreadonly);
    QCOMPARE(layoutreadonly->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mNameReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mNameReadOnly"));
    QVERIFY(mNameReadOnly);
    QVERIFY(mNameReadOnly->text().isEmpty());
    QCOMPARE(mNameReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QCOMPARE(mNameReadOnly->textFormat(), Qt::RichText);

    auto *mCommentReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mCommentReadOnly"));
    QVERIFY(mCommentReadOnly);
    QVERIFY(mCommentReadOnly->text().isEmpty());
    QCOMPARE(mCommentReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mCommentReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto *mAnnouncementReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mAnnouncementReadOnly"));
    QVERIFY(mAnnouncementReadOnly);
    QVERIFY(mAnnouncementReadOnly->text().isEmpty());
    QCOMPARE(mAnnouncementReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mAnnouncementReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto *mDescriptionReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mDescriptionReadOnly"));
    QVERIFY(mDescriptionReadOnly);
    QCOMPARE(mDescriptionReadOnly->textFormat(), Qt::RichText);
    QVERIFY(mDescriptionReadOnly->text().isEmpty());
    QCOMPARE(mDescriptionReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
}
