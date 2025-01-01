/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "room/plugins/plugintextinterface.h"
namespace KIO
{
class KUriFilterSearchProviderActions;
}
class WebShortcutTextInterface : public PluginTextInterface
{
    Q_OBJECT
public:
    explicit WebShortcutTextInterface(QObject *parent = nullptr);
    ~WebShortcutTextInterface() override;

    void addAction(QMenu *menu) override;

    void setSelectedText(const QString &str) override;

private:
    KIO::KUriFilterSearchProviderActions *const mWebShortcutMenuManager;
};
