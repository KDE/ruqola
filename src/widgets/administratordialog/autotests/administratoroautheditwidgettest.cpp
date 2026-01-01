/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroautheditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mActiveCheckBox = w.findChild<QCheckBox *>(u"mActiveCheckBox"_s);
    QVERIFY(mActiveCheckBox);
    QVERIFY(!mActiveCheckBox->text().isEmpty());
    QVERIFY(!mActiveCheckBox->isChecked());

    auto mApplicationName = w.findChild<QLineEdit *>(u"mApplicationName"_s);
    QVERIFY(mApplicationName);
    QVERIFY(mApplicationName->text().isEmpty());

    auto mRedirectUrl = w.findChild<QLineEdit *>(u"mRedirectUrl"_s);
    QVERIFY(mRedirectUrl);
    QVERIFY(mRedirectUrl->text().isEmpty());

    auto mClientId = w.findChild<QLineEdit *>(u"mClientId"_s);
    QVERIFY(mClientId);
    QVERIFY(mClientId->text().isEmpty());
    QVERIFY(mClientId->isReadOnly());

    auto mClientSecret = w.findChild<QLineEdit *>(u"mClientSecret"_s);
    QVERIFY(mClientSecret);
    QVERIFY(mClientSecret->text().isEmpty());
    QVERIFY(mClientSecret->isReadOnly());

    auto mAuthorizationUrl = w.findChild<QLineEdit *>(u"mAuthorizationUrl"_s);
    QVERIFY(mAuthorizationUrl);
    QVERIFY(mAuthorizationUrl->text().isEmpty());
    QVERIFY(mAuthorizationUrl->isReadOnly());

    auto mAccessTokenUrl = w.findChild<QLineEdit *>(u"mAccessTokenUrl"_s);
    QVERIFY(mAccessTokenUrl);
    QVERIFY(mAccessTokenUrl->text().isEmpty());
    QVERIFY(mAccessTokenUrl->isReadOnly());
}

#include "moc_administratoroautheditwidgettest.cpp"
