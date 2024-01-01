/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "readonlylineeditwidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(label->text().isEmpty());

    QFont font = label->font();
    QVERIFY(font.bold());
    QVERIFY(font.italic());
}

void ReadOnlyLineEditWidgetTest::shouldChangeText()
{
    ReadOnlyLineEditWidget w;
    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QString str{QStringLiteral("text text")};
    w.setMessage(str);
    QCOMPARE(label->text(), str);

    str = QStringLiteral("bla");
    w.setMessage(str);
    QCOMPARE(label->text(), str);
}

#include "moc_readonlylineeditwidgettest.cpp"
