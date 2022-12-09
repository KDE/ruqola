/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallwidgettest.h"
#include "conferencecalldialog/conferencecallwidget.h"
#include <QTest>
QTEST_MAIN(ConferenceCallWidgetTest)
ConferenceCallWidgetTest::ConferenceCallWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceCallWidgetTest::shouldHaveDefaultValues()
{
    ConferenceCallWidget w(nullptr);
    // TODO
}
