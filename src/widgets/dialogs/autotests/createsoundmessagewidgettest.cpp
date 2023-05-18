/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidgettest.h"
#include "dialogs/createsoundmessagewidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateSoundMessageWidgetTest)
CreateSoundMessageWidgetTest::CreateSoundMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CreateSoundMessageWidgetTest::shouldHaveDefaultValues()
{
    // TODO
    CreateSoundMessageWidget w;
}
