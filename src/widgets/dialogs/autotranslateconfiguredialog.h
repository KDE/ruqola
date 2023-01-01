/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class AutoTranslateConfigureWidget;
class Room;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoTranslateConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AutoTranslateConfigureDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AutoTranslateConfigureDialog() override;
    void setRoom(Room *room);

private:
    AutoTranslateConfigureWidget *const mAutoTranslateConfigureWidget;
};
