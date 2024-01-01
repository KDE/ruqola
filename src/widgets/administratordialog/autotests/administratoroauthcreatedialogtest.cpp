/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatedialogtest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Ok | QDialogButtonBox::Cancel});

    auto mCreateWidget = w.findChild<AdministratorOauthCreateWidget *>(QStringLiteral("mCreateWidget"));
    QVERIFY(mCreateWidget);

    auto mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(!mOkButton->isEnabled());
}

#include "moc_administratoroauthcreatedialogtest.cpp"
