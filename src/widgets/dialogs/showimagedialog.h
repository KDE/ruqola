/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
    LIBRUQOLAWIDGETS_NO_EXPORT void updateServiceList();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenWith(QAction *action);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateMenu(const ShowImageWidget::ImageInfo &info);
    KService::List mServiceList;
    ShowImageWidget *const mShowImageWidget;
    QMenu *const mClipboardMenu;
    QToolButton *const mOpenWithButton;
    QMenu *const mOpenWithMenu;
    QAction *mClipboardImageAction = nullptr;
    RocketChatAccount *const mRocketChatAccount;
};
