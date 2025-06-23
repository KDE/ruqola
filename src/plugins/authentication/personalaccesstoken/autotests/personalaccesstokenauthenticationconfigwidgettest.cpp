/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationconfigwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mUserLineEdit = w.findChild<QLineEdit *>(u"mUserLineEdit"_s);
    QVERIFY(mUserLineEdit);
    QVERIFY(mUserLineEdit->text().isEmpty());

    auto mPersonalAccessTokenLineEdit = w.findChild<QLineEdit *>(u"mPersonalAccessTokenLineEdit"_s);
    QVERIFY(mPersonalAccessTokenLineEdit);
    QVERIFY(mPersonalAccessTokenLineEdit->text().isEmpty());

    auto mServerNameLineEdit = w.findChild<QLineEdit *>(u"mServerNameLineEdit"_s);
    QVERIFY(mServerNameLineEdit);
    QVERIFY(mServerNameLineEdit->text().isEmpty());

    auto mAccountNameLineEdit = w.findChild<QLineEdit *>(u"mAccountNameLineEdit"_s);
    QVERIFY(mAccountNameLineEdit);
    QVERIFY(mAccountNameLineEdit->text().isEmpty());

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->rowCount(), 4);

    // QVERIFY(w.info().isEmpty());
}

void PersonalAccessTokenAuthenticationConfigWidgetTest::shouldEmitSignalEnableOkButton()
{
    PersonalAccessTokenAuthenticationConfigWidget w;
    auto mUserLineEdit = w.findChild<QLineEdit *>(u"mUserLineEdit"_s);
    auto mPersonalAccessTokenLineEdit = w.findChild<QLineEdit *>(u"mPersonalAccessTokenLineEdit"_s);
    auto mServerNameLineEdit = w.findChild<QLineEdit *>(u"mServerNameLineEdit"_s);
    auto mAccountNameLineEdit = w.findChild<QLineEdit *>(u"mAccountNameLineEdit"_s);

    QSignalSpy spyEnableOkButton(&w, &PersonalAccessTokenAuthenticationConfigWidget::settingsIsValid);
    mUserLineEdit->setText(u"foo"_s);
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mUserLineEdit->clear();
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mPersonalAccessTokenLineEdit->setText(u"bla"_s);
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mUserLineEdit->setText(u"foo"_s);
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mServerNameLineEdit->setText(u"acc"_s);
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(!spyEnableOkButton.at(0).at(0).toBool());

    spyEnableOkButton.clear();
    mAccountNameLineEdit->setText(u"bb"_s);
    QCOMPARE(spyEnableOkButton.count(), 1);
    QVERIFY(spyEnableOkButton.at(0).at(0).toBool());
}

#include "moc_personalaccesstokenauthenticationconfigwidgettest.cpp"
