/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "showimagewidget.h"
#include <QDialog>
class RocketChatAccount;
class QMenu;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowImageDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowImageDialog() override;

    void setImageInfo(const ShowImageWidget::ImageInfo &info);

private:
    void readConfig();
    void writeConfig();
    ShowImageWidget *const mShowImageWidget;
    QMenu *const mClipboardMenu;
    QAction *mClipboardImageAction = nullptr;
};

