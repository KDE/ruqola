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

#include "serverinfowidgettest.h"
#include "dialogs/serverinfowidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(ServerInfoWidgetTest)
ServerInfoWidgetTest::ServerInfoWidgetTest(QWidget *parent)
    : QWidget(parent)
{
}

void ServerInfoWidgetTest::shouldHaveDefaultValues()
{
    ServerInfoWidget w;
    auto *layout = w.findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mAccountName = w.findChild<QLabel *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QCOMPARE(mAccountName->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto *mUserName = w.findChild<QLabel *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QCOMPARE(mUserName->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto *mServerVersion = w.findChild<QLabel *>(QStringLiteral("mServerVersion"));
    QVERIFY(mServerVersion);
    QVERIFY(mServerVersion->text().isEmpty());
    QCOMPARE(mServerVersion->textInteractionFlags(), Qt::TextSelectableByMouse);
}
