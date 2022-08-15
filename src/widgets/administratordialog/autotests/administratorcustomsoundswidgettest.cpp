/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundswidgettest.h"
#include "administratordialog/customsounds/administratorcustomsoundswidget.h"
#include "model/admincustomsoundmodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include <KTreeWidgetSearchLineWidget>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AdministratorCustomSoundsWidgetTest)
AdministratorCustomSoundsWidgetTest::AdministratorCustomSoundsWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomSoundsWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomSoundsWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mModel = w.findChild<AdminCustomSoundModel *>(QStringLiteral("mAdminCustomSoundModel"));
    QVERIFY(mModel);

    auto mProxyModelModel = w.findChild<SearchTreeBaseFilterProxyModel *>(QStringLiteral("mCustomSoundProxyModel"));
    QVERIFY(mProxyModelModel);
}
