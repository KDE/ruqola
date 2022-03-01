/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    Q_EMIT channelIsValid(state ? ChannelNameValidLineEdit::ChannelNameStatus::Valid : ChannelNameValidLineEdit::ChannelNameStatus::AlreadyExistingName);
}
