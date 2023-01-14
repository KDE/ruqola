/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoMessageHistoryListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit ServerErrorInfoMessageHistoryListView(QWidget *parent = nullptr);
    ~ServerErrorInfoMessageHistoryListView() override;

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
};
