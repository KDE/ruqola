/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testdata.h"
using namespace Qt::Literals::StringLiterals;

#include "accountmanager.h"
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
    Ruqola::self()->accountManager()->addAccount(new RocketChatAccount());
    Ruqola::self()->rocketChatAccount()->setAccountName(u"accountName"_s);
}

MessageAttachment testAttachment()
{
    MessageAttachment msgAttach;
    msgAttach.setAttachmentType(MessageAttachment::AttachmentType::Image);
    const QString title = u"This is the title"_s;
    msgAttach.setTitle(title);
    const QString description = u"A description"_s;
    msgAttach.setDescription(description);
    QPixmap pix(50, 100);
    pix.fill(Qt::white);
    // Save the pixmap directly into the cache so that no download happens
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, Ruqola::self()->rocketChatAccount()->accountName());
    QDir().mkpath(cachePath);
    const QString link = u"/testfile.png"_s;
    const QString pixFileName = cachePath + link;
    pix.save(pixFileName, "png");
    msgAttach.setLink(link);
    msgAttach.setImageUrlPreview(link);
    msgAttach.generateTitle();
    return msgAttach;
}

QUrl avatarLink()
{
    // Save the pixmap directly into the cache so that no download happens
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, Ruqola::self()->rocketChatAccount()->accountName());
    QDir().mkpath(cachePath);
    const QString link = u"/avatarpix.png"_s;
    const QString pixFileName = cachePath + link;

    if (!QFileInfo::exists(pixFileName)) {
        const QString srcPath = QFINDTESTDATA("../../../../src/icons/32-apps-ruqola.png");
        Q_ASSERT(!srcPath.isEmpty());
        if (!QFile::copy(srcPath, pixFileName)) {
            qWarning() << "Couldn't copy" << srcPath << "to" << pixFileName;
        }
    }

    return QUrl("https://example.com"_L1 + link);
}
