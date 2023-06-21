/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroautheditwidgettest.h"
#include "administratordialog/oauth/administratoroautheditwidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorOauthEditWidgetTest)
AdministratorOauthEditWidgetTest::AdministratorOauthEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorOauthEditWidgetTest::shouldHaveDefaultValues()
{
    AdministratorOauthEditWidget w;
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

    auto mClientId = w.findChild<QLineEdit *>(QStringLiteral("mClientId"));
    QVERIFY(mClientId);
    QVERIFY(mClientId->text().isEmpty());
    QVERIFY(mClientId->isReadOnly());

    auto mClientSecret = w.findChild<QLineEdit *>(QStringLiteral("mClientSecret"));
    QVERIFY(mClientSecret);
    QVERIFY(mClientSecret->text().isEmpty());
    QVERIFY(mClientSecret->isReadOnly());

    auto mAuthorizationUrl = w.findChild<QLineEdit *>(QStringLiteral("mAuthorizationUrl"));
    QVERIFY(mAuthorizationUrl);
    QVERIFY(mAuthorizationUrl->text().isEmpty());
    QVERIFY(mAuthorizationUrl->isReadOnly());

    auto mAccessTokenUrl = w.findChild<QLineEdit *>(QStringLiteral("mAccessTokenUrl"));
    QVERIFY(mAccessTokenUrl);
    QVERIFY(mAccessTokenUrl->text().isEmpty());
    QVERIFY(mAccessTokenUrl->isReadOnly());
}

#include "moc_administratoroautheditwidgettest.cpp"
