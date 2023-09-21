/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString roomAvatar() const;

    [[nodiscard]] bool wasChanged() const;

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
