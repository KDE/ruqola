/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
#include <KCollapsibleGroupBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto collapsibleGroupBox = w.findChild<KCollapsibleGroupBox *>(QStringLiteral("collapsibleGroupBox"));
    QVERIFY(collapsibleGroupBox);
    QVERIFY(!collapsibleGroupBox->title().isEmpty());

    auto mInviteUserLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mInviteUserLineEdit"));
    QVERIFY(mInviteUserLineEdit);
    QVERIFY(mInviteUserLineEdit->isReadOnly());

    auto copyLinkButton = w.findChild<QToolButton *>(QStringLiteral("copyLinkButton"));
    QVERIFY(copyLinkButton);
    QVERIFY(!copyLinkButton->icon().isNull());
    QVERIFY(!copyLinkButton->toolTip().isEmpty());

    auto mExpireDateLabel = w.findChild<QLabel *>(QStringLiteral("mExpireDateLabel"));
    QVERIFY(mExpireDateLabel);
    QVERIFY(mExpireDateLabel->text().isEmpty());

    auto formLayout = w.findChild<QFormLayout *>(QStringLiteral("formLayout"));
    QVERIFY(formLayout);
    QCOMPARE(formLayout->contentsMargins(), {});

    auto mExpirationDays = w.findChild<QComboBox *>(QStringLiteral("mExpirationDays"));
    QVERIFY(mExpirationDays);

    auto mMaxUses = w.findChild<QComboBox *>(QStringLiteral("mMaxUses"));
    QVERIFY(mMaxUses);

    auto generateNewLink = w.findChild<QPushButton *>(QStringLiteral("generateNewLink"));
    QVERIFY(generateNewLink);
    QVERIFY(!generateNewLink->text().isEmpty());
}
