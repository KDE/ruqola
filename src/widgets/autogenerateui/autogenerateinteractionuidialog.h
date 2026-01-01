/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class QVBoxLayout;
class RocketChatAccount;
class AutoGenerateInteractionUi;
class AutoGenerateInteractionUiDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AutoGenerateInteractionUiDialog() override;

    [[nodiscard]] bool parse(const QJsonObject &r);

private:
    void slotActionChanged(const QJsonObject &replyObject);
    QVBoxLayout *const mainLayout;
    RocketChatAccount *const mRocketChatAccount;
    QWidget *mMainWidget = nullptr;
    AutoGenerateInteractionUi *const mAutoGenerateInteractionUi;
};
