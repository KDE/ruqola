/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>

class LIBRUQOLAWIDGETS_TESTS_EXPORT OauthTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit OauthTreeView(QWidget *parent = nullptr);
    ~OauthTreeView() override;

Q_SIGNALS:
    void removeInvite(const QString &identifier);

private:
    void initialize();
    void removeClicked(const QString &identifier);
    void slotCustomContextMenuRequested(const QPoint &pos);
};
