/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircwidgettest.h"
#include "administratorsettingsdialog/irc/ircwidget.h"
#include "settingswidgetshelper.h"
#include <QTest>

QTEST_MAIN(IrcWidgetTest)

IrcWidgetTest::IrcWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void IrcWidgetTest::shouldHaveDefaultValues()
{
    IrcWidget w(nullptr);
    // TODO
}
