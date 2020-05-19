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

#include "rocketchaticonsmodel.h"

#include "managerdatapaths.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

RocketChatIconsModel::RocketChatIconsModel(QObject *parent)
    : QAbstractListModel(parent)
    , mNetworkAccessManager(new QNetworkAccessManager(this))
{
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished,
            this, &RocketChatIconsModel::slotIconDownloaded);
}

RocketChatIconsModel::~RocketChatIconsModel() = default;

int RocketChatIconsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return 1;
}

QHash<RocketChatIconsModel::IconRoles, QString> RocketChatIconsModel::sIconFileNames {
    {BellOffIcon, QStringLiteral("Bell-off.svg")},
    {DownloadIcon, QStringLiteral("Download.svg")},
    {EyeIcon, QStringLiteral("Eye.svg")},
    {FileGoogleDriveIcon, QStringLiteral("File-google-drive.svg")},
    {FileKeynoteIcon, QStringLiteral("File-keynote.svg")},
    {FilesAudioIcon, QStringLiteral("Files-audio.svg")},
    {FilesVideoIcon, QStringLiteral("Files-video.svg")},
    {FilesZipIcon, QStringLiteral("Files-zip.svg")},
    {MultilineIcon, QStringLiteral("Multiline.svg")},
    {SendActiveIcon, QStringLiteral("Send-active.svg")},
    {StarFilledIcon, QStringLiteral("Star-filled.svg")},
    {VideoOffIcon, QStringLiteral("Video-off.svg")},
    {VolumeDisableIcon, QStringLiteral("Volume-disable.svg")},
    {AddReactionIcon, QStringLiteral("add-reaction.svg")},
    {ArrowDownIcon, QStringLiteral("arrow-down.svg")},
    {AtIcon, QStringLiteral("at.svg")},
    {BackIcon, QStringLiteral("back.svg")},
    {BanIcon, QStringLiteral("ban.svg")},
    {BellIcon, QStringLiteral("bell.svg")},
    {BoldIcon, QStringLiteral("bold.svg")},
    {CalendarIcon, QStringLiteral("calendar.svg")},
    {ChatIcon, QStringLiteral("chat.svg")},
    {CheckIcon, QStringLiteral("check.svg")},
    {CheckmarkCircledIcon, QStringLiteral("checkmark-circled.svg")},
    {CircleCrossIcon, QStringLiteral("circle-cross.svg")},
    {CircleIcon, QStringLiteral("circle.svg")},
    {CircledArrowDownIcon, QStringLiteral("circled-arrow-down.svg")},
    {ClipIcon, QStringLiteral("clip.svg")},
    {ClipboardIcon, QStringLiteral("clipboard.svg")},
    {ClockIcon, QStringLiteral("clock.svg")},
    {CloudPlusIcon, QStringLiteral("cloud-plus.svg")},
    {CodeIcon, QStringLiteral("code.svg")},
    {CogIcon, QStringLiteral("cog.svg")},
    {ComputerIcon, QStringLiteral("computer.svg")},
    {CopyIcon, QStringLiteral("copy.svg")},
    {CrossIcon, QStringLiteral("cross.svg")},
    {CubeIcon, QStringLiteral("cube.svg")},
    {CustomizeIcon, QStringLiteral("customize.svg")},
    {DiscoverIcon, QStringLiteral("discover.svg")},
    {EditRoundedIcon, QStringLiteral("edit-rounded.svg")},
    {EditIcon, QStringLiteral("edit.svg")},
    {EmojiIcon, QStringLiteral("emoji.svg")},
    {EraserIcon, QStringLiteral("eraser.svg")},
    {EyeOffIcon, QStringLiteral("eye-off.svg")},
    {FacebookIcon, QStringLiteral("facebook.svg")},
    {FileDocumentIcon, QStringLiteral("file-document.svg")},
    {FileGenericIcon, QStringLiteral("file-generic.svg")},
    {FilePdfIcon, QStringLiteral("file-pdf.svg")},
    {FileSheetsIcon, QStringLiteral("file-sheets.svg")},
    {FlagIcon, QStringLiteral("flag.svg")},
    {FolderIcon, QStringLiteral("folder.svg")},
    {GithubIcon, QStringLiteral("github.svg")},
    {GitlabIcon, QStringLiteral("gitlab.svg")},
    {GoogleIcon, QStringLiteral("google.svg")},
    {HandPointerIcon, QStringLiteral("hand-pointer.svg")},
    {HashtagIcon, QStringLiteral("hashtag.svg")},
    {HelpIcon, QStringLiteral("help.svg")},
    {HubotIcon, QStringLiteral("hubot.svg")},
    {ImportIcon, QStringLiteral("import.svg")},
    {InfoCircledIcon, QStringLiteral("info-circled.svg")},
    {ItalicIcon, QStringLiteral("italic.svg")},
    {JumpIcon, QStringLiteral("jump.svg")},
    {KeyIcon, QStringLiteral("key.svg")},
    {KeyboardIcon, QStringLiteral("keyboard.svg")},
    {LanguageIcon, QStringLiteral("language.svg")},
    {LinkedinIcon, QStringLiteral("linkedin.svg")},
    {ListAltIcon, QStringLiteral("list-alt.svg")},
    {ListIcon, QStringLiteral("list.svg")},
    {LivechatIcon, QStringLiteral("livechat.svg")},
    {LoadingIcon, QStringLiteral("loading.svg")},
    {LockIcon, QStringLiteral("lock.svg")},
    {MagnifierIcon, QStringLiteral("magnifier.svg")},
    {MailIcon, QStringLiteral("mail.svg")},
    {MapPinIcon, QStringLiteral("map-pin.svg")},
    {MenuIcon, QStringLiteral("menu.svg")},
    {MessageIcon, QStringLiteral("message.svg")},
    {MicIcon, QStringLiteral("mic.svg")},
    {MobileIcon, QStringLiteral("mobile.svg")},
    {ModalWarningIcon, QStringLiteral("modal-warning.svg")},
    {MuteIcon, QStringLiteral("mute.svg")},
    {PauseIcon, QStringLiteral("pause.svg")},
    {PermalinkIcon, QStringLiteral("permalink.svg")},
    {PinIcon, QStringLiteral("pin.svg")},
    {PlaySolidIcon, QStringLiteral("play-solid.svg")},
    {PlayIcon, QStringLiteral("play.svg")},
    {PlusIcon, QStringLiteral("plus.svg")},
    {PodcastIcon, QStringLiteral("podcast.svg")},
    {PostIcon, QStringLiteral("post.svg")},
    {QueueIcon, QStringLiteral("queue.svg")},
    {QuoteIcon, QStringLiteral("quote.svg")},
    {ReloadIcon, QStringLiteral("reload.svg")},
    {ReplyIcon, QStringLiteral("reply.svg")},
    {SendIcon, QStringLiteral("send.svg")},
    {ShareIcon, QStringLiteral("share.svg")},
    {ShieldAltIcon, QStringLiteral("shield-alt.svg")},
    {ShieldCheckIcon, QStringLiteral("shield-check.svg")},
    {ShieldIcon, QStringLiteral("shield.svg")},
    {SignOutIcon, QStringLiteral("sign-out.svg")},
    {SortAmountDownIcon, QStringLiteral("sort-amount-down.svg")},
    {SortDownIcon, QStringLiteral("sort-down.svg")},
    {SortUpIcon, QStringLiteral("sort-up.svg")},
    {SortIcon, QStringLiteral("sort.svg")},
    {StarIcon, QStringLiteral("star.svg")},
    {StrikeIcon, QStringLiteral("strike.svg")},
    {TeamIcon, QStringLiteral("team.svg")},
    {ThListIcon, QStringLiteral("th-list.svg")},
    {ThreadIcon, QStringLiteral("thread.svg")},
    {TrashIcon, QStringLiteral("trash.svg")},
    {TwitterIcon, QStringLiteral("twitter.svg")},
    {UploadIcon, QStringLiteral("upload.svg")},
    {UserPlusIcon, QStringLiteral("user-plus.svg")},
    {UserRoundedIcon, QStringLiteral("user-rounded.svg")},
    {UserIcon, QStringLiteral("user.svg")},
    {VideoIcon, QStringLiteral("video.svg")},
    {VolumeMuteIcon, QStringLiteral("volume-mute.svg")},
    {VolumeIcon, QStringLiteral("volume.svg")},
    {WarningIcon, QStringLiteral("warning.svg")}
};

