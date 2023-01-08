/*
  SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
class QNetworkConfigurationManager;
#endif
namespace TextTranslator
{
/**
 * @brief The NetworkManager class
 * @author Laurent Montel <montel@kde.org>
 */
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager() override;
    static NetworkManager *self();

    Q_REQUIRED_RESULT bool isOnline() const;

Q_SIGNALS:
    void networkStatusChanged(bool isOnline);

private:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QNetworkConfigurationManager *const mNetworkConfigureManager;
#endif
};
}
