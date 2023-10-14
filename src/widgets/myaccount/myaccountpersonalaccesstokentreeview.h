/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPersonalAccessTokenTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit MyAccountPersonalAccessTokenTreeView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountPersonalAccessTokenTreeView() override;

Q_SIGNALS:
    void removeToken(const QString &tokenName);
    void createToken();
    void regenerateToken(const QString &tokenName);

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void removeClicked(const QString &tokenName);
};
