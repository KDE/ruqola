/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QLabel;
class QListView;
class DiscussionsFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowDiscussionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowDiscussionsWidget(QWidget *parent = nullptr);
    ~ShowDiscussionsWidget() override;

    void setModel(DiscussionsFilterProxyModel *model);

Q_SIGNALS:
    void loadMoreDiscussion();

private:
    void slotSearchMessageTextChanged(const QString &str);
    void updateLabel();
    Q_REQUIRED_RESULT QString displayShowDiscussionInRoom() const;
    QLineEdit *const mSearchDiscussionLineEdit;
    QLabel *const mDiscussionInfoLabel;
    QListView *const mListDiscussions;
    DiscussionsFilterProxyModel *mDiscussionModel = nullptr;
};

