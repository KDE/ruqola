/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferenceinfowidgettest.h"
#include "conferencecalldialog/conferenceinfowidget.h"
#include <QFormLayout>
#include <QTest>

QTEST_MAIN(ConferenceInfoWidgetTest)
ConferenceInfoWidgetTest::ConferenceInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceInfoWidgetTest::shouldHaveDefaultValues()
{
    ConferenceInfoWidget w(nullptr);
    QVERIFY(w.conferenceId().isEmpty());
    auto mFormLayout = w.findChild<QFormLayout *>(QStringLiteral("mFormLayout"));
    QVERIFY(mFormLayout);
    QCOMPARE(mFormLayout->contentsMargins(), QMargins{});
}

#include "moc_conferenceinfowidgettest.cpp"
