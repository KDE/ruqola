/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>

class LIBRUQOLAWIDGETS_TESTS_EXPORT InviteTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit InviteTreeView(QWidget *parent = nullptr);
    ~InviteTreeView() override;

Q_SIGNALS:
    void removeInvite(const QString &identifier);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize();
    LIBRUQOLAWIDGETS_NO_EXPORT void removeClicked(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
};
