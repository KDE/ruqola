/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QListView>
class LIBRUQOLAWIDGETS_TESTS_EXPORT RecentUsedEmoticonView : public QListView
{
    Q_OBJECT
public:
    explicit RecentUsedEmoticonView(QWidget *parent = nullptr);
    ~RecentUsedEmoticonView() override;

Q_SIGNALS:
    void clearAll();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