QUrl RocketChatIconsModel::localPathForIcon(IconRoles icon) const
{
    QUrl cachePath = QUrl::fromUserInput(ManagerDataPaths::self()->path(ManagerDataPaths::Cache, QStringLiteral("_client")));
    cachePath.setPath(cachePath.path() + QString(QStringLiteral("/ui-icons/%1").arg(sIconFileNames[icon])));
    return cachePath;
}

QUrl RocketChatIconsModel::urlForIcon(IconRoles icon) const
{
    QUrl baseUrl = QUrl::fromUserInput(QStringLiteral("https://raw.githubusercontent.com/"));
    baseUrl.setPath(
        QString(QStringLiteral("/RocketChat/Rocket.Chat/e275b949c56b3dd9a1dfd3fe6c958a338c7d6736/app/ui-master/public/icons/%1"))
        .arg(sIconFileNames[icon]));

    auto cachePath = localPathForIcon(icon);
    if (QFileInfo(cachePath.toLocalFile()).exists()) {
        return cachePath;
    }
    else {
        QNetworkRequest req(baseUrl);
        req.setAttribute(QNetworkRequest::Attribute::User, static_cast<int>(icon));
        mNetworkAccessManager->get(req);
        return {};
    }
}

QVariant RocketChatIconsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() != 0) return {};

    return urlForIcon(static_cast<IconRoles>(role));
}

