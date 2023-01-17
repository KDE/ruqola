/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QObject>
class LIBRUQOLACORE_EXPORT TranslateTextJob : public QObject
{
    Q_OBJECT
public:
    struct TranslateInfo {
        QString from;
        QString to;
        QString inputText;
        Q_REQUIRED_RESULT bool isValid() const;
    };
    explicit TranslateTextJob(QObject *parent = nullptr);
    ~TranslateTextJob() override;

    void translate();

    Q_REQUIRED_RESULT const TranslateInfo &info() const;
    void setInfo(const TranslateInfo &newInfo);

Q_SIGNALS:
    void translateDone(const QString &translatedText);
    void translateFailed(bool result, const QString &errorMessage = QString());

private:
    void initializeTranslateEngine();
    TranslateInfo mInfo;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TranslateTextJob::TranslateInfo &t);
