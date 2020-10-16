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

#include "directchannelinfowidgettest.h"
#include "dialogs/directchannelinfowidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(DirectChannelInfoWidgetTest)
DirectChannelInfoWidgetTest::DirectChannelInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void DirectChannelInfoWidgetTest::shouldHaveDefaultValues()
{
    DirectChannelInfoWidget w;

    QFormLayout *mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QLabel *mAvatar = w.findChild<QLabel * >(QStringLiteral("mAvatar"));
    QVERIFY(mAvatar);
    QVERIFY(mAvatar->text().isEmpty());

    QLabel *mName = w.findChild<QLabel * >(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QCOMPARE(mName->textInteractionFlags(), Qt::TextBrowserInteraction);

    QLabel *mUserName = w.findChild<QLabel * >(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QCOMPARE(mUserName->textInteractionFlags(), Qt::TextBrowserInteraction);

    QLabel *mStatus = w.findChild<QLabel * >(QStringLiteral("mStatus"));
    QVERIFY(mStatus);
    QVERIFY(mStatus->text().isEmpty());
    QCOMPARE(mStatus->textInteractionFlags(), Qt::TextBrowserInteraction);

    QLabel *mCustomStatus = w.findChild<QLabel * >(QStringLiteral("mCustomStatus"));
    QVERIFY(mCustomStatus);
    QVERIFY(mCustomStatus->text().isEmpty());
    QCOMPARE(mCustomStatus->textInteractionFlags(), Qt::TextBrowserInteraction);

    QLabel *mTimeZone = w.findChild<QLabel * >(QStringLiteral("mTimeZone"));
    QVERIFY(mTimeZone);
    QVERIFY(mTimeZone->text().isEmpty());
    QCOMPARE(mTimeZone->textInteractionFlags(), Qt::TextBrowserInteraction);

    QLabel *mRoles = w.findChild<QLabel * >(QStringLiteral("mRoles"));
    QVERIFY(mRoles);
    QVERIFY(mRoles->text().isEmpty());
    QCOMPARE(mRoles->textInteractionFlags(), Qt::TextBrowserInteraction);

    QLabel *mCreateAt = w.findChild<QLabel * >(QStringLiteral("mCreateAt"));
    QVERIFY(mCreateAt);
    QVERIFY(mCreateAt->text().isEmpty());
    QCOMPARE(mCreateAt->textInteractionFlags(), Qt::TextBrowserInteraction);

    QLabel *mLastLogin = w.findChild<QLabel * >(QStringLiteral("mLastLogin"));
    QVERIFY(mLastLogin);
    QVERIFY(mLastLogin->text().isEmpty());
    QCOMPARE(mLastLogin->textInteractionFlags(), Qt::TextBrowserInteraction);
}
