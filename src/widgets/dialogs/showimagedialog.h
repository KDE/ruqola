/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "showimagewidget.h"
#include <KService>
#include <QDialog>
class RocketChatAccount;
class QMenu;
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowImageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowImageDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowImageDialog() override;

    void setImageInfo(const ShowImageWidget::ImageInfo &info);

    void showImages(const QByteArray &fileId, const QByteArray &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateServiceList(const ShowImageWidget::ImageInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenWith(QAction *action);
    KService::List mServiceList;
    ShowImageWidget *const mShowImageWidget;
    QMenu *const mClipboardMenu;
    QToolButton *const mOpenWithButton;
    QMenu *const mOpenWithMenu;
    QAction *mClipboardImageAction = nullptr;
};
