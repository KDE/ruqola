/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratordirectroomseditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mLineEdit = w.findChild<QLineEdit *>(u"mLineEdit"_s);
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());

    auto mDefaultCheckBox = w.findChild<QCheckBox *>(u"mDefaultCheckBox"_s);
    QVERIFY(mDefaultCheckBox);
    QVERIFY(!mDefaultCheckBox->isChecked());
    QVERIFY(!mDefaultCheckBox->text().isEmpty());

    auto mFavoriteCheckBox = w.findChild<QCheckBox *>(u"mFavoriteCheckBox"_s);
    QVERIFY(mFavoriteCheckBox);
    QVERIFY(!mFavoriteCheckBox->isChecked());
    QVERIFY(!mFavoriteCheckBox->text().isEmpty());

    auto mFeaturedCheckBox = w.findChild<QCheckBox *>(u"mFeaturedCheckBox"_s);
    QVERIFY(mFeaturedCheckBox);
    QVERIFY(!mFeaturedCheckBox->isChecked());
    QVERIFY(!mFeaturedCheckBox->text().isEmpty());
}

#include "moc_administratordirectroomseditwidgettest.cpp"
