/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT OauthTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit OauthTreeView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~OauthTreeView() override;

Q_SIGNALS:
    void removeOauth(const QString &identifier);
    void oauthAdded(const QJsonObject &replyObject);

private:
    void initialize();
    void removeClicked(const QString &identifier);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void addClicked();
    void editClicked(const QModelIndex &index);
    RocketChatAccount *const mRocketChatAccount;
};
