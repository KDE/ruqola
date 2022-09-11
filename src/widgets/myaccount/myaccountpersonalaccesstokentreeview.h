/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPersonalAccessTokenTreeView : public QTreeView
{
public:
    explicit MyAccountPersonalAccessTokenTreeView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountPersonalAccessTokenTreeView() override;

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void addClicked();
    void removeClicked(const QString &identifier);
    void editClicked();
    RocketChatAccount *const mRocketAccount;
};
