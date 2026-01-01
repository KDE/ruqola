/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GrabScreenPluginJob : public QObject
{
    Q_OBJECT
public:
    explicit GrabScreenPluginJob(QObject *parent = nullptr);
    ~GrabScreenPluginJob() override;

    void start();

    [[nodiscard]] bool canStart() const;

    [[nodiscard]] QString filePath() const;
    void setFilePath(const QString &newFilePath);

Q_SIGNALS:
    void captureDone();
    void captureCanceled();

private:
    QString mFilePath;
};
