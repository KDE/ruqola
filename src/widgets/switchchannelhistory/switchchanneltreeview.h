/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class SwitchChannelDelegate;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SwitchChannelTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SwitchChannelTreeView(QWidget *parent = nullptr);
    ~SwitchChannelTreeView() override;

    [[nodiscard]] int sizeHintWidth() const;
    void resizeColumnsToContents();

    void setCurrentRocketChatAccount(RocketChatAccount *newCurrentRocketChatAccount);

Q_SIGNALS:
    void channelSelected(const QModelIndex &index);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    SwitchChannelDelegate *const mSwitchChannelDelegate;
};
