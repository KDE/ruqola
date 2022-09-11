/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
#include <QWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountManageDeviceConfigureWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit MyAccountManageDeviceConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountManageDeviceConfigureWidget() override;

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    void slotDeviceRemoved(const QString &emojiId);
    void slotDisconnectDevice(const QModelIndex &index);
    Q_REQUIRED_RESULT QString displayShowMessage() const;
};
