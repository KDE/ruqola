/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacentralwidgettest.h"
#include "ruqolacentralwidget.h"
#if 0
#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#endif
#include "whatsnew/whatsnewmessagewidget.h"
#include "whatsnew/whatsnewtranslations.h"

#include <QStackedWidget>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

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

#if 0 // Create on demand => it will be nullptr
    auto mServerErrorInfoMessageWidget = w.findChild<ServerErrorInfoMessageWidget *>(QStringLiteral("mServerErrorInfoMessageWidget"));
    QVERIFY(mServerErrorInfoMessageWidget);
#endif
    auto whatsNewMessageWidget = w.findChild<WhatsNewMessageWidget *>(QStringLiteral("whatsNewMessageWidget"));
    if (WhatsNewTranslations().lastNewFeatures().isEmpty())
        QVERIFY(!whatsNewMessageWidget);
    else
        QVERIFY(whatsNewMessageWidget);
}

#include "moc_ruqolacentralwidgettest.cpp"
