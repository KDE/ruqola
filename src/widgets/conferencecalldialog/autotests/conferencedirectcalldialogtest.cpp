/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencedirectcalldialogtest.h"
#include "conferencecalldialog/conferencecallwidget.h"
#include "conferencecalldialog/conferencedirectcalldialog.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConferenceDirectCallDialogTest)
ConferenceDirectCallDialogTest::ConferenceDirectCallDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceDirectCallDialogTest::shouldHaveDefaultValues()
{
    ConferenceDirectCallDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mConferenceCallWidget = d.findChild<ConferenceCallWidget *>(QStringLiteral("mConferenceCallWidget"));
    QVERIFY(mConferenceCallWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    //    auto info = d.startInfo();
    //    QVERIFY(!info.useCamera);
    //    QVERIFY(!info.useMic);
}

#include "moc_conferencedirectcalldialogtest.cpp"
