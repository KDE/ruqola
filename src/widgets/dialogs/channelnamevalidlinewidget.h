/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelnamevalidlineedit.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelNameValidLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelNameValidLineWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelNameValidLineWidget() override;

    [[nodiscard]] QString text() const;
Q_SIGNALS:
    void channelIsValid(bool valid);

private:
    void slotChannelIsValid(ChannelNameValidLineEdit::ChannelNameStatus isValid);
    ChannelNameValidLineEdit *const mChannelNameValidLineEdit;
    QLabel *const mChannelNameLabel;
    QString mValidCharacters;
};
