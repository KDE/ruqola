/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "modifystatuswidgettest.h"
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
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStatusCombobox = w.findChild<StatusCombobox *>(QStringLiteral("mStatusCombobox"));
    QVERIFY(mStatusCombobox);

    auto mStatusLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mStatusLineEdit"));
    QVERIFY(mStatusLineEdit);
    QVERIFY(mStatusLineEdit->isClearButtonEnabled());

    QVERIFY(w.messageStatus().isEmpty());
}

void ModifyStatusWidgetTest::changeStatusMessage()
{
    ModifyStatusWidget w;
    const QString messages = QStringLiteral("messages");
    w.setMessageStatus(messages);
    QCOMPARE(w.messageStatus(), messages);
}
