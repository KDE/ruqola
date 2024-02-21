/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationconfigdialogtest.h"
#include "../personalaccesstokenauthenticationconfigdialog.h"
#include "../personalaccesstokenauthenticationconfigwidget.h"
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(PersonalAccessTokenAuthenticationConfigDialogTest)
PersonalAccessTokenAuthenticationConfigDialogTest::PersonalAccessTokenAuthenticationConfigDialogTest(QObject *parent)
    : QObject{parent}
{
}

void PersonalAccessTokenAuthenticationConfigDialogTest::shouldHaveDefaultValues()
{
    PersonalAccessTokenAuthenticationConfigDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mPersonalAccessTokenAuthenticationConfigWidget =
        d.findChild<PersonalAccessTokenAuthenticationConfigWidget *>(QStringLiteral("mPersonalAccessTokenAuthenticationConfigWidget"));
    QVERIFY(mPersonalAccessTokenAuthenticationConfigWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    auto okButton = button->findChild<QPushButton *>(QStringLiteral("okButton"));
    QVERIFY(okButton);
    QVERIFY(!okButton->isEnabled());
}

void PersonalAccessTokenAuthenticationConfigDialogTest::shouldEnableOkButton()
{
    PersonalAccessTokenAuthenticationConfigDialog d;
    auto mPersonalAccessTokenAuthenticationConfigWidget =
        d.findChild<PersonalAccessTokenAuthenticationConfigWidget *>(QStringLiteral("mPersonalAccessTokenAuthenticationConfigWidget"));
    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    auto okButton = button->findChild<QPushButton *>(QStringLiteral("okButton"));
    auto mUserLineEdit = mPersonalAccessTokenAuthenticationConfigWidget->findChild<QLineEdit *>(QStringLiteral("mUserLineEdit"));
    auto mPersonalAccessTokenLineEdit = mPersonalAccessTokenAuthenticationConfigWidget->findChild<QLineEdit *>(QStringLiteral("mPersonalAccessTokenLineEdit"));

    mUserLineEdit->setText(QStringLiteral("foo"));
    QVERIFY(!okButton->isEnabled());

    mUserLineEdit->clear();
    QVERIFY(!okButton->isEnabled());

    mPersonalAccessTokenLineEdit->setText(QStringLiteral("bla"));
    QVERIFY(!okButton->isEnabled());

    mUserLineEdit->setText(QStringLiteral("foo"));
    QVERIFY(okButton->isEnabled());
}

#include "moc_personalaccesstokenauthenticationconfigdialogtest.cpp"
