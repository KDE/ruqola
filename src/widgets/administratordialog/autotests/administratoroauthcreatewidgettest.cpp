/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatewidgettest.h"
#include "administratordialog/oauth/administratoroauthcreatewidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorOauthCreateWidgetTest)
AdministratorOauthCreateWidgetTest::AdministratorOauthCreateWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorOauthCreateWidgetTest::shouldHaveDefaultValues()
{
    AdministratorOauthCreateWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mActiveCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mActiveCheckBox"));
    QVERIFY(mActiveCheckBox);
    QVERIFY(!mActiveCheckBox->text().isEmpty());
    QVERIFY(!mActiveCheckBox->isChecked());

    auto mApplicationName = w.findChild<QLineEdit *>(QStringLiteral("mApplicationName"));
    QVERIFY(mApplicationName);
    QVERIFY(mApplicationName->text().isEmpty());

    auto mRedirectUrl = w.findChild<QLineEdit *>(QStringLiteral("mRedirectUrl"));
    QVERIFY(mRedirectUrl);
    QVERIFY(mRedirectUrl->text().isEmpty());
}
