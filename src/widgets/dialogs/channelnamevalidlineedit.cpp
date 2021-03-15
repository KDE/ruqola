/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "channelnamevalidlineedit.h"
#include "misc/directoryjob.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KColorScheme>

ChannelNameValidLineEdit::ChannelNameValidLineEdit(QWidget *parent)
    : SearchWithDelayLineEdit(parent)
{
    connect(this, &ChannelNameValidLineEdit::searchRequested, this, &ChannelNameValidLineEdit::slotSearchChannelRequested);
}

ChannelNameValidLineEdit::~ChannelNameValidLineEdit()
{
}

void ChannelNameValidLineEdit::slotSearchChannelRequested(const QString &text)
{
    if (!text.trimmed().isEmpty()) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::DirectoryJob(this);
        RocketChatRestApi::DirectoryJob::DirectoryInfo info;
        info.pattern = text;
        info.searchType = RocketChatRestApi::DirectoryJob::Room;
        job->setDirectoryInfo(info);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &ChannelNameValidLineEdit::slotSearchDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
        }
    } else {
        Q_EMIT channelIsValid(true);
    }
}

void ChannelNameValidLineEdit::slotSearchDone(const QJsonObject &obj)
{
    const QJsonArray rooms = obj.value(QLatin1String("result")).toArray();
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        qDebug() << " o " << o;
        if (channel.roomName() == text()) {
            Q_EMIT emitIsValid(false);
            return;
        }
    }
    Q_EMIT emitIsValid(true);
}

void ChannelNameValidLineEdit::emitIsValid(bool state)
{
#ifndef QT_NO_STYLE_STYLESHEET
    QString styleSheet;
    if (mNegativeBackground.isEmpty()) {
        const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeBackground);
        mNegativeBackground = QStringLiteral("QLineEdit{ background-color:%1 }").arg(bgBrush.brush(this).color().name());
    }
    if (!state) {
        styleSheet = mNegativeBackground;
    }
    setStyleSheet(styleSheet);
#endif
    Q_EMIT channelIsValid(state);
}
