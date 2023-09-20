/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationinfotest.h"
#include "moderation/moderationinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(ModerationInfoTest)
ModerationInfoTest::ModerationInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationInfoTest::shouldHaveDefaultValues()
{
    ModerationInfo i;
    // TODO
}

#include "moc_moderationinfotest.cpp"
