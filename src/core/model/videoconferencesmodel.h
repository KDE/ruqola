/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "videoconference/videoconferenceinfos.h"
#include <QAbstractListModel>

class RocketChatAccount;
class LIBRUQOLACORE_EXPORT VideoConferencesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum VideoConferencesRoles {
        StartedVideo = Qt::UserRole + 1,
    };
    Q_ENUM(VideoConferencesRoles)

    explicit VideoConferencesModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~VideoConferencesModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    void parseVideoConferences(const QJsonObject &fileAttachmentsObj, const QString &roomId);
    void addMoreVideoConferences(const QJsonObject &fileAttachmentsObj);
    void initialize();
    [[nodiscard]] int total() const;

    [[nodiscard]] bool hasFullList() const;
    void setHasFullList(bool state);

    [[nodiscard]] bool loadMoreVideoConferencesInProgress() const;
    void setLoadMoreVideoConferencesInProgress(bool loadMoreFilesInProgress);

    void clear();

    void setVideoConferenceInfos(const QList<VideoConferenceInfo> &files);
Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    VideoConferenceInfos mVideoConferenceInfos;
    QString mRoomId;
    bool mHasFullList = false;
    bool mLoadMoreFilesInProgress = false;
    RocketChatAccount *const mRochetChantAccount;
};
