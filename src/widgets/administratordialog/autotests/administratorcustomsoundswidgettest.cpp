/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mModel = w.findChild<AdminCustomSoundModel *>(u"mAdminCustomSoundModel"_s);
    QVERIFY(mModel);

    auto mProxyModelModel = w.findChild<SearchTreeBaseFilterProxyModel *>(u"mCustomSoundProxyModel"_s);
    QVERIFY(mProxyModelModel);
}

#include "moc_administratorcustomsoundswidgettest.cpp"
