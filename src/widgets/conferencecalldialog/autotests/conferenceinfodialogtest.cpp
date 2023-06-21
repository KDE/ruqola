/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "conferenceinfodialogtest.h"
#include "conferencecalldialog/conferenceinfodialog.h"
#include "conferencecalldialog/conferenceinfowidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ConferenceInfoDialogTest)
ConferenceInfoDialogTest::ConferenceInfoDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceInfoDialogTest::shouldHaveDefaultValues()
{
    ConferenceInfoDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mConferenceInfoWidget = d.findChild<ConferenceInfoWidget *>(QStringLiteral("mConferenceInfoWidget"));
    QVERIFY(mConferenceInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QVERIFY(d.conferenceId().isEmpty());
}

#include "moc_conferenceinfodialogtest.cpp"
