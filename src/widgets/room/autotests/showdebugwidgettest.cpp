/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showdebugwidgettest.h"
#include "misc/jsonplaintexteditwidget.h"
#include "room/debugdialog/showdebugwidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowDebugWidgetTest)
using namespace Qt::Literals::StringLiterals;
ShowDebugWidgetTest::ShowDebugWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ShowDebugWidgetTest::shouldHaveDefaultValues()
{
    const ShowDebugWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget);
}

#include "moc_showdebugwidgettest.cpp"
