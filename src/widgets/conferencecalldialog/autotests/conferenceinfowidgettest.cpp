/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfowidgettest.h"
#include "conferencecalldialog/conferenceinfowidget.h"
#include <QTest>

QTEST_MAIN(ConferenceInfoWidgetTest)
ConferenceInfoWidgetTest::ConferenceInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceInfoWidgetTest::shouldHaveDefaultValues()
{
    ConferenceInfoWidget w(nullptr);
    // TODO
}
