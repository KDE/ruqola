/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidgettest.h"
#include "administratordialog/moderationconsole/moderationmessageinfowidget.h"
#include <QTest>
QTEST_MAIN(ModerationMessageInfoWidgetTest)
ModerationMessageInfoWidgetTest::ModerationMessageInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationMessageInfoWidgetTest::shouldHaveDefaultValues()
{
    ModerationMessageInfoWidget d;
    // TODO
}

#include "moc_moderationmessageinfowidgettest.cpp"
