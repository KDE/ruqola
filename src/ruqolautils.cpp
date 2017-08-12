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
#include <KTextToHTML>
#include <QDesktopServices>
#include <QUrl>

RuqolaUtils *RuqolaUtils::m_self = nullptr;

RuqolaUtils::RuqolaUtils(QObject *parent)
    : QObject(parent)
{
}

RuqolaUtils *RuqolaUtils::self()
{
    if (!m_self) {
        m_self = new RuqolaUtils;
    }
    return m_self;
}

QString RuqolaUtils::markdownToRichText(const QString &markDown)
{
    //qCDebug(RUQOLA_LOG) << "BEFORE markdownToRichText "<<markDown;
    //Problem with smileys as qml load image as qrc:/.... so perhaps we need to change contextProperty("baseUrl"...)
    const KTextToHTML::Options convertFlags = KTextToHTML::PreserveSpaces | KTextToHTML::HighlightText /* | KTextToHTML::ReplaceSmileys*/;
    const QString str = KTextToHTML::convertToHtml(markDown, convertFlags);
    //qCDebug(RUQOLA_LOG) << "markdownToRichText "<<str;
    return str;
}

void RuqolaUtils::openUrl(const QString &url)
{
    if (!url.startsWith(QStringLiteral("ruqola:"))) {
        QDesktopServices::openUrl(QUrl(url));
    } else {
        qDebug() << " url " << url;
    }
}
