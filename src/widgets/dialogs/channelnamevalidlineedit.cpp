/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelnamevalidlineedit.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"

#include "connection.h"
#include "rooms/roomsnameexistsjob.h"
#include "ruqolaserverconfig.h"
#include "ruqolawidgets_debug.h"

#include <KColorScheme>
#include <KStatefulBrush>
#include <chrono>
using namespace std::chrono_literals;

ChannelNameValidLineEdit::ChannelNameValidLineEdit(RocketChatAccount *account, QWidget *parent)
    : SearchWithDelayLineEdit(parent)
    , mRocketChatAccount(account)
{
    setDelayMs(500ms);
    setPlaceholderText(QString());
    connect(this, &ChannelNameValidLineEdit::searchRequested, this, &ChannelNameValidLineEdit::slotSearchChannelRequested);
    connect(this, &ChannelNameValidLineEdit::searchCleared, this, &ChannelNameValidLineEdit::clearLineEdit);
    if (mRocketChatAccount) {
        auto mChannelNameValidLineEditValidator =
            new ChannelNameValidLineEditValidator(QRegularExpression(mRocketChatAccount->ruqolaServerConfig()->channelNameValidation()), this);
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
    auto job = new RocketChatRestApi::RoomsNameExistsJob(this);
    job->setRoomName(text.trimmed());
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::RoomsNameExistsJob::roomNameExistsDone, this, [this](bool exists) {
        emitIsValid(!exists);
    });

    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RoomsNameExistsJob job";
    }
}

void ChannelNameValidLineEdit::updateStyleSheet(bool state)
{
#ifndef QT_NO_STYLE_STYLESHEET
    QString styleSheet;
    if (mNegativeBackground.isEmpty()) {
        const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeBackground);
        mNegativeBackground = u"QLineEdit{ background-color:%1 }"_s.arg(bgBrush.brush(palette()).color().name());
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

#include "moc_channelnamevalidlineedit.cpp"
