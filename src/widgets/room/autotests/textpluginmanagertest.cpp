/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textpluginmanagertest.h"
#include <QTest>
QTEST_MAIN(TextPluginManagerTest)

TextPluginManagerTest::TextPluginManagerTest(QObject *parent)
    : QObject(parent)
{
}

#include "moc_textpluginmanagertest.cpp"
