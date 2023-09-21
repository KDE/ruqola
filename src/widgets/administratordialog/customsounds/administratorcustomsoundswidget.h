/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
class QWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomSoundsWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorCustomSoundsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorCustomSoundsWidget() override;

protected:
    void updateLabel() override;
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;

private:
    [[nodiscard]] QString displayShowMessage() const;
    void slotModifyCustomSound(const QModelIndex &index);
    void slotAddCustomSound();
    void slotRemoveCustomSound(const QModelIndex &index);
    void slotCustomSoundRemoved(const QString &identifier);
    void slotCustomSoundAdded();
    void slotCustomSoundUpdated();
};
