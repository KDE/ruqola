/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class RocketChatAccount;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eDisableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit E2eDisableWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~E2eDisableWidget() override;

    // Room the keys of which will be reset. The reset button stays disabled until it is known.
    void setRoomId(const QByteArray &roomId);

Q_SIGNALS:
    // The room keys were reset: encryption stays enabled, so the dialog has to be closed without
    // disabling it.
    void resetEncryptionKeysDone();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotResetEncryptionKeys();
    QByteArray mRoomId;
    QPushButton *const mResetKeysButton;
    RocketChatAccount *const mRocketChatAccount;
};
