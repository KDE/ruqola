/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class SearchWithDelayLineEdit;
class SearchChannelListView;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchChannelWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchChannelWidget() override;

private:
    void slotOpenChannel(const QModelIndex &index);
    void slotTextChanged(const QString &str);
    void slotSearchCleared();
    SearchWithDelayLineEdit *const mSearchLineEdit;
    SearchChannelListView *const mResultListWidget;
    RocketChatAccount *const mRocketChatAccount;
};

