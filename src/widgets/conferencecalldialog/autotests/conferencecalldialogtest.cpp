/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecalldialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "conferencecalldialog/conferencecalldialog.h"
#include "conferencecalldialog/conferencecallwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConferenceCallDialogTest)
ConferenceCallDialogTest::ConferenceCallDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceCallDialogTest::shouldHaveDefaultValues()
{
    ConferenceCallDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mConferenceCallWidget = d.findChild<ConferenceCallWidget *>(u"mConferenceCallWidget"_s);
    QVERIFY(mConferenceCallWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto info = d.conferenceCallInfo();
    QVERIFY(!info.useCamera);
    QVERIFY(!info.useMic);
}

#include "moc_conferencecalldialogtest.cpp"
