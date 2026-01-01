/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "soundconfigurewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/soundconfigurewidget.h"
#include <QTest>
QTEST_MAIN(SoundConfigureWidgetTest)
SoundConfigureWidgetTest::SoundConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void SoundConfigureWidgetTest::shouldHaveDefaultValues()
{
    SoundConfigureWidget w(nullptr);
    // TODO
}

#include "moc_soundconfigurewidgettest.cpp"
