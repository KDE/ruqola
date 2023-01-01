/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorroomseditbasewidget.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class RoomAvatarWidget;
class MessageTextEditor;
class QLabel;
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
    QLabel *const mOwnerName;

    RoomAvatarWidget *const mRoomAvatarWidget;
    MessageTextEditor *const mTopic;
    MessageTextEditor *const mAnnouncement;
    MessageTextEditor *const mDescription;
};
