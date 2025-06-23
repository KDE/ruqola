/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/invites/administratorinviteswidget.h"
#include "administratordialog/invites/invitetreeview.h"
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorInvitesWidgetTest)
AdministratorInvitesWidgetTest::AdministratorInvitesWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorInvitesWidgetTest::shouldHaveDefaultValues()
{
    AdministratorInvitesWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    auto mSearchLineWidget = d.findChild<QLineEdit *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mInviteTreeView = d.findChild<InviteTreeView *>(u"mInviteTreeWidget"_s);
    QVERIFY(mInviteTreeView);
}

#include "moc_administratorinviteswidgettest.cpp"
