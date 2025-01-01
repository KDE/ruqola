/*
  SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QByteArray text();
    [[nodiscard]] QMenu *menu() const;

    void setSelectedText(const QString &str);

Q_SIGNALS:
    void errorMessage(const QString &message);
    void successMessage(const QString &message);

private:
    void slotInitializeShareMenu();
    void slotShareActionFinished(const QJsonObject &output, int error, const QString &message);
    QString mSelectedText;
    Purpose::Menu *const mShareMenu;
    QTemporaryFile *mTemporaryShareFile = nullptr;
};
