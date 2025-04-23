/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/applicationssettingssettingsinfo.h"
#include <QWidget>
class RocketChatAccount;
class QCheckBox;
class ApplictionSettingsCustomWidgetsBase : public QWidget
{
    Q_OBJECT
public:
    explicit ApplictionSettingsCustomWidgetsBase(const QByteArray &appId, RocketChatAccount *account, QWidget *parent = nullptr);
    ~ApplictionSettingsCustomWidgetsBase() override;

    [[nodiscard]] virtual QString value() const = 0;

Q_SIGNALS:
    void dataChanged(bool status);
    void resetValue();

protected:
    [[nodiscard]] QString getTranslatedIdentifier(const QString &lang, const QString &id) const;
    RocketChatAccount *const mRocketChatAccount;
    const QByteArray mAppId;
};

class ApplictionSettingsCustomWidgetsCheckBox : public ApplictionSettingsCustomWidgetsBase
{
    Q_OBJECT
public:
    explicit ApplictionSettingsCustomWidgetsCheckBox(const QByteArray &appId,
                                                     RocketChatAccount *account,
                                                     const ApplicationsSettingsSettingsInfo &info,
                                                     QWidget *parent = nullptr);
    ~ApplictionSettingsCustomWidgetsCheckBox() override;

    [[nodiscard]] QString value() const override;

private:
    QCheckBox *const mCheckBox;
};
