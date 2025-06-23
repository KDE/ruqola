/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    auto mSearchLineWidget = d.findChild<QLineEdit *>(u"mSearchLineWidget"_s);
    QVERIFY(mSearchLineWidget);
    QVERIFY(mSearchLineWidget->text().isEmpty());
    QVERIFY(!mSearchLineWidget->placeholderText().isEmpty());

    auto mOauthTreeWidget = d.findChild<OauthTreeView *>(u"mOauthTreeWidget"_s);
    QVERIFY(mOauthTreeWidget);
}

#include "moc_administratoroauthwidgettest.cpp"
