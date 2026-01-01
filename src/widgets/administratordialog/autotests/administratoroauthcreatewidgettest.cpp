/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
}

#include "moc_administratoroauthcreatewidgettest.cpp"
