/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionseditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mRolesCombobox = w.findChild<RolesComboBox *>(u"mRolesCombobox"_s);
    QVERIFY(mRolesCombobox);
}

#include "moc_permissionseditwidgettest.cpp"
