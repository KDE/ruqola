/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>

class PlaySoundWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PlaySoundDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlaySoundDialog(QWidget *parent = nullptr);
    ~PlaySoundDialog() override;
    void setAudioUrl(const QUrl &url);

private:
    PlaySoundWidget *const mSoundWidget;
};
