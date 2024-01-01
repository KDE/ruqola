/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroautheditdialogtest.h"
#include "administratordialog/oauth/administratoroautheditdialog.h"
#include "administratordialog/oauth/administratoroautheditwidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorOauthEditDialogTest)
AdministratorOauthEditDialogTest::AdministratorOauthEditDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorOauthEditDialogTest::shouldHaveDefaultValues()
{
    AdministratorOauthEditDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), QDialogButtonBox::StandardButtons{QDialogButtonBox::Ok | QDialogButtonBox::Cancel});

    auto mOauthEditWidget = w.findChild<AdministratorOauthEditWidget *>(QStringLiteral("mOauthEditWidget"));
    QVERIFY(mOauthEditWidget);

    auto mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(!mOkButton->isEnabled());
}

#include "moc_administratoroautheditdialogtest.cpp"
