/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavaddserverwidgettest.h"
#include "webdav/webdavaddserverwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
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

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    auto mUrl = w.findChild<QLineEdit *>(u"mUrl"_s);
    QVERIFY(mUrl);
    QVERIFY(mUrl->text().isEmpty());
    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mPasswordLineEdit"_s);
    QVERIFY(mPasswordLineEdit);

    QVERIFY(!w.addServerInfo().isValid());
}

#include "moc_webdavaddserverwidgettest.cpp"
