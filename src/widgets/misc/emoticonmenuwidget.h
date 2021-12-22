/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QTabWidget;
class SearchWithDelayLineEdit;
class QListView;
class RocketChatAccount;
class EmoticonRecentUsedFilterProxyModel;
class EmoticonModelFilterProxyModel;
class EmoticonCustomModelFilterProxyModel;
class RecentUsedEmoticonView;
class EmoticonCategoryModelFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonMenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmoticonMenuWidget(QWidget *parent = nullptr);
    ~EmoticonMenuWidget() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void loadRecentUsed();
Q_SIGNALS:
    void insertEmoticons(const QString &emoticon);

private:
    void slotInsertEmoticons(const QString &identifier);
    void initializeTab(RocketChatAccount *account);
    void slotSearchTextChanged(const QString &text);

    SearchWithDelayLineEdit *const mSearchLineEdit;
    QTabWidget *const mTabWidget;
    EmoticonRecentUsedFilterProxyModel *const mRecentUsedFilterProxyModel;
    EmoticonModelFilterProxyModel *const mEmoticonFilterProxyModel;
    EmoticonCustomModelFilterProxyModel *const mEmoticonCustomFilterProxyModel;
    QListView *const mSearchEmojisView;
    RecentUsedEmoticonView *const mRecentUsedEmoticonView;
    QListView *const mCustomEmojiView;
    QList<EmoticonCategoryModelFilterProxyModel *> mEmoticonListViews;
    int mAllTabIndex = -1;
};

