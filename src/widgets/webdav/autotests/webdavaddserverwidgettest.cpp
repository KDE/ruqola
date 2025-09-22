/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavaddserverwidgettest.h"
#include "webdav/webdavaddserverwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QSignalSpy>
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

void WebDavAddServerWidgetTest::shouldChangeEnableButtonStatus()
{
    const WebDavAddServerWidget w;
    QSignalSpy spyUpdateButton(&w, &WebDavAddServerWidget::okButtonEnabled);

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    auto mUrl = w.findChild<QLineEdit *>(u"mUrl"_s);
    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mPasswordLineEdit"_s);

    mName->setText(u"dd"_s);
    QVERIFY(spyUpdateButton.count() == 1);
    QCOMPARE(spyUpdateButton.at(0).at(0), false);

    spyUpdateButton.clear();
    mUrl->setText(u"dd"_s);
    QVERIFY(spyUpdateButton.count() == 1);
    QCOMPARE(spyUpdateButton.at(0).at(0), false);

    spyUpdateButton.clear();
    mUserName->setText(u"foo"_s);
    QVERIFY(spyUpdateButton.count() == 1);
    QCOMPARE(spyUpdateButton.at(0).at(0), false);

    spyUpdateButton.clear();
    mPasswordLineEdit->setPassword(u"AAAA"_s);
    QVERIFY(spyUpdateButton.count() == 1);
    QCOMPARE(spyUpdateButton.at(0).at(0), true);

    // Empty
    spyUpdateButton.clear();
    mName->setText(u" "_s);
    QVERIFY(spyUpdateButton.count() == 1);
    QCOMPARE(spyUpdateButton.at(0).at(0), false);
}

#include "moc_webdavaddserverwidgettest.cpp"
