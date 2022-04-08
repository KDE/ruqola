/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QListView;
class QLineEdit;

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationHistoryWidget(QWidget *parent = nullptr);
    ~NotificationHistoryWidget() override;

Q_SIGNALS:
    void openMessage(); // TODO add arguments

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotShowMessage(const QModelIndex &index);
    void slotClearList();
    QListView *const mListNotifications;
    QLineEdit *const mSearchLineEdit;
};
