/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "modifystatuswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/modifystatuswidget.h"
#include "misc/statuscombobox.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(ModifyStatusWidgetTest)

ModifyStatusWidgetTest::ModifyStatusWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ModifyStatusWidgetTest::shouldHaveDefaultValues()
{
    ModifyStatusWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStatusCombobox = w.findChild<StatusCombobox *>(u"mStatusCombobox"_s);
    QVERIFY(mStatusCombobox);

    auto mStatusLineEdit = w.findChild<QLineEdit *>(u"mStatusLineEdit"_s);
    QVERIFY(mStatusLineEdit);
    QVERIFY(mStatusLineEdit->isClearButtonEnabled());

    QVERIFY(w.messageStatus().isEmpty());
}

void ModifyStatusWidgetTest::changeStatusMessage()
{
    ModifyStatusWidget w;
    const QString messages = u"messages"_s;
    w.setMessageStatus(messages);
    QCOMPARE(w.messageStatus(), messages);
}

#include "moc_modifystatuswidgettest.cpp"
