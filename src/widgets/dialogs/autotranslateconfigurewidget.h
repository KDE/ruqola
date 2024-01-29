/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QPointer>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QComboBox;
class Room;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoTranslateConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AutoTranslateConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AutoTranslateConfigureWidget() override;

    [[nodiscard]] Room *room() const;
    void setRoom(Room *room);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAutoTranslateChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChangeAutoTranslate(bool status);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLanguageChanged(int index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAutoTranslateLanguageChanged();
    QCheckBox *const mAutoTranslate;
    QComboBox *const mLanguageComboBox;
    QPointer<Room> mRoom;
    RocketChatAccount *const mRocketChatAccount;
};
