/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemaximumsizedialogtest.h"
#include "messagemaximumsizedialog/messagemaximumsizedialog.h"
#include "messagemaximumsizedialog/messagemaximumsizewidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MessageMaximumSizeDialogTest)

MessageMaximumSizeDialogTest::MessageMaximumSizeDialogTest(QObject *parent)
    : QObject{parent}
{
}

void MessageMaximumSizeDialogTest::shouldHaveDefaultValues()
{
    MessageMaximumSizeDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mMessageMaximumSizeWidget = d.findChild<MessageMaximumSizeWidget *>(QStringLiteral("mMessageMaximumSizeWidget"));
    QVERIFY(mMessageMaximumSizeWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QPushButton *buttonOk = button->button(QDialogButtonBox::Ok);
    QVERIFY(buttonOk);
    QVERIFY(!buttonOk->isEnabled());

    QVERIFY(d.description().isEmpty());
    QVERIFY(d.fileName().isEmpty());
}
