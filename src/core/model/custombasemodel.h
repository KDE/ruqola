/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] virtual int total() const = 0;

    void setHasFullList(bool state);
    [[nodiscard]] bool hasFullList() const;

    [[nodiscard]] bool loadMoreInProgress() const;
    void setLoadMoreInProgress(bool loadMoreInProgress);

    virtual void parseElements(const QJsonObject &obj) = 0;

    virtual void addMoreElements(const QJsonObject &obj) = 0;

    virtual QList<int> hideColumns() const = 0;

    virtual QList<int> excludeResizeToContentColumns() const;

    virtual void insertElement(const QJsonObject &obj);

    virtual void removeElement(const QByteArray &identifier);

    virtual void updateElement(const QJsonObject &obj);

    void initialize();

protected:
    virtual void checkFullList() = 0;

Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    bool mLoadMoreInProgress = false;
    bool mHasFullList = false;
};
