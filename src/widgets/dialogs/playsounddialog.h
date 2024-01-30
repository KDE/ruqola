/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class RocketChatAccount;
class PlaySoundWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PlaySoundDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlaySoundDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PlaySoundDialog() override;
    void setAudioPath(const QString &url);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateTitle(const QUrl &url);
    PlaySoundWidget *const mSoundWidget;
};
