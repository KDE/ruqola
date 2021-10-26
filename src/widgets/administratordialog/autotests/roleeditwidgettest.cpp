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

#include "roleeditwidgettest.h"
#include "administratordialog/roles/roleeditwidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(RoleEditWidgetTest)
RoleEditWidgetTest::RoleEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RoleEditWidgetTest::shouldHaveDefaultValues()
{
    RoleEditWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());

    auto mDescription = w.findChild<QLineEdit *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());

    auto mTwoFactor = w.findChild<QCheckBox *>(QStringLiteral("mTwoFactor"));
    QVERIFY(mTwoFactor);
    QVERIFY(!mTwoFactor->isChecked());
    QVERIFY(!mTwoFactor->text().isEmpty());
}
