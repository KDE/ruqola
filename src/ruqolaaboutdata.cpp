/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "ruqolaaboutdata.h"

#include <KAboutData>
#include <KLocalizedString>
#include <QDebug>

RuqolaAboutData::RuqolaAboutData(QObject *parent)
    : QObject(parent)
{

}

RuqolaAboutData::~RuqolaAboutData()
{

}

QString RuqolaAboutData::version() const
{
    return KAboutData::applicationData().version();
}

QString RuqolaAboutData::title() const
{
    return i18n("<html><font size=\"5\">%1</font><br />Version %2</html>", KAboutData::applicationData().displayName(), KAboutData::applicationData().version());
}

QString RuqolaAboutData::about() const
{
    //Set up the first page...
    QString aboutPageText = KAboutData::applicationData().shortDescription() + QLatin1Char('\n');

    if (!KAboutData::applicationData().otherText().isEmpty()) {
        aboutPageText += QLatin1Char('\n') + KAboutData::applicationData().otherText() + QLatin1Char('\n');
    }

    if (!KAboutData::applicationData().copyrightStatement().isEmpty()) {
        aboutPageText += QLatin1Char('\n') + KAboutData::applicationData().copyrightStatement() + QLatin1Char('\n');
    }

    if (!KAboutData::applicationData().homepage().isEmpty()) {
        aboutPageText += QLatin1Char('\n') + QStringLiteral("<a href=\"%1\">%1</a>").arg(KAboutData::applicationData().homepage()) + QLatin1Char('\n');
    }
    aboutPageText = aboutPageText.trimmed();
    return aboutPageText;
}

QString RuqolaAboutData::licenseText(const QString &val)
{
    int license = val.toInt();
    if (license >= 0) {
        return KAboutData::applicationData().licenses().at(license).text();
    }
    return {};
}

QString RuqolaAboutData::licenses() const
{
    QString licensesStr;
    const int licenseCount = KAboutData::applicationData().licenses().count();
    for (int i = 0; i < licenseCount; ++i) {
        const KAboutLicense license = KAboutData::applicationData().licenses().at(i);
        licensesStr = QStringLiteral("<a href=\"%1\">%2</a>").arg(QString::number(i),
                                  i18n("License: %1", license.name(KAboutLicense::FullName)));
    }
    return licensesStr;
}
