/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QPointer>
#include <QToolButton>
class RocketChatAccount;
class ActionButtonsGenerator;
class QMenu;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageLineExtraToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit MessageLineExtraToolButton(QWidget *parent = nullptr);
    ~MessageLineExtraToolButton() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account);

    void setRoomId(const QByteArray &roomId);
Q_SIGNALS:
    void uiInteractionRequested(const QJsonObject &obj);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotActionButtonChanged();
    QByteArray mRoomId;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    ActionButtonsGenerator *const mActionButtonsGenerator;
    QMenu *const mMenu;
};
