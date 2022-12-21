/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "conferenceinfodialogtest.h"
#include "conferencecalldialog/conferenceinfodialog.h"
#include <QTest>
QTEST_MAIN(ConferenceInfoDialogTest)
ConferenceInfoDialogTest::ConferenceInfoDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ConferenceInfoDialogTest::shouldHaveDefaultValues()
{
    ConferenceInfoDialog d(nullptr);
    // TODO
}
