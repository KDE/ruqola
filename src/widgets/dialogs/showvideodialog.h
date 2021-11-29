/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ShowVideoWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowVideoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowVideoDialog(QWidget *parent = nullptr);
    ~ShowVideoDialog() override;
    void setVideoUrl(const QUrl &videoPath);

private:
    void readConfig();
    void writeConfig();
    ShowVideoWidget *const mShowVideoWidget;
};

