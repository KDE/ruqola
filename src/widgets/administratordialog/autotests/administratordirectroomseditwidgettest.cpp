/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
