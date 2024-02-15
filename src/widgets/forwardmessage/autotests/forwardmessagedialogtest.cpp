/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagedialogtest.h"
#include "forwardmessage/forwardmessagedialog.h"
#include "forwardmessage/forwardmessagewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ForwardMessageDialogTest)
ForwardMessageDialogTest::ForwardMessageDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ForwardMessageDialogTest::shouldHaveDefaultValues()
{
    ForwardMessageDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mForwardMessageWidget = d.findChild<ForwardMessageWidget *>(QStringLiteral("mForwardMessageWidget"));
    QVERIFY(mForwardMessageWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_forwardmessagedialogtest.cpp"
