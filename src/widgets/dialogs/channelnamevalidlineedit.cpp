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
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <KColorScheme>

ChannelNameValidLineEdit::ChannelNameValidLineEdit(RocketChatAccount *account, QWidget *parent)
    : SearchWithDelayLineEdit(parent)
    , mRocketChatAccount(account)
{
    setDelayMs(500);
    setPlaceholderText(QString());
    connect(this, &ChannelNameValidLineEdit::searchRequested, this, &ChannelNameValidLineEdit::slotSearchChannelRequested);
    connect(this, &ChannelNameValidLineEdit::searchCleared, this, [this] {
        updateStyleSheet(true);
    });
    if (mRocketChatAccount) {
        connect(mRocketChatAccount->ddp(), &DDPClient::result, this, &ChannelNameValidLineEdit::slotSearchDone);
    }
}

ChannelNameValidLineEdit::~ChannelNameValidLineEdit() = default;

void ChannelNameValidLineEdit::slotSearchChannelRequested(const QString &text)
{
    if (!text.trimmed().isEmpty()) {
        mDdpIdentifier = mRocketChatAccount->ddp()->roomNameExists(text);
    } else {
        updateStyleSheet(true);
    }
}

void ChannelNameValidLineEdit::slotSearchDone(quint64 id, const QJsonDocument &result)
{
    if (id == mDdpIdentifier) {
        const QJsonObject objresult = result.object();
        const auto resultValue = objresult.value(QLatin1String("result"));
        if (!resultValue.isUndefined()) {
            emitIsValid(!resultValue.toBool());
        }
    }
}

void ChannelNameValidLineEdit::updateStyleSheet(bool state)
{
#ifndef QT_NO_STYLE_STYLESHEET
    QString styleSheet;
    if (mNegativeBackground.isEmpty()) {
        const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeBackground);
        mNegativeBackground = QStringLiteral("QLineEdit{ background-color:%1 }").arg(bgBrush.brush(palette()).color().name());
    }
    if (!state) {
        styleSheet = mNegativeBackground;
    }
    setStyleSheet(styleSheet);
#endif
}

void ChannelNameValidLineEdit::emitIsValid(bool state)
{
    updateStyleSheet(state);
    Q_EMIT channelIsValid(state);
}
