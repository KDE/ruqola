/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "configurenotificationdialogtest.h"
#include "dialogs/configurenotificationdialog.h"
#include "dialogs/configurenotificationwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureNotificationDialogTest)
ConfigureNotificationDialogTest::ConfigureNotificationDialogTest(QObject *parent)
    : QObject(parent)
{

}

void ConfigureNotificationDialogTest::shouldHaveDefaultValues()
{
    ConfigureNotificationDialog w;
    QVERIFY(w.windowTitle().isEmpty());
    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    ConfigureNotificationWidget *mConfigureNoticationWidget = w.findChild<ConfigureNotificationWidget *>(QStringLiteral("mConfigureNoticationWidget"));
    QVERIFY(mConfigureNoticationWidget);

    QDialogButtonBox *buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);
}
