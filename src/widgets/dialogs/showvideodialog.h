/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ShowVideoWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowVideoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowVideoDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowVideoDialog() override;
    void setVideoPath(const QString &videoPath);

private:
    void slotUpdateTitle(const QUrl &url);
    void readConfig();
    void writeConfig();
    ShowVideoWidget *const mShowVideoWidget;
};
