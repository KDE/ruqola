/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

class QWidget;
#include <TextEmoticonsWidgets/EmoticonTextEditSelector>
class RocketChatAccount;
class EmoticonMenuWidget : public TextEmoticonsWidgets::EmoticonTextEditSelector
{
    Q_OBJECT
public:
    explicit EmoticonMenuWidget(QWidget *parent = nullptr);
    ~EmoticonMenuWidget() override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

protected:
    [[nodiscard]] QSize sizeHint() const override;
};
