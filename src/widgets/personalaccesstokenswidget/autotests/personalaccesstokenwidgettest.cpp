/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenwidgettest.h"
#include "personalaccesstokenswidget/personalaccesstokenwidget.h"
#include <QTest>
QTEST_MAIN(PersonalAccessTokenWidgetTest)
PersonalAccessTokenWidgetTest::PersonalAccessTokenWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PersonalAccessTokenWidgetTest::shouldHaveDefaultValues()
{
    PersonalAccessTokenWidget w(nullptr);
    // TODO
}
