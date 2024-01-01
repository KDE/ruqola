/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorroomseditbasewidget.h"
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorDirectRoomsEditWidget : public AdministratorRoomsEditBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorDirectRoomsEditWidget(QWidget *parent = nullptr);
    ~AdministratorDirectRoomsEditWidget() override;

    [[nodiscard]] RoomEditInfo roomEditInfo() const override;
    void setRoomEditInfo(const RoomEditInfo &newRoomEditInfo) override;

private:
    QLineEdit *const mLineEdit;
    QCheckBox *const mDefaultCheckBox;
    QCheckBox *const mFavoriteCheckBox;
    QCheckBox *const mFeaturedCheckBox;
};
