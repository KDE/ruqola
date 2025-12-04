/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_export.h"
#include <QObject>
class QMenu;
class LIBRUQOLAWIDGETS_EXPORT PluginToolInterface : public QObject
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_EXPORT PluginToolInfo {
        QByteArray roomId;
        QString accountName;
        QByteArray tmid;
        QByteArray msgId;
    };

    explicit PluginToolInterface(QWidget *parentWidget, QObject *parent = nullptr);
    ~PluginToolInterface() override;

    virtual void activateTool();

    [[nodiscard]] PluginToolInfo info() const;
    void setInfo(const PluginToolInfo &newInfo);

    [[nodiscard]] virtual QMenu *menu() const;
Q_SIGNALS:
    void executed();
    void activateRequested();

protected:
    PluginToolInfo mInfo;
    QWidget *mParentWidget = nullptr;
};
class QDebug;
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const PluginToolInterface::PluginToolInfo &t);
