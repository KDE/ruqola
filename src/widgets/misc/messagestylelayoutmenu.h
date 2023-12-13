/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <KActionMenu>

class MessageStyleLayoutMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit MessageStyleLayoutMenu(QObject *parent = nullptr);
    ~MessageStyleLayoutMenu() override;

Q_SIGNALS:
    void styleChanged();
};
