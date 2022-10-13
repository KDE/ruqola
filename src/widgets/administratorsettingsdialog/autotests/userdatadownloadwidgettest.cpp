/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userdatadownloadwidgettest.h"
#include "administratorsettingsdialog/userdatadownload/userdatadownloadwidget.h"
#include <QTest>
QTEST_MAIN(UserDataDownloadWidgetTest)

UserDataDownloadWidgetTest::UserDataDownloadWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void UserDataDownloadWidgetTest::shouldHaveDefaultValues()
{
    UserDataDownloadWidget w(nullptr);
    // TODO
}
