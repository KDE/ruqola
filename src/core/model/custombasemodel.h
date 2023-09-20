/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>
class LIBRUQOLACORE_EXPORT CustomBaseModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CustomBaseModel(QObject *parent = nullptr);
    ~CustomBaseModel() override;

    Q_REQUIRED_RESULT virtual int total() const = 0;

    void setHasFullList(bool state);
    Q_REQUIRED_RESULT bool hasFullList() const;

    Q_REQUIRED_RESULT bool loadMoreInProgress() const;
    void setLoadMoreInProgress(bool loadMoreInProgress);

    virtual void parseElements(const QJsonObject &obj) = 0;

    virtual void addMoreElements(const QJsonObject &obj) = 0;

    virtual QList<int> hideColumns() const = 0;

    virtual QList<int> excludeResizeToContentColumns() const;

    virtual void insertElement(const QJsonObject &obj);

    virtual void removeElement(const QString &identifier);

    virtual void updateElement(const QJsonObject &obj);

    void initialize();
Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    Q_DISABLE_COPY(CustomBaseModel)
    bool mLoadMoreInProgress = false;
    bool mHasFullList = false;
    int mTotal = -1;
};
