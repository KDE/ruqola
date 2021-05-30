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

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLabel;
class QLineEdit;
class QTreeView;
class QJsonObject;
class DirectoryBaseModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryWidget : public QWidget
{
    Q_OBJECT
public:
    enum DirectoryType {
        Unknown = 0,
        Room = 1,
        User = 2,
        Team = 3,
    };
    explicit DirectoryWidget(DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryWidget() override;

    void fillDirectory();

    Q_REQUIRED_RESULT DirectoryType type() const;

private:
    void slotSearchDone(const QJsonObject &obj);
    void loadElements(int offset = -1, int count = -1);
    void loadMoreElements();
    void slotLoadMoreElementDone(const QJsonObject &obj);
    void updateLabel();
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotOpen(const QModelIndex &index);
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    Q_REQUIRED_RESULT QString noFoundInfo() const;
    const DirectoryType mType;
    QLabel *const mLabelResultSearch;
    QLineEdit *const mSearchLineEdit;
    QTreeView *const mTreeView;
    DirectoryBaseModel *mModel = nullptr;
};