void RocketChatIconsModel::slotIconDownloaded(QNetworkReply *reply)
{
    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status != 200) {
        qWarning() << Q_FUNC_INFO << "An error occurred while downloading an icon";
        return;
    }

    auto icon = reply->request().attribute(QNetworkRequest::Attribute::User).toInt();
    QUrl cachePath = localPathForIcon(static_cast<IconRoles>(icon));
    QByteArray data = reply->readAll();
    // Make it a valid SVG tiny
    data.replace(R"( viewBox="0 0 20 20")", R"( width="20" height="20")");
    QDir().mkpath(cachePath.adjusted(QUrl::RemoveFilename).toLocalFile());
    QFile file(cachePath.toLocalFile());
    if (file.open(QIODevice::ReadWrite)) {
        file.write(data);
        file.close();
        Q_EMIT dataChanged(index(0, 0), index(0, 0), {icon});
    }
}

QHash<int, QByteArray> RocketChatIconsModel::roleNames() const
{
    return {
        {BellOffIcon, QByteArrayLiteral("bellOffIcon")},
        {DownloadIcon, QByteArrayLiteral("downloadIcon")},
        {EyeIcon, QByteArrayLiteral("eyeIcon")},
        {FileGoogleDriveIcon, QByteArrayLiteral("fileGoogleDriveIcon")},
        {FileKeynoteIcon, QByteArrayLiteral("fileKeynoteIcon")},
        {FilesAudioIcon, QByteArrayLiteral("filesAudioIcon")},
        {FilesVideoIcon, QByteArrayLiteral("filesVideoIcon")},
        {FilesZipIcon, QByteArrayLiteral("filesZipIcon")},
        {MultilineIcon, QByteArrayLiteral("multilineIcon")},
        {SendActiveIcon, QByteArrayLiteral("sendActiveIcon")},
        {StarFilledIcon, QByteArrayLiteral("starFilledIcon")},
        {VideoOffIcon, QByteArrayLiteral("videoOffIcon")},
        {VolumeDisableIcon, QByteArrayLiteral("volumeDisableIcon")},
        {AddReactionIcon, QByteArrayLiteral("addReactionIcon")},
        {ArrowDownIcon, QByteArrayLiteral("arrowDownIcon")},
        {AtIcon, QByteArrayLiteral("atIcon")},
        {BackIcon, QByteArrayLiteral("backIcon")},
        {BanIcon, QByteArrayLiteral("banIcon")},
        {BellIcon, QByteArrayLiteral("bellIcon")},
        {BoldIcon, QByteArrayLiteral("boldIcon")},
        {CalendarIcon, QByteArrayLiteral("calendarIcon")},
        {ChatIcon, QByteArrayLiteral("chatIcon")},
        {CheckIcon, QByteArrayLiteral("checkIcon")},
        {CheckmarkCircledIcon, QByteArrayLiteral("checkmarkCircledIcon")},
        {CircleCrossIcon, QByteArrayLiteral("circleCrossIcon")},
        {CircleIcon, QByteArrayLiteral("circleIcon")},
        {CircledArrowDownIcon, QByteArrayLiteral("circledArrowDownIcon")},
        {ClipIcon, QByteArrayLiteral("clipIcon")},
        {ClipboardIcon, QByteArrayLiteral("clipboardIcon")},
        {ClockIcon, QByteArrayLiteral("clockIcon")},
        {CloudPlusIcon, QByteArrayLiteral("cloudPlusIcon")},
        {CodeIcon, QByteArrayLiteral("codeIcon")},
        {CogIcon, QByteArrayLiteral("cogIcon")},
        {ComputerIcon, QByteArrayLiteral("computerIcon")},
        {CopyIcon, QByteArrayLiteral("copyIcon")},
        {CrossIcon, QByteArrayLiteral("crossIcon")},
        {CubeIcon, QByteArrayLiteral("cubeIcon")},
        {CustomizeIcon, QByteArrayLiteral("customizeIcon")},
        {DiscoverIcon, QByteArrayLiteral("discoverIcon")},
        {EditRoundedIcon, QByteArrayLiteral("editRoundedIcon")},
        {EditIcon, QByteArrayLiteral("editIcon")},
        {EmojiIcon, QByteArrayLiteral("emojiIcon")},
        {EraserIcon, QByteArrayLiteral("eraserIcon")},
        {EyeOffIcon, QByteArrayLiteral("eyeOffIcon")},
        {FacebookIcon, QByteArrayLiteral("facebookIcon")},
        {FileDocumentIcon, QByteArrayLiteral("fileDocumentIcon")},
        {FileGenericIcon, QByteArrayLiteral("fileGenericIcon")},
        {FilePdfIcon, QByteArrayLiteral("filePdfIcon")},
        {FileSheetsIcon, QByteArrayLiteral("fileSheetsIcon")},
        {FlagIcon, QByteArrayLiteral("flagIcon")},
        {FolderIcon, QByteArrayLiteral("folderIcon")},
        {GithubIcon, QByteArrayLiteral("githubIcon")},
        {GitlabIcon, QByteArrayLiteral("gitlabIcon")},
        {GoogleIcon, QByteArrayLiteral("googleIcon")},
        {HandPointerIcon, QByteArrayLiteral("handPointerIcon")},
        {HashtagIcon, QByteArrayLiteral("hashtagIcon")},
        {HelpIcon, QByteArrayLiteral("helpIcon")},
        {HubotIcon, QByteArrayLiteral("hubotIcon")},
        {ImportIcon, QByteArrayLiteral("importIcon")},
        {InfoCircledIcon, QByteArrayLiteral("infoCircledIcon")},
        {ItalicIcon, QByteArrayLiteral("italicIcon")},
        {JumpIcon, QByteArrayLiteral("jumpIcon")},
        {KeyIcon, QByteArrayLiteral("keyIcon")},
        {KeyboardIcon, QByteArrayLiteral("keyboardIcon")},
        {LanguageIcon, QByteArrayLiteral("languageIcon")},
        {LinkedinIcon, QByteArrayLiteral("linkedinIcon")},
        {ListAltIcon, QByteArrayLiteral("listAltIcon")},
        {ListIcon, QByteArrayLiteral("listIcon")},
        {LivechatIcon, QByteArrayLiteral("livechatIcon")},
        {LoadingIcon, QByteArrayLiteral("loadingIcon")},
        {LockIcon, QByteArrayLiteral("lockIcon")},
        {MagnifierIcon, QByteArrayLiteral("magnifierIcon")},
        {MailIcon, QByteArrayLiteral("mailIcon")},
        {MapPinIcon, QByteArrayLiteral("mapPinIcon")},
        {MenuIcon, QByteArrayLiteral("menuIcon")},
        {MessageIcon, QByteArrayLiteral("messageIcon")},
        {MicIcon, QByteArrayLiteral("micIcon")},
        {MobileIcon, QByteArrayLiteral("mobileIcon")},
        {ModalWarningIcon, QByteArrayLiteral("modalWarningIcon")},
        {MuteIcon, QByteArrayLiteral("muteIcon")},
        {PauseIcon, QByteArrayLiteral("pauseIcon")},
        {PermalinkIcon, QByteArrayLiteral("permalinkIcon")},
        {PinIcon, QByteArrayLiteral("pinIcon")},
        {PlaySolidIcon, QByteArrayLiteral("playSolidIcon")},
        {PlayIcon, QByteArrayLiteral("playIcon")},
        {PlusIcon, QByteArrayLiteral("plusIcon")},
        {PodcastIcon, QByteArrayLiteral("podcastIcon")},
        {PostIcon, QByteArrayLiteral("postIcon")},
        {QueueIcon, QByteArrayLiteral("queueIcon")},
        {QuoteIcon, QByteArrayLiteral("quoteIcon")},
        {ReloadIcon, QByteArrayLiteral("reloadIcon")},
        {ReplyIcon, QByteArrayLiteral("replyIcon")},
        {SendIcon, QByteArrayLiteral("sendIcon")},
        {ShareIcon, QByteArrayLiteral("shareIcon")},
        {ShieldAltIcon, QByteArrayLiteral("shieldAltIcon")},
        {ShieldCheckIcon, QByteArrayLiteral("shieldCheckIcon")},
        {ShieldIcon, QByteArrayLiteral("shieldIcon")},
        {SignOutIcon, QByteArrayLiteral("signOutIcon")},
        {SortAmountDownIcon, QByteArrayLiteral("sortAmountDownIcon")},
        {SortDownIcon, QByteArrayLiteral("sortDownIcon")},
        {SortUpIcon, QByteArrayLiteral("sortUpIcon")},
        {SortIcon, QByteArrayLiteral("sortIcon")},
        {StarIcon, QByteArrayLiteral("starIcon")},
        {StrikeIcon, QByteArrayLiteral("strikeIcon")},
        {TeamIcon, QByteArrayLiteral("teamIcon")},
        {ThListIcon, QByteArrayLiteral("thListIcon")},
        {ThreadIcon, QByteArrayLiteral("threadIcon")},
        {TrashIcon, QByteArrayLiteral("trashIcon")},
        {TwitterIcon, QByteArrayLiteral("twitterIcon")},
        {UploadIcon, QByteArrayLiteral("uploadIcon")},
        {UserPlusIcon, QByteArrayLiteral("userPlusIcon")},
        {UserRoundedIcon, QByteArrayLiteral("userRoundedIcon")},
        {UserIcon, QByteArrayLiteral("userIcon")},
        {VideoIcon, QByteArrayLiteral("videoIcon")},
        {VolumeMuteIcon, QByteArrayLiteral("volumeMuteIcon")},
        {VolumeIcon, QByteArrayLiteral("volumeIcon")},
        {WarningIcon, QByteArrayLiteral("warningIcon")}
    };
}
