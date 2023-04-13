/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
#include <TextEmoticonsWidgets/EmoticonTextEditSelector>
class RocketChatAccount;
class CustomEmojiIconManager;
class EmoticonMenuWidget : public TextEmoticonsWidgets::EmoticonTextEditSelector
{
    Q_OBJECT
public:
    explicit EmoticonMenuWidget(QWidget *parent = nullptr);
    ~EmoticonMenuWidget() override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

protected:
    QSize sizeHint() const override;

private:
    CustomEmojiIconManager *const mCustomEmojiIconManager;
};
