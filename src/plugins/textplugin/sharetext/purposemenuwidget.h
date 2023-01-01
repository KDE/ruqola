/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
namespace Purpose
{
class Menu;
}
class QMenu;
class QTemporaryFile;
class PurposeMenuWidget : public QObject
{
    Q_OBJECT
public:
    explicit PurposeMenuWidget(QObject *parent = nullptr);
    ~PurposeMenuWidget() override;

    Q_REQUIRED_RESULT QByteArray text();
    Q_REQUIRED_RESULT QMenu *menu() const;

    void setSelectedText(const QString &str);

private:
    void slotInitializeShareMenu();
    void slotShareActionFinished(const QJsonObject &output, int error, const QString &message);
    QString mSelectedText;
    Purpose::Menu *mShareMenu = nullptr;
    QTemporaryFile *mTemporaryShareFile = nullptr;
};
