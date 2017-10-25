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
#include "kcoreaddons_version.h"
#include "ruqolaaboutdataauthormodel.h"

#include <KAboutData>
#include <KLocalizedString>
#include <QGuiApplication>
#include <QDebug>
#include <QUrl>

RuqolaAboutData::RuqolaAboutData(QObject *parent)
    : QObject(parent)
    , mAboutData(KAboutData::applicationData())
{    
    mAuthorModel = new RuqolaAboutDataAuthorModel(this);
}

RuqolaAboutData::~RuqolaAboutData()
{

}

QString RuqolaAboutData::version() const
{
    return mAboutData.version();
}

QString RuqolaAboutData::title() const
{
    return i18n("<html><font size=\"5\">%1</font><br />Version %2</html>", mAboutData.displayName(), mAboutData.version());
}

QString RuqolaAboutData::about() const
{
    //Set up the first page...
    QString aboutPageText = mAboutData.shortDescription() + QLatin1Char('\n');

    if (!mAboutData.otherText().isEmpty()) {
        aboutPageText += QLatin1Char('\n') + mAboutData.otherText() + QLatin1Char('\n');
    }

    if (!mAboutData.copyrightStatement().isEmpty()) {
        aboutPageText += QLatin1Char('\n') + mAboutData.copyrightStatement() + QLatin1Char('\n');
    }

    if (!mAboutData.homepage().isEmpty()) {
        aboutPageText += QLatin1Char('\n') + QStringLiteral("<a href=\"%1\">%1</a>").arg(mAboutData.homepage()) + QLatin1Char('\n');
    }
    aboutPageText = aboutPageText.trimmed();
    return aboutPageText;
}

QString RuqolaAboutData::licenseText(const QString &val)
{
    int license = val.toInt();
    if (license >= 0) {
        return mAboutData.licenses().at(license).text();
    }
    return {};
}

QString RuqolaAboutData::licenses() const
{
    QString licensesStr;
    const int licenseCount = mAboutData.licenses().count();
    for (int i = 0; i < licenseCount; ++i) {
        const KAboutLicense license = mAboutData.licenses().at(i);
        licensesStr = QStringLiteral("<a href=\"%1\">%2</a>").arg(QString::number(i),
                                  i18n("License: %1", license.name(KAboutLicense::FullName)));
    }
    return licensesStr;
}

QString RuqolaAboutData::libraries() const
{
    return i18n("<ul><li>KDE Frameworks %1</li><li>Qt %2 (built against %3)</li><li>The <em>%4</em> windowing system</li></ul>",
                QStringLiteral(KCOREADDONS_VERSION_STRING),
                QString::fromLocal8Bit(qVersion()),
                QStringLiteral(QT_VERSION_STR),
                QGuiApplication::platformName());
}

QString RuqolaAboutData::reportBugs() const
{
    if (!mAboutData.customAuthorTextEnabled() || !mAboutData.customAuthorRichText().isEmpty()) {
        if (!mAboutData.customAuthorTextEnabled()) {
            const QString bugAddress = mAboutData.bugAddress();
            if (bugAddress.isEmpty() || bugAddress == QLatin1String("submit@bugs.kde.org")) {
                return i18n("Please use <a href=\"https://bugs.kde.org\">https://bugs.kde.org</a> to report bugs.\n");
            } else {
                QUrl bugUrl(bugAddress);
                if (bugUrl.scheme().isEmpty()) {
                    bugUrl.setScheme(QLatin1String("mailto"));
                }
                return i18n("Please report bugs to <a href=\"%1\">%2</a>.\n",
                                        bugUrl.toString(), bugAddress);
            }
        } else {
            return mAboutData.customAuthorRichText();
        }
    }
    return {};
}

RuqolaAboutDataAuthorModel *RuqolaAboutData::authorModel() const
{
    return mAuthorModel;
}
