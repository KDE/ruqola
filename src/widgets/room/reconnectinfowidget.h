/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT int reconnectSecondDelay() const;
    void setReconnectSecondDelay(int newReconnectDelay);

Q_SIGNALS:
    void tryReconnect();

private:
    void updateText();
    void slotUpdateTimer();
    void slotLinkActivated(const QString &contents);
    QTimer *const mDelayTimer;
    int mCurrentDelay = -1;
    int mReconnectSecondDelay = -1;
};
