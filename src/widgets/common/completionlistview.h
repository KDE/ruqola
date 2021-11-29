/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QListView>

/**
 * Completion popup for nicks, channels, etc.
 * QCompleter does the filtering itself... so we need to implement our own popup, instead.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT CompletionListView : public QListView
{
    Q_OBJECT
public:
    CompletionListView();
    ~CompletionListView() override;

    void setTextWidget(QWidget *textWidget);

    void setModel(QAbstractItemModel *model) override;

    void slotCompletionAvailable();

Q_SIGNALS:
    void complete(const QModelIndex &currentIndex);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool event(QEvent *event) override;

private:
    QWidget *mTextWidget = nullptr;
};

