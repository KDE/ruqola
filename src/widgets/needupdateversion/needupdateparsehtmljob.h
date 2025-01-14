/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QObject>
#include <QUrl>
namespace KIO
{
class Job;
}
class LIBRUQOLAWIDGETS_TESTS_EXPORT NeedUpdateParseHtmlJob : public QObject
{
    Q_OBJECT
public:
    explicit NeedUpdateParseHtmlJob(QObject *parent = nullptr);
    ~NeedUpdateParseHtmlJob() override;

    [[nodiscard]] bool canStart() const;

    [[nodiscard]] QUrl url() const;
    void setUrl(const QUrl &newUrl);

    void start();

Q_SIGNALS:
    void downLoadDone(const QString &data);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotHttpDataFile(KIO::Job *job, const QByteArray &data);
    QUrl mUrl;
    QString mData;
};
