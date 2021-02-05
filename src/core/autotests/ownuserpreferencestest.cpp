/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "ownuserpreferencestest.h"
#include "ownuserpreferences.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(OwnUserPreferencesTest)
OwnUserPreferencesTest::OwnUserPreferencesTest(QObject *parent)
    : QObject(parent)
{
}

void OwnUserPreferencesTest::shouldHaveDefaultValues()
{
    OwnUserPreferences p;
    QVERIFY(p.highlightWords().isEmpty());
    QVERIFY(p.emailNotificationMode().isEmpty());
    QVERIFY(p.desktopNotifications().isEmpty());
    QVERIFY(p.mobileNotifications().isEmpty());
    QVERIFY(p.convertAsciiEmoji());
    QVERIFY(p.useEmojis());
}

void OwnUserPreferencesTest::shouldLoadOwnUserPreferencesInfo_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<OwnUserPreferences>("ownUserPreferences");
    {
        OwnUserPreferences preferences;
        QTest::addRow("empty") << QStringLiteral("empty") << preferences;
    }
    {
        OwnUserPreferences preferences;
        preferences.setEmailNotificationMode(QStringLiteral("mentions"));
        preferences.setMobileNotifications(QStringLiteral("nothing"));
        preferences.setDesktopNotifications(QStringLiteral("mentions"));
        preferences.setConvertAsciiEmoji(false);
        preferences.setUseEmojis(false);
        QTest::addRow("ownuserpreferences1") << QStringLiteral("ownuserpreferences1") << preferences;
    }
}

void OwnUserPreferencesTest::shouldLoadOwnUserPreferencesInfo()
{
    QFETCH(QString, fileName);
    QFETCH(OwnUserPreferences, ownUserPreferences);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/ownuserpreferences/") + fileName + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    OwnUserPreferences r;
    r.parsePreferences(obj);
    const bool result = (r == ownUserPreferences);
    if (!result) {
        qDebug() << "result " << r;
        qDebug() << "expected " << ownUserPreferences;
    }
    QVERIFY(result);
}
