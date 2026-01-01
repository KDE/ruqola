/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserswidgettest.h"
#include "administratordialog/moderationconsole/moderationuserswidget.h"
#include <QTest>

QTEST_MAIN(ModerationUsersWidgetTest)
ModerationUsersWidgetTest::ModerationUsersWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationUsersWidgetTest::shouldHaveDefaultValues()
{
    ModerationUsersWidget w(nullptr);
    // TODO
}

#include "moc_moderationuserswidgettest.cpp"
