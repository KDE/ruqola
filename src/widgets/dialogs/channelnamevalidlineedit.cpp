/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlineedit.h"
#include "rocketchataccount.h"

#include <KColorScheme>
#include <KStatefulBrush>

ChannelNameValidLineEdit::ChannelNameValidLineEdit(RocketChatAccount *account, QWidget *parent)
    : SearchWithDelayLineEdit(parent)
    , mRocketChatAccount(account)
{
    setDelayMs(std::chrono::milliseconds(500));
    setPlaceholderText(QString());
    connect(this, &ChannelNameValidLineEdit::searchRequested, this, &ChannelNameValidLineEdit::slotSearchChannelRequested);
    connect(this, &ChannelNameValidLineEdit::searchCleared, this, &ChannelNameValidLineEdit::clearLineEdit);
    if (mRocketChatAccount) {
        connect(mRocketChatAccount->ddp(), &DDPClient::result, this, &ChannelNameValidLineEdit::slotSearchDone);
        auto mChannelNameValidLineEditValidator =
            new ChannelNameValidLineEditValidator(QRegularExpression(account->ruqolaServerConfig()->channelNameValidation()), this);
        connect(mChannelNameValidLineEditValidator, &ChannelNameValidLineEditValidator::textIsValid, this, &ChannelNameValidLineEdit::slotTextIsValid);
        setValidator(mChannelNameValidLineEditValidator);
    }
}

ChannelNameValidLineEdit::~ChannelNameValidLineEdit() = default;

void ChannelNameValidLineEdit::slotTextIsValid(bool state)
{
    if (text().isEmpty()) {
        Q_EMIT channelIsValid(ChannelNameValidLineEdit::ChannelNameStatus::Unknown);
    } else {
        Q_EMIT channelIsValid(state ? ChannelNameValidLineEdit::ChannelNameStatus::Valid : ChannelNameValidLineEdit::ChannelNameStatus::InvalidCharacters);
    }
}

void ChannelNameValidLineEdit::clearLineEdit()
{
    updateStyleSheet(true);
    Q_EMIT channelIsValid(ChannelNameValidLineEdit::ChannelNameStatus::Unknown);
}

void ChannelNameValidLineEdit::slotSearchChannelRequested(const QString &text)
{
    if (!text.trimmed().isEmpty()) {
        mDdpIdentifier = mRocketChatAccount->ddp()->roomNameExists(text);
    } else {
        clearLineEdit();
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

ChannelNameValidLineEditValidator::ChannelNameValidLineEditValidator(const QRegularExpression &re, QObject *parent)
    : QRegularExpressionValidator(re, parent)
{
}

QValidator::State ChannelNameValidLineEditValidator::validate(QString &input, int &pos) const
{
    const QValidator::State state = QRegularExpressionValidator::validate(input, pos);
    Q_EMIT textIsValid(state == QValidator::State::Acceptable);
    return state;
}

ChannelNameValidLineEditValidator::~ChannelNameValidLineEditValidator() = default;
