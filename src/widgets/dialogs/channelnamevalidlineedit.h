/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/searchwithdelaylineedit.h"

#include <QJsonDocument>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelNameValidLineEditValidator : public QRegularExpressionValidator
{
    Q_OBJECT
public:
    explicit ChannelNameValidLineEditValidator(const QRegularExpression &re, QObject *parent = nullptr);
    ~ChannelNameValidLineEditValidator() override;
    Q_REQUIRED_RESULT QValidator::State validate(QString &input, int &pos) const override;
Q_SIGNALS:
    void textIsValid(bool isValid) const;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelNameValidLineEdit : public SearchWithDelayLineEdit
{
    Q_OBJECT
public:
    enum ChannelNameStatus {
        Unknown = 0,
        Valid = 1,
        AlreadyExistingName = 2,
        InvalidCharacters = 3,
    };
    explicit ChannelNameValidLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelNameValidLineEdit() override;
Q_SIGNALS:
    void channelIsValid(ChannelNameValidLineEdit::ChannelNameStatus status);

private:
    void updateStyleSheet(bool state);
    void slotSearchDone(quint64 id, const QJsonDocument &result);
    void slotSearchChannelRequested(const QString &str);
    void clearLineEdit();
    void emitIsValid(bool state);
    void slotTextIsValid(bool state);
    QString mNegativeBackground;
    quint64 mDdpIdentifier = 0;
    QRegularExpression mRegularExpression;
    RocketChatAccount *const mRocketChatAccount;
};
