/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "conferenceinfodialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mConferenceInfoWidget = d.findChild<ConferenceInfoWidget *>(u"mConferenceInfoWidget"_s);
    QVERIFY(mConferenceInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(d.conferenceId().isEmpty());
}

#include "moc_conferenceinfodialogtest.cpp"
