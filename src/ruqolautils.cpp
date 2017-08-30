/*

 * Copyright 2017  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ruqolautils.h"
#include "ruqola_debug.h"
#include "utils.h"
#include <QDesktopServices>
#include <QUrl>

RuqolaUtils::RuqolaUtils(QObject *parent)
    : QObject(parent)
{
}

RuqolaUtils *RuqolaUtils::self()
{
    static RuqolaUtils s_self;
    return &s_self;
}

QString RuqolaUtils::markdownToRichText(const QString &markDown)
{
    return Utils::markdownToRichText(markDown);
}

void RuqolaUtils::openUrl(const QString &url)
{
    const QUrl clickedUrl = QUrl::fromUserInput(url);
    if (clickedUrl.scheme() == QStringLiteral("ruqola")) {
        if (clickedUrl.path().startsWith(QLatin1String("/room/"))) {
            qDebug() << " room clicked ";
        } else if (clickedUrl.path().startsWith(QLatin1String("/user/"))){
            qDebug() << " clickedUrl.path() " << clickedUrl.path();
        } else {
            qDebug() << " Unknown url " << url;
        }
    } else {
        QDesktopServices::openUrl(clickedUrl);
    }
}
