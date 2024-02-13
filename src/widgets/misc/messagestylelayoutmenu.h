/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "ruqolaglobalconfig.h"
#include <KActionMenu>
class QActionGroup;
class MessageStyleLayoutMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit MessageStyleLayoutMenu(QObject *parent = nullptr);
    ~MessageStyleLayoutMenu() override;

Q_SIGNALS:
    void styleChanged();

private:
    void addMessageStyleAction(const QString &name, RuqolaGlobalConfig::EnumMessageStyle::type status);
    QActionGroup *const mActionGroup;
};
