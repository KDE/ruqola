/*
   Copyright (c) 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include <QAbstractItemModel>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class RocketChatIconsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum IconRoles {
        BellOffIcon = Qt::UserRole + 1,
        DownloadIcon,
        EyeIcon,
        FileGoogleDriveIcon,
        FileKeynoteIcon,
        FilesAudioIcon,
        FilesVideoIcon,
        FilesZipIcon,
        MultilineIcon,
        SendActiveIcon,
        StarFilledIcon,
        VideoOffIcon,
        VolumeDisableIcon,
        AddReactionIcon,
        ArrowDownIcon,
        AtIcon,
        BackIcon,
        BanIcon,
        BellIcon,
        BoldIcon,
        CalendarIcon,
        ChatIcon,
        CheckIcon,
        CheckmarkCircledIcon,
        CircleCrossIcon,
        CircleIcon,
        CircledArrowDownIcon,
        ClipIcon,
        ClipboardIcon,
        ClockIcon,
        CloudPlusIcon,
        CodeIcon,
        CogIcon,
        ComputerIcon,
        CopyIcon,
        CrossIcon,
        CubeIcon,
        CustomizeIcon,
        DiscoverIcon,
        EditRoundedIcon,
        EditIcon,
        EmojiIcon,
        EraserIcon,
        EyeOffIcon,
        FacebookIcon,
        FileDocumentIcon,
        FileGenericIcon,
        FilePdfIcon,
        FileSheetsIcon,
        FlagIcon,
        FolderIcon,
        GithubIcon,
        GitlabIcon,
        GoogleIcon,
        HandPointerIcon,
        HashtagIcon,
        HelpIcon,
        HubotIcon,
        ImportIcon,
        InfoCircledIcon,
        ItalicIcon,
        JumpIcon,
        KeyIcon,
        KeyboardIcon,
        LanguageIcon,
        LinkedinIcon,
        ListAltIcon,
        ListIcon,
        LivechatIcon,
        LoadingIcon,
        LockIcon,
        MagnifierIcon,
        MailIcon,
        MapPinIcon,
        MenuIcon,
        MessageIcon,
        MicIcon,
        MobileIcon,
        ModalWarningIcon,
        MuteIcon,
        PauseIcon,
        PermalinkIcon,
        PinIcon,
        PlaySolidIcon,
        PlayIcon,
        PlusIcon,
        PodcastIcon,
        PostIcon,
        QueueIcon,
        QuoteIcon,
        ReloadIcon,
        ReplyIcon,
        SendIcon,
        ShareIcon,
        ShieldAltIcon,
        ShieldCheckIcon,
        ShieldIcon,
        SignOutIcon,
        SortAmountDownIcon,
        SortDownIcon,
        SortUpIcon,
        SortIcon,
        StarIcon,
        StrikeIcon,
        TeamIcon,
        ThListIcon,
        ThreadIcon,
        TrashIcon,
        TwitterIcon,
        UploadIcon,
        UserPlusIcon,
        UserRoundedIcon,
        UserIcon,
        VideoIcon,
        VolumeMuteIcon,
        VolumeIcon,
        WarningIcon,

        IconRolesCount,
    };
    Q_ENUM(IconRoles)

    explicit RocketChatIconsModel(QObject *parent = nullptr);
    ~RocketChatIconsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

private:
    QUrl urlForIcon(IconRoles icon) const;

    static QHash<IconRoles, QString> sIconFileNames;
    void slotIconDownloaded(QNetworkReply *reply);
    QUrl localPathForIcon(IconRoles icon) const;
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
};

Q_DECLARE_METATYPE(RocketChatIconsModel*)
