/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavaddserverwidgettest.h"
#include "webdav/webdavaddserverwidget.h"
#include <QFormLayout>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(WebDavAddServerWidgetTest)
WebDavAddServerWidgetTest::WebDavAddServerWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WebDavAddServerWidgetTest::shouldHaveDefaultValues()
{
    const WebDavAddServerWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());
}
