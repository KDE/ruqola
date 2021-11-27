/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "administratorroomseditbasewidget.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class RoomAvatarWidget;
class MessageTextEditor;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRoomsEditWidget : public AdministratorRoomsEditBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorRoomsEditWidget(QWidget *parent = nullptr);
    ~AdministratorRoomsEditWidget() override;

    void setRoomEditInfo(const RoomEditInfo &newRoomEditInfo) override;
    Q_REQUIRED_RESULT RoomEditInfo roomEditInfo() const override;

private:
    QLineEdit *const mLineEdit;
    QCheckBox *const mDefaultCheckBox;
    QCheckBox *const mFavoriteCheckBox;
    QCheckBox *const mFeaturedCheckBox;
    QCheckBox *const mPrivateCheckBox;
    QCheckBox *const mReadOnlyCheckBox;
    QCheckBox *const mArchivedCheckBox;

    RoomAvatarWidget *const mRoomAvatarWidget;
    MessageTextEditor *const mComment;
    MessageTextEditor *const mAnnouncement;
    MessageTextEditor *const mDescription;
};
