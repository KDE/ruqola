/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createsoundmessagedialogtest.h"
#include "dialogs/createsoundmessagedialog.h"
#include "dialogs/createsoundmessagewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateSoundMessageDialogTest)
CreateSoundMessageDialogTest::CreateSoundMessageDialogTest(QObject *parent)
    : QObject{parent}
{
}

void CreateSoundMessageDialogTest::shouldHaveDefaultValues()
{
    CreateSoundMessageDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateSoundMessageWidget = w.findChild<CreateSoundMessageWidget *>(QStringLiteral("mCreateSoundMessageWidget"));
    QVERIFY(mCreateSoundMessageWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
