/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteuserswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto collapsibleGroupBox = w.findChild<KCollapsibleGroupBox *>(u"collapsibleGroupBox"_s);
    QVERIFY(collapsibleGroupBox);
    QVERIFY(!collapsibleGroupBox->title().isEmpty());

    auto mInviteUserLineEdit = w.findChild<QLineEdit *>(u"mInviteUserLineEdit"_s);
    QVERIFY(mInviteUserLineEdit);
    QVERIFY(mInviteUserLineEdit->isReadOnly());

    auto copyLinkButton = w.findChild<QToolButton *>(u"copyLinkButton"_s);
    QVERIFY(copyLinkButton);
    QVERIFY(!copyLinkButton->icon().isNull());
    QVERIFY(!copyLinkButton->toolTip().isEmpty());

    auto mExpireDateLabel = w.findChild<QLabel *>(u"mExpireDateLabel"_s);
    QVERIFY(mExpireDateLabel);
    QVERIFY(mExpireDateLabel->text().isEmpty());

    auto formLayout = w.findChild<QFormLayout *>(u"formLayout"_s);
    QVERIFY(formLayout);
    QCOMPARE(formLayout->contentsMargins(), QMargins{});

    auto mExpirationDays = w.findChild<QComboBox *>(u"mExpirationDays"_s);
    QVERIFY(mExpirationDays);

    auto mMaxUses = w.findChild<QComboBox *>(u"mMaxUses"_s);
    QVERIFY(mMaxUses);

    auto generateNewLink = w.findChild<QPushButton *>(u"generateNewLink"_s);
    QVERIFY(generateNewLink);
    QVERIFY(!generateNewLink->text().isEmpty());
}

#include "moc_inviteuserswidgettest.cpp"
