/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "permissionseditwidgettest.h"
#include "administratordialog/permissions/permissionseditwidget.h"
#include "misc/rolescombobox.h"
#include <QFormLayout>
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
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mRolesCombobox = w.findChild<RolesComboBox *>(QStringLiteral("mRolesCombobox"));
    QVERIFY(mRolesCombobox);
}
