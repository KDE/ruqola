/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "testdata.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <QFile>
#include <QPixmap>
#include <QStandardPaths>
#include <QTest>

void initTestAccount()
{
    QStandardPaths::setTestModeEnabled(true);
    Ruqola::self()->rocketChatAccount()->setAccountName(QStringLiteral("accountName"));
}

MessageAttachment testAttachment()
{
    MessageAttachment msgAttach;
    msgAttach.setAttachmentType(MessageAttachment::Image);
    const QString title = QStringLiteral("This is the title");
    msgAttach.setTitle(title);
    const QString description = QStringLiteral("A description");
    msgAttach.setDescription(description);
    QPixmap pix(50, 100);
    pix.fill(Qt::white);
    // Save the pixmap directly into the cache so that no download happens
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, Ruqola::self()->rocketChatAccount()->accountName());
    QDir().mkpath(cachePath);
    const QString link = QStringLiteral("/testfile.png");
    const QString pixFileName = cachePath + link;
    pix.save(pixFileName, "png");
    msgAttach.setLink(link);
    msgAttach.setImageUrlPreview(link);
    return msgAttach;
}

QUrl avatarLink()
{
    // Save the pixmap directly into the cache so that no download happens
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, Ruqola::self()->rocketChatAccount()->accountName());
    QDir().mkpath(cachePath);
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
