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

#include "inviteuserswidgettest.h"
#include "dialogs/inviteuserswidget.h"
#include <KLineEdit>
#include <QLabel>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(InviteUsersWidgetTest)

InviteUsersWidgetTest::InviteUsersWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void InviteUsersWidgetTest::shouldHaveDefaultValues()
{
    InviteUsersWidget w;
    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QLabel *label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    KLineEdit *mInviteUserLineEdit = w.findChild<KLineEdit *>(QStringLiteral("mInviteUserLineEdit"));
    QVERIFY(mInviteUserLineEdit);
    QVERIFY(mInviteUserLineEdit->isReadOnly());
    QVERIFY(mInviteUserLineEdit->trapReturnKey());

    QToolButton *copyLinkButton = w.findChild<QToolButton *>(QStringLiteral("copyLinkButton"));
    QVERIFY(copyLinkButton);
    QVERIFY(!copyLinkButton->icon().isNull());
    QVERIFY(!copyLinkButton->toolTip().isEmpty());

    QLabel *mLink = w.findChild<QLabel *>(QStringLiteral("mLink"));
    QVERIFY(mLink);
    QVERIFY(mLink->text().isEmpty());
}
