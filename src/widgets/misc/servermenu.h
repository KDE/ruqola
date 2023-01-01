/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <KActionMenu>

#include "libruqolawidgets_private_export.h"
class KActionCollection;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit ServerMenu(QWidget *parent = nullptr);
    ~ServerMenu() override;

    void setActionCollection(KActionCollection *ac);

private:
    void slotUpdateAccountMenu();
    KActionCollection *mActionCollection = nullptr;
};
