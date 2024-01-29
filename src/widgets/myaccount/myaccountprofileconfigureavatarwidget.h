/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QPushButton>
#include <QWidget>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AvatarImage : public QPushButton
{
    Q_OBJECT
public:
    explicit AvatarImage(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AvatarImage() override;

    void setCurrentIconPath(const QString &currentPath);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void changeUrl();
    LIBRUQOLAWIDGETS_NO_EXPORT void changeImage();
    LIBRUQOLAWIDGETS_NO_EXPORT void resetAvatar();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);
    QString mCurrentIconPath;
    RocketChatAccount *const mRocketChatAccount;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountProfileConfigureAvatarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountProfileConfigureAvatarWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountProfileConfigureAvatarWidget() override;
    void setCurrentIconPath(const QString &currentPath);

private:
    AvatarImage *const mAvatarImage;
};
