/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QObject>
#include <QUrl>

class LIBRUQOLAWIDGETS_TESTS_EXPORT NeedUpdateCheckExistingNewVersionJob : public QObject
{
    Q_OBJECT
public:
    explicit NeedUpdateCheckExistingNewVersionJob(QObject *parent = nullptr);
    ~NeedUpdateCheckExistingNewVersionJob() override;

    void start();

    [[nodiscard]] QUrl url() const;
    void setUrl(const QUrl &newUrl);

    [[nodiscard]] bool canStart() const;

    [[nodiscard]] QString compileDate() const;
    void setCompileDate(const QString &newCompileDate);

Q_SIGNALS:
    void foundNewVersion(bool state);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDownloadDone(const QString &str);
    QUrl mUrl;
    QString mCompileDate;
};
