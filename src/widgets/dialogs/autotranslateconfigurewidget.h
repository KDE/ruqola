/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT Room *room() const;
    void setRoom(Room *room);

private:
    void slotAutoTranslateChanged();
    void slotChangeAutoTranslate(bool status);
    void slotLanguageChanged(int index);
    void slotAutoTranslateLanguageChanged();
    QCheckBox *const mAutoTranslate;
    QComboBox *const mLanguageComboBox;
    QPointer<Room> mRoom;
    RocketChatAccount *const mRocketChatAccount;
};
