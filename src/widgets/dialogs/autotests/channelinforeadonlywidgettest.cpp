/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "channelinforeadonlywidgettest.h"
#include "dialogs/channelinforeadonlywidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(ChannelInfoReadOnlyWidgetTest)

ChannelInfoReadOnlyWidgetTest::ChannelInfoReadOnlyWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoReadOnlyWidgetTest::shouldHaveDefaultValues()
{
    ChannelInfoReadOnlyWidget w;
    auto layoutReadOnly = w.findChild<QFormLayout *>(QStringLiteral("layoutReadOnly"));
    QVERIFY(layoutReadOnly);
    QCOMPARE(layoutReadOnly->contentsMargins(), {});

    auto mNameReadOnly = w.findChild<QLabel *>(QStringLiteral("mNameReadOnly"));
    QVERIFY(mNameReadOnly);
    QCOMPARE(mNameReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mNameReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mNameReadOnly->openExternalLinks());

    auto mCommentReadOnly = w.findChild<QLabel *>(QStringLiteral("mCommentReadOnly"));
    QVERIFY(mCommentReadOnly);
    QCOMPARE(mCommentReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mCommentReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mCommentReadOnly->openExternalLinks());

    auto mAnnouncementReadOnly = w.findChild<QLabel *>(QStringLiteral("mAnnouncementReadOnly"));
    QVERIFY(mAnnouncementReadOnly);
    QCOMPARE(mAnnouncementReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mAnnouncementReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mAnnouncementReadOnly->openExternalLinks());

    auto mDescriptionReadOnly = w.findChild<QLabel *>(QStringLiteral("mDescriptionReadOnly"));
    QVERIFY(mDescriptionReadOnly);
    QCOMPARE(mDescriptionReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mDescriptionReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mDescriptionReadOnly->openExternalLinks());
}
