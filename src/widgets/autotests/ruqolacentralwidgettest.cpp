/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacentralwidgettest.h"
#include "ruqolacentralwidget.h"
#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#include "whatsnew/whatsnewmessagewidget.h"

#include <QHBoxLayout>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(RuqolaCentralWidgetTest)
RuqolaCentralWidgetTest::RuqolaCentralWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RuqolaCentralWidgetTest::shouldHaveDefaultValues()
{
    RuqolaCentralWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);

    auto mServerErrorInfoMessageWidget = w.findChild<ServerErrorInfoMessageWidget *>(QStringLiteral("mServerErrorInfoMessageWidget"));
    QVERIFY(mServerErrorInfoMessageWidget);

    auto whatsNewMessageWidget = w.findChild<WhatsNewMessageWidget *>(QStringLiteral("whatsNewMessageWidget"));
    QVERIFY(whatsNewMessageWidget);
}

#include "moc_ruqolacentralwidgettest.cpp"
