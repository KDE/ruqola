/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>
class QTimer;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ReconnectInfoWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit ReconnectInfoWidget(QWidget *parent = nullptr);
    ~ReconnectInfoWidget() override;

    [[nodiscard]] int reconnectSecondDelay() const;
    void setReconnectSecondDelay(int newReconnectDelay);

Q_SIGNALS:
    void tryReconnect();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateText();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateTimer();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLinkActivated(const QString &contents);
    QTimer *const mDelayTimer;
    int mCurrentDelay = -1;
    int mReconnectSecondDelay = -1;
};
