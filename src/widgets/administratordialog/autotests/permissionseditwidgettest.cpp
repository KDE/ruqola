/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionseditwidgettest.h"
#include "administratordialog/permissions/permissionseditwidget.h"
#include "misc/rolescombobox.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(PermissionsEditWidgetTest)
PermissionsEditWidgetTest::PermissionsEditWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionsEditWidgetTest::shouldHaveDefaultValues()
{
    PermissionsEditWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mRolesCombobox = w.findChild<RolesComboBox *>(QStringLiteral("mRolesCombobox"));
    QVERIFY(mRolesCombobox);
}
