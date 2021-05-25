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
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryWidget : public QWidget
{
    Q_OBJECT
public:
    enum DirectoryType { Unknown = 0, Room = 1, User = 2, Team = 3 };
    explicit DirectoryWidget(DirectoryType type, QWidget *parent = nullptr);
    ~DirectoryWidget() override;

private:
    const DirectoryType mType;
    QLabel *const mLabelResultSearch;
    QLineEdit *const mSearchLineEdit;
};
