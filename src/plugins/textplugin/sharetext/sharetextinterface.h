/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintextinterface.h"
class PurposeMenuWidget;
class ShareTextInterface : public PluginTextInterface
{
    Q_OBJECT
public:
    explicit ShareTextInterface(QObject *parent = nullptr);
    ~ShareTextInterface() override;

    void addAction(QMenu *menu) override;

    void setSelectedText(const QString &str) override;

Q_SIGNALS:
    void errorMessage(const QString &message);
    void successMessage(const QString &message);

private:
    PurposeMenuWidget *const mPurposeMenuWidget;
};
