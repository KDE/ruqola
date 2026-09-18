/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <KActionMenu>

#include "libruqolawidgets_private_export.h"
class QActionGroup;
class KActionCollection;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit ServerMenu(QWidget *parent = nullptr);
    ~ServerMenu() override;

    void setActionCollection(KActionCollection *ac);

    void slotUpdateAccountMenu();

private:
    KActionCollection *mActionCollection = nullptr;
    QActionGroup *const mActionGroup;
};
