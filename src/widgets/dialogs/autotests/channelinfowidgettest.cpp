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
#include <KLineEdit>
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
    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QStackedWidget *mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);

    //Editable channel
    QWidget *mEditableChannel = w.findChild<QWidget *>(QStringLiteral("mEditableChannel"));
    QVERIFY(mEditableChannel);

    QFormLayout *layout = mEditableChannel->findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins(0, 0, 0, 0));

    ChangeTextWidget *mComment = mEditableChannel->findChild<ChangeTextWidget *>(QStringLiteral("mComment"));
    QVERIFY(mComment);

    ChangeTextWidget *mAnnouncement = mEditableChannel->findChild<ChangeTextWidget *>(QStringLiteral("mAnnouncement"));
    QVERIFY(mAnnouncement);

    ChangeTextWidget *mDescription = mEditableChannel->findChild<ChangeTextWidget *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);

    KPasswordLineEdit *mPasswordLineEdit = mEditableChannel->findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);

    QCheckBox *mArchive = mEditableChannel->findChild<QCheckBox *>(QStringLiteral("mArchive"));
    QVERIFY(mArchive);
    QVERIFY(mArchive->text().isEmpty());

    QCheckBox *mReadOnly = mEditableChannel->findChild<QCheckBox *>(QStringLiteral("mReadOnly"));
    QVERIFY(mReadOnly);
    QVERIFY(mReadOnly->text().isEmpty());

    QCheckBox *mPrivate = mEditableChannel->findChild<QCheckBox *>(QStringLiteral("mPrivate"));
    QVERIFY(mPrivate);
    QVERIFY(mPrivate->text().isEmpty());

    QPushButton *mDeleteChannel = mEditableChannel->findChild<QPushButton *>(QStringLiteral("mDeleteChannel"));
    QVERIFY(mDeleteChannel);
    QVERIFY(!mDeleteChannel->text().isEmpty());

    //ReadOnly Channel
    QWidget *mReadOnlyChannel = w.findChild<QWidget *>(QStringLiteral("mReadOnlyChannel"));
    QVERIFY(mReadOnlyChannel);

    QFormLayout *layoutreadonly = mReadOnlyChannel->findChild<QFormLayout *>(QStringLiteral("layoutReadOnly"));
    QVERIFY(layoutreadonly);
    QCOMPARE(layoutreadonly->contentsMargins(), QMargins(0, 0, 0, 0));

    QLabel *mNameReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mNameReadOnly"));
    QVERIFY(mNameReadOnly);
    QVERIFY(mNameReadOnly->text().isEmpty());

    QLabel *mCommentReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mCommentReadOnly"));
    QVERIFY(mCommentReadOnly);
    QVERIFY(mCommentReadOnly->text().isEmpty());

    QLabel *mAnnouncementReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mAnnouncementReadOnly"));
    QVERIFY(mAnnouncementReadOnly);
    QVERIFY(mAnnouncementReadOnly->text().isEmpty());

    QLabel *mDescriptionReadOnly = mReadOnlyChannel->findChild<QLabel *>(QStringLiteral("mDescriptionReadOnly"));
    QVERIFY(mDescriptionReadOnly);
    QVERIFY(mDescriptionReadOnly->text().isEmpty());
}
