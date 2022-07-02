/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircfederationwidgettest.h"
#include "administratorsettingsdialog/ircfederation/ircfederationwidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTest>

QTEST_MAIN(IrcFederationWidgetTest)
IrcFederationWidgetTest::IrcFederationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void IrcFederationWidgetTest::shouldHaveDefaultValues()
{
    IrcFederationWidget w(nullptr);
    // TODO
}
