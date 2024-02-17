/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class SearchWithDelayLineEdit;
class QTreeView;
class CustomBaseModel;
class QVBoxLayout;
class SearchTreeBaseFilterProxyModel;
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
    void doubleClicked(const QModelIndex &index);

protected:
    virtual void slotCustomContextMenuRequested(const QPoint &pos) = 0;
    virtual void updateLabel() = 0;
    virtual void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) = 0;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreElementDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void connectModel();
    LIBRUQOLAWIDGETS_NO_EXPORT void hideColumns();
    LIBRUQOLAWIDGETS_NO_EXPORT void finishSearching();
    LIBRUQOLAWIDGETS_NO_EXPORT void resizeColumToContents();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString clickableStr() const;
    QLabel *const mLabelResultSearch;
    SearchWithDelayLineEdit *const mSearchLineEdit;
    QTreeView *const mTreeView;
    CustomBaseModel *mModel = nullptr;
    SearchTreeBaseFilterProxyModel *mProxyModelModel = nullptr;
    QVBoxLayout *mSearchLayout = nullptr;
    RocketChatAccount *const mRocketChatAccount;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchCleared();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchRequested(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreElements();
};
