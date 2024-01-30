/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

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
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    LIBRUQOLAWIDGETS_NO_EXPORT void removeClicked(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void addClicked();
    LIBRUQOLAWIDGETS_NO_EXPORT void editClicked(const QModelIndex &index);
    RocketChatAccount *const mRocketChatAccount;
};
