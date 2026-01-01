/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "directorywidget.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class DirectoryStackedWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryContainerWidget(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryContainerWidget() override;

    void slotJoin();

Q_SIGNALS:
    void updateJoinButton(bool status);

private:
    DirectoryStackedWidget *const mDirectoryStackedWidget;
};
