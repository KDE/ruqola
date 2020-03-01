/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "managerdatapaths.h"
#include "ruqola.h"
#include "rocketchataccount.h"

#include <QFile>
#include <QPixmap>
#include <QStandardPaths>
#include <QTest>

MessageAttachment testAttachment()
{
    MessageAttachment msgAttach;
    const QString title = QStringLiteral("This is the title");
    msgAttach.setTitle(title);
    const QString description = QStringLiteral("A description");
    msgAttach.setDescription(description);
    QPixmap pix(50, 100);
    pix.fill(Qt::white);
    // Save the pixmap directly into the cache so that no download hpapens
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, Ruqola::self()->rocketChatAccount()->accountName());
    const QString link = QStringLiteral("/testfile.png");
    const QString pixFileName = cachePath + link;
    pix.save(pixFileName, "png");
    msgAttach.setLink(link);
    return msgAttach;
}

QUrl avatarLink()
{
    // Save the pixmap directly into the cache so that no download hpapens
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, Ruqola::self()->rocketChatAccount()->accountName());
    const QString link = QStringLiteral("/avatarpix.png");
    const QString pixFileName = cachePath + link;

    if (!QFileInfo::exists(pixFileName)) {
        const QString srcPath = QFINDTESTDATA("../../../../src/icons/32-apps-ruqola.png");
        Q_ASSERT(!srcPath.isEmpty());
        if (!QFile::copy(srcPath, pixFileName)) {
            qWarning() << "Couldn't copy" << srcPath << "to" << pixFileName;
        }
    }

    return QUrl(QLatin1String("https://example.com") + link);
}
