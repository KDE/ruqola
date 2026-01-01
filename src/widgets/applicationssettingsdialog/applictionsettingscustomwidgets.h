/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/applicationssettingssettingsinfo.h"
#include <QWidget>
class RocketChatAccount;
class QCheckBox;
class QSpinBox;
class QComboBox;
class QPlainTextEdit;
class QLineEdit;
class ApplictionSettingsCustomWidgetsBase : public QWidget
{
    Q_OBJECT
public:
    explicit ApplictionSettingsCustomWidgetsBase(const QByteArray &appId,
                                                 RocketChatAccount *account,
                                                 const ApplicationsSettingsSettingsInfo &info,
                                                 QWidget *parent = nullptr);
    ~ApplictionSettingsCustomWidgetsBase() override;

    [[nodiscard]] virtual QString value() const = 0;

    [[nodiscard]] ApplicationsSettingsSettingsInfo info() const;

Q_SIGNALS:
    void dataChanged(bool status);
    void resetValue();

protected:
    [[nodiscard]] QString getTranslatedIdentifier(const QString &lang, const QString &id) const;
    RocketChatAccount *const mRocketChatAccount;
    const QByteArray mAppId;
    const ApplicationsSettingsSettingsInfo mInfo;
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

class ApplictionSettingsCustomWidgetsSpinBox : public ApplictionSettingsCustomWidgetsBase
{
    Q_OBJECT
public:
    explicit ApplictionSettingsCustomWidgetsSpinBox(const QByteArray &appId,
                                                    RocketChatAccount *account,
                                                    const ApplicationsSettingsSettingsInfo &info,
                                                    QWidget *parent = nullptr);
    ~ApplictionSettingsCustomWidgetsSpinBox() override;

    [[nodiscard]] QString value() const override;

private:
    QSpinBox *const mSpinBox;
};

class ApplictionSettingsCustomWidgetsComboBox : public ApplictionSettingsCustomWidgetsBase
{
    Q_OBJECT
public:
    explicit ApplictionSettingsCustomWidgetsComboBox(const QByteArray &appId,
                                                     RocketChatAccount *account,
                                                     const ApplicationsSettingsSettingsInfo &info,
                                                     QWidget *parent = nullptr);
    ~ApplictionSettingsCustomWidgetsComboBox() override;

    [[nodiscard]] QString value() const override;

private:
    QComboBox *const mComboBox;
};

class ApplictionSettingsCustomWidgetsString : public ApplictionSettingsCustomWidgetsBase
{
    Q_OBJECT
public:
    explicit ApplictionSettingsCustomWidgetsString(const QByteArray &appId,
                                                   RocketChatAccount *account,
                                                   const ApplicationsSettingsSettingsInfo &info,
                                                   QWidget *parent = nullptr);
    ~ApplictionSettingsCustomWidgetsString() override;

    [[nodiscard]] QString value() const override;

private:
    QPlainTextEdit *mPlainTextEdit = nullptr;
    QLineEdit *mLineEdit = nullptr;
};
