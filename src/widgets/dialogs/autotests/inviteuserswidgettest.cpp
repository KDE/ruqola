/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    InviteUsersWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

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
    QCOMPARE(formLayout->contentsMargins(), QMargins{});

    auto mExpirationDays = w.findChild<QComboBox *>(QStringLiteral("mExpirationDays"));
    QVERIFY(mExpirationDays);

    auto mMaxUses = w.findChild<QComboBox *>(QStringLiteral("mMaxUses"));
    QVERIFY(mMaxUses);

    auto generateNewLink = w.findChild<QPushButton *>(QStringLiteral("generateNewLink"));
    QVERIFY(generateNewLink);
    QVERIFY(!generateNewLink->text().isEmpty());
}

#include "moc_inviteuserswidgettest.cpp"
