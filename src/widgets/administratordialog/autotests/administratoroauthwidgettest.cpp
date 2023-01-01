/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthwidgettest.h"
#include "administratordialog/oauth/administratoroauthwidget.h"
#include "administratordialog/oauth/oauthtreeview.h"
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorOauthWidgetTest)
AdministratorOauthWidgetTest::AdministratorOauthWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorOauthWidgetTest::shouldHaveDefaultValues()
{
    AdministratorOauthWidget d(nullptr);

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mSearchLineWidget = d.findChild<QLineEdit *>(QStringLiteral("mSearchLineWidget"));
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mOauthTreeWidget = d.findChild<OauthTreeView *>(QStringLiteral("mOauthTreeWidget"));
    QVERIFY(mOauthTreeWidget);
}
