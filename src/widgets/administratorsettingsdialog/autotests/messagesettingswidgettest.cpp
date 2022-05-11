/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidgettest.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include <QTest>
QTEST_MAIN(MessageSettingsWidgetTest)

MessageSettingsWidgetTest::MessageSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MessageSettingsWidgetTest::shouldHaveDefaultValues()
{
    MessageSettingsWidget w(nullptr);
    // TODO
}
