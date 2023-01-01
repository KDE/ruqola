/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagedialogtest.h"
#include "dialogs/createvideomessagedialog.h"
#include "dialogs/createvideomessagewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateVideoMessageDialogTest)

CreateVideoMessageDialogTest::CreateVideoMessageDialogTest(QObject *parent)
    : QObject(parent)
{
}

void CreateVideoMessageDialogTest::shouldHaveDefaultValues()
{
    CreateVideoMessageDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateVideoMessageWidget = w.findChild<CreateVideoMessageWidget *>(QStringLiteral("mCreateVideoMessageWidget"));
    QVERIFY(mCreateVideoMessageWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
