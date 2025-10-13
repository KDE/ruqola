/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacentralwidgettest.h"
#include "config-ruqola.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqolacentralwidget.h"
#if 0
#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#endif
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
#include <TextAddonsWidgets/WhatsNewMessageWidget>
#else
#include "whatsnew/whatsnewmessagewidget.h"
#endif
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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainlayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);

#if 0 // Create on demand => it will be nullptr
    auto mServerErrorInfoMessageWidget = w.findChild<ServerErrorInfoMessageWidget *>(u"mServerErrorInfoMessageWidget"_s);
    QVERIFY(mServerErrorInfoMessageWidget);
#endif
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    auto whatsNewMessageWidget = w.findChild<TextAddonsWidgets::WhatsNewMessageWidget *>(u"whatsNewMessageWidget"_s);
#else
    auto whatsNewMessageWidget = w.findChild<WhatsNewMessageWidget *>(u"whatsNewMessageWidget"_s);
#endif
    if (WhatsNewTranslations().lastNewFeatures().isEmpty()) {
        QVERIFY(!whatsNewMessageWidget);
    }
}

#include "moc_ruqolacentralwidgettest.cpp"
