/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class SearchWithDelayLineEdit;
class QTreeView;
class DirectoryBaseModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchTreeBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchTreeBaseWidget(QWidget *parent = nullptr);
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
    QLabel *const mLabelResultSearch;
    SearchWithDelayLineEdit *const mSearchLineEdit;
    QTreeView *const mTreeView;
    DirectoryBaseModel *mModel = nullptr;

private:
    void slotSearchCleared();
    void slotSearchRequested(const QString &str);
    void slotLoadMoreElements();
};
