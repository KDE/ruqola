/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WebRtcSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit WebRtcSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~WebRtcSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mEnabled;
    QCheckBox *const mEnablePublicChannels;
    QCheckBox *const mEnablePrivateChannels;
    QCheckBox *const mEnableDirectMessages;
    QLineEdit *const mServer;
};
