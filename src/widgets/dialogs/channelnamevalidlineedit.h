/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/searchwithdelaylineedit.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelNameValidLineEditValidator : public QRegularExpressionValidator
{
    Q_OBJECT
public:
    explicit ChannelNameValidLineEditValidator(const QRegularExpression &re, QObject *parent = nullptr);
    ~ChannelNameValidLineEditValidator() override;
    [[nodiscard]] QValidator::State validate(QString &input, int &pos) const override;
Q_SIGNALS:
    void textIsValid(bool isValid) const;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelNameValidLineEdit : public SearchWithDelayLineEdit
{
    Q_OBJECT
public:
    enum class ChannelNameStatus : uint8_t {
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
    LIBRUQOLAWIDGETS_NO_EXPORT void updateStyleSheet(bool state);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchChannelRequested(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void clearLineEdit();
    LIBRUQOLAWIDGETS_NO_EXPORT void emitIsValid(bool state);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextIsValid(bool state);
    QString mNegativeBackground;
    QRegularExpression mRegularExpression;
    RocketChatAccount *const mRocketChatAccount;
};
