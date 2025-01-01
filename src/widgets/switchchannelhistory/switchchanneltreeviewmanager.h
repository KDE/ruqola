/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_export.h"
#include <QList>
#include <QObject>
#include <QPointer>
class SwitchChannelTreeView;
class QAction;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_EXPORT SwitchChannelTreeViewManager : public QObject
{
    Q_OBJECT
public:
    explicit SwitchChannelTreeViewManager(QObject *parent = nullptr);
    ~SwitchChannelTreeViewManager() override;

    void addActions(const QList<QAction *> &lst);

    [[nodiscard]] QWidget *parentWidget() const;
    void setParentWidget(QWidget *newParentWidget);

    void selectForward();
    void selectBackward();

    [[nodiscard]] SwitchChannelTreeView *switchChannelTreeView() const;

    [[nodiscard]] RocketChatAccount *currentRocketChatAccount() const;
    void setCurrentRocketChatAccount(RocketChatAccount *newCurrentRocketChatAccount);

Q_SIGNALS:
    void switchToChannel(const QByteArray &identifier);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateViewGeometry();
    LIBRUQOLAWIDGETS_NO_EXPORT void activateChannel(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void switchToCollectionClicked(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void selectChannel(const int from, const int to);
    QWidget *mParentWidget = nullptr;
    SwitchChannelTreeView *const mSwitcherChannelTreeView;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
