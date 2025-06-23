/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "viewlogwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/logs/viewlogplaintextedit.h"
#include "administratordialog/logs/viewlogwidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ViewLogWidgetTest)

ViewLogWidgetTest::ViewLogWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ViewLogWidgetTest::shouldHaveDefaultValues()
{
    ViewLogWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mPlainTextEdit = w.findChild<ViewLogPlainTextEdit *>(u"mPlainTextEdit"_s);
    QVERIFY(mPlainTextEdit);
    QVERIFY(mPlainTextEdit->isReadOnly());
}

#include "moc_viewlogwidgettest.cpp"
