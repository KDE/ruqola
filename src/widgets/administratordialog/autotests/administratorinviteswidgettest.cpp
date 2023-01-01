/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteswidgettest.h"
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
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mSearchLineWidget = d.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mInviteTreeView = d.findChild<InviteTreeView *>(QStringLiteral("mInviteTreeWidget"));
    QVERIFY(mInviteTreeView);
}
