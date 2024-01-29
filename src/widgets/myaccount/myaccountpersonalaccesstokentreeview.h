/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountPersonalAccessTokenTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit MyAccountPersonalAccessTokenTreeView(QWidget *parent = nullptr);
    ~MyAccountPersonalAccessTokenTreeView() override;

Q_SIGNALS:
    void removeToken(const QString &tokenName);
    void createToken();
    void regenerateToken(const QString &tokenName);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void removeClicked(const QString &tokenName);
};
