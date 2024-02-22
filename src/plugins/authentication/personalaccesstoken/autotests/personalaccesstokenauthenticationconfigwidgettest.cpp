/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationconfigwidgettest.h"
#include "../personalaccesstokenauthenticationconfigwidget.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(PersonalAccessTokenAuthenticationConfigWidgetTest)
PersonalAccessTokenAuthenticationConfigWidgetTest::PersonalAccessTokenAuthenticationConfigWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PersonalAccessTokenAuthenticationConfigWidgetTest::shouldHaveDefaultValues()
{
    PersonalAccessTokenAuthenticationConfigWidget w;
    auto mUserLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mUserLineEdit"));
    QVERIFY(mUserLineEdit);
    QVERIFY(mUserLineEdit->text().isEmpty());

    auto mPersonalAccessTokenLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mPersonalAccessTokenLineEdit"));
    QVERIFY(mPersonalAccessTokenLineEdit);
    QVERIFY(mPersonalAccessTokenLineEdit->text().isEmpty());

    auto mServerNameLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mServerNameLineEdit"));
    QVERIFY(mServerNameLineEdit);
    QVERIFY(mServerNameLineEdit->text().isEmpty());

    auto mAccountNameLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mAccountNameLineEdit"));
    QVERIFY(mAccountNameLineEdit);
    QVERIFY(mAccountNameLineEdit->text().isEmpty());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->rowCount(), 4);

    QVERIFY(w.info().isEmpty());
}

void PersonalAccessTokenAuthenticationConfigWidgetTest::shouldEmitSignalEnableOkButton()
{
    PersonalAccessTokenAuthenticationConfigWidget w;
    auto mUserLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mUserLineEdit"));
    auto mPersonalAccessTokenLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mPersonalAccessTokenLineEdit"));
    QSignalSpy spyEnableOkButton(&w, &PersonalAccessTokenAuthenticationConfigWidget::enableOkButton);
    mUserLineEdit->setText(QStringLiteral("foo"));
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mUserLineEdit->clear();
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mPersonalAccessTokenLineEdit->setText(QStringLiteral("bla"));
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mUserLineEdit->setText(QStringLiteral("foo"));
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(spyEnableOkButton.at(0).at(0).toBool());
}

#include "moc_personalaccesstokenauthenticationconfigwidgettest.cpp"
