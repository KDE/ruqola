/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratordirectroomseditwidgettest.h"
#include "administratordialog/rooms/administratordirectroomseditwidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorDirectRoomsEditWidgetTest)
AdministratorDirectRoomsEditWidgetTest::AdministratorDirectRoomsEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorDirectRoomsEditWidgetTest::shouldHaveDefaultValues()
{
    AdministratorDirectRoomsEditWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mLineEdit"));
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());

    auto mDefaultCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mDefaultCheckBox"));
    QVERIFY(mDefaultCheckBox);
    QVERIFY(!mDefaultCheckBox->isChecked());
    QVERIFY(!mDefaultCheckBox->text().isEmpty());

    auto mFavoriteCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mFavoriteCheckBox"));
    QVERIFY(mFavoriteCheckBox);
    QVERIFY(!mFavoriteCheckBox->isChecked());
    QVERIFY(!mFavoriteCheckBox->text().isEmpty());

    auto mFeaturedCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mFeaturedCheckBox"));
    QVERIFY(mFeaturedCheckBox);
    QVERIFY(!mFeaturedCheckBox->isChecked());
    QVERIFY(!mFeaturedCheckBox->text().isEmpty());
}
