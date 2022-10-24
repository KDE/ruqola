/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emailsettingswidgettest.h"
#include "administratorsettingsdialog/email/emailsettingswidget.h"
#include <QTest>
QTEST_MAIN(EmailSettingsWidgetTest)
EmailSettingsWidgetTest::EmailSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EmailSettingsWidgetTest::shouldHaveDefaultValues()
{
    EmailSettingsWidget w(nullptr);
    // TODO
}
