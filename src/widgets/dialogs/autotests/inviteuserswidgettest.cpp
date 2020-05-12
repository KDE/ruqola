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
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
#include <KCollapsibleGroupBox>
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

    KCollapsibleGroupBox *collapsibleGroupBox = w.findChild<KCollapsibleGroupBox *>(QStringLiteral("collapsibleGroupBox"));
    QVERIFY(collapsibleGroupBox);
    QVERIFY(!collapsibleGroupBox->title().isEmpty());

    QLineEdit *mInviteUserLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mInviteUserLineEdit"));
    QVERIFY(mInviteUserLineEdit);
    QVERIFY(mInviteUserLineEdit->isReadOnly());

    QToolButton *copyLinkButton = w.findChild<QToolButton *>(QStringLiteral("copyLinkButton"));
    QVERIFY(copyLinkButton);
    QVERIFY(!copyLinkButton->icon().isNull());
    QVERIFY(!copyLinkButton->toolTip().isEmpty());

    QLabel *mExpireDateLabel = w.findChild<QLabel *>(QStringLiteral("mExpireDateLabel"));
    QVERIFY(mExpireDateLabel);
    QVERIFY(mExpireDateLabel->text().isEmpty());

    QFormLayout *formLayout = w.findChild<QFormLayout *>(QStringLiteral("formLayout"));
    QVERIFY(formLayout);
    QCOMPARE(formLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QComboBox *mExpirationDays = w.findChild<QComboBox *>(QStringLiteral("mExpirationDays"));
    QVERIFY(mExpirationDays);

    QComboBox *mMaxUses = w.findChild<QComboBox *>(QStringLiteral("mMaxUses"));
    QVERIFY(mMaxUses);

    QPushButton *generateNewLink = w.findChild<QPushButton *>(QStringLiteral("generateNewLink"));
    QVERIFY(generateNewLink);
    QVERIFY(!generateNewLink->text().isEmpty());
}
