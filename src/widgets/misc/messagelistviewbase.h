/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QListView>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListViewBase : public QListView
{
    Q_OBJECT
public:
    explicit MessageListViewBase(QWidget *parent = nullptr);
    ~MessageListViewBase() override;

    void checkIfAtBottom();
    void updateVerticalPageStep();
    void maybeScrollToBottom();

protected:
    void resizeEvent(QResizeEvent *ev) override;

private:
    bool mAtBottom = true;
};
