/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
class QWidget;
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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeviceRemoved(const QString &emojiId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDisconnectDevice(const QModelIndex &index);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessage() const;
};
