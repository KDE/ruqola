/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showsoundwidgettest.h"
#include "dialogs/showsoundwidget.h"
#include <QTest>
QTEST_MAIN(ShowSoundWidgetTest)
ShowSoundWidgetTest::ShowSoundWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ShowSoundWidgetTest::shouldHaveDefaultValues()
{
    ShowSoundWidget w;
    // TODO
}
