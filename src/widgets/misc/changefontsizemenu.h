/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KActionMenu>

class ChangeFontSizeMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit ChangeFontSizeMenu(QObject *parent = nullptr);
    ~ChangeFontSizeMenu() override;

Q_SIGNALS:
    void fontChanged();

private:
    void changeFontSize(int offset);
};
