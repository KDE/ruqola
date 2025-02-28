/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "commandpreviewwidgettest.h"
#include "room/commandpreviewwidget.h"
#include <QTest>
QTEST_MAIN(CommandPreviewWidgetTest)

CommandPreviewWidgetTest::CommandPreviewWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CommandPreviewWidgetTest::shouldHaveDefaultValues()
{
    CommandPreviewWidget w;
    // TODO
}
