/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "readonlylineeditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "room/readonlylineeditwidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ReadOnlyLineEditWidgetTest)
ReadOnlyLineEditWidgetTest::ReadOnlyLineEditWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ReadOnlyLineEditWidgetTest::shouldHaveDefaultValues()
{
    ReadOnlyLineEditWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(label->text().isEmpty());

    QFont font = label->font();
    QVERIFY(font.bold());
    QVERIFY(font.italic());
}

void ReadOnlyLineEditWidgetTest::shouldChangeText()
{
    ReadOnlyLineEditWidget w;
    auto label = w.findChild<QLabel *>(u"label"_s);
    QString str{u"text text"_s};
    w.setMessage(str);
    QCOMPARE(label->text(), str);

    str = u"bla"_s;
    w.setMessage(str);
    QCOMPARE(label->text(), str);
}

#include "moc_readonlylineeditwidgettest.cpp"
