/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatedialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/oauth/administratoroauthcreatedialog.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorOauthCreateDialogTest)
AdministratorOauthCreateDialogTest::AdministratorOauthCreateDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorOauthCreateDialogTest::shouldHaveDefaultValues()
{
    AdministratorOauthCreateDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Ok | QDialogButtonBox::Cancel});

    auto mCreateWidget = w.findChild<AdministratorOauthCreateWidget *>(u"mCreateWidget"_s);
    QVERIFY(mCreateWidget);

    auto mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(!mOkButton->isEnabled());
}

#include "moc_administratoroauthcreatedialogtest.cpp"
