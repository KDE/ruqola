/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class SearchWithDelayLineEdit;
class QTreeView;
class DirectoryBaseModel;
class QVBoxLayout;
class DirectoryBaseFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchTreeBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchTreeBaseWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchTreeBaseWidget() override;

    void initialize();
Q_SIGNALS:
    void loadMoreElements();

protected:
    virtual void slotCustomContextMenuRequested(const QPoint &pos) = 0;
    virtual void updateLabel() = 0;
    virtual void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) = 0;
    void slotLoadMoreElementDone(const QJsonObject &obj);
    void slotSearchDone(const QJsonObject &obj);
    void connectModel();
    void hideColumns();
    void finishSearching();
    void resizeColumToContents();
    QLabel *const mLabelResultSearch;
    SearchWithDelayLineEdit *const mSearchLineEdit;
    QTreeView *const mTreeView;
    DirectoryBaseModel *mModel = nullptr;
    DirectoryBaseFilterProxyModel *mProxyModelModel = nullptr;
    QVBoxLayout *mSearchLayout = nullptr;
    RocketChatAccount *const mRocketChatAccount;
    Q_REQUIRED_RESULT QString clickableStr() const;

private:
    void slotSearchCleared();
    void slotSearchRequested(const QString &str);
    void slotLoadMoreElements();
};
