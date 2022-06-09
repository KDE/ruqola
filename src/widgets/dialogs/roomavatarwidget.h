/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QPushButton>
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomAvatarWidget : public QPushButton
{
    Q_OBJECT
public:
    explicit RoomAvatarWidget(QWidget *parent = nullptr);
    ~RoomAvatarWidget() override;

    Q_REQUIRED_RESULT QString roomAvatar() const;

    Q_REQUIRED_RESULT bool wasChanged() const;

    void setCurrentIconPath(const QString &currentPath);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void slotCancel();
    void slotChangeImage();
    void slotResetAvatar();
    QString mCurrentIconPath;
    QString mRoomAvatarPath;
    bool mWasChanged = false;
};
