/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavaddserverdialogtest.h"
#include "webdav/webdavaddserverdialog.h"
#include "webdav/webdavaddserverwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(WebDavAddServerDialogTest)

WebDavAddServerDialogTest::WebDavAddServerDialogTest(QObject *parent)
    : QObject{parent}
{
}

void WebDavAddServerDialogTest::shouldHaveDefaultValues()
{
    const WebDavAddServerDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mWebDavAddServerWidget = d.findChild<WebDavAddServerWidget *>(u"mWebDavAddServerWidget"_s);
    QVERIFY(mWebDavAddServerWidget);

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);

    QVERIFY(!d.addServerInfo().isValid());
}

#include "moc_webdavaddserverdialogtest.cpp"
