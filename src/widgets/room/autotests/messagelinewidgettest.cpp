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

#include "messagelinewidgettest.h"
#include "room/messagelinewidget.h"
#include "room/messagelineedit.h"
#include <QHBoxLayout>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(MessageLineWidgetTest)

MessageLineWidgetTest::MessageLineWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MessageLineWidgetTest::shouldHaveDefaultValues()
{
    MessageLineWidget w;
    QCOMPARE(w.mode(), MessageLineWidget::EditingMode::NewMessage);

    auto *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    QCOMPARE(mainLayout->spacing(), 0);

    auto *mMessageLineEdit = w.findChild<MessageLineEdit *>(QStringLiteral("mMessageLineEdit"));
    QVERIFY(mMessageLineEdit);

    auto *mSendFile = w.findChild<QToolButton *>(QStringLiteral("mSendFile"));
    QVERIFY(mSendFile);
    QVERIFY(!mSendFile->icon().isNull());

    auto *mEmoticonButton = w.findChild<QToolButton *>(QStringLiteral("mEmoticonButton"));
    QVERIFY(mEmoticonButton);
    QVERIFY(!mEmoticonButton->icon().isNull());
    QCOMPARE(mEmoticonButton->popupMode(), QToolButton::InstantPopup);

    auto *mSendMessageButton = w.findChild<QToolButton *>(QStringLiteral("mSendMessageButton"));
    QVERIFY(mSendMessageButton);
    QVERIFY(!mSendMessageButton->icon().isNull());
}

//TODO test signal/slot
