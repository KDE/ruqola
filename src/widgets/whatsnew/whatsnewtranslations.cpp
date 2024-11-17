/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewtranslations.h"

WhatsNewTranslations::WhatsNewTranslations() = default;

WhatsNewTranslations::~WhatsNewTranslations() = default;

// Use by newFeaturesMD5
QList<KLazyLocalizedString> WhatsNewTranslations::lastNewFeatures() const
{
    const QList<KLazyLocalizedString> info{
        kli18n("Allow to clean up room history when room was not opened from long time."),
        kli18n("Add restore button in administrator server settings dialog."),
        kli18n("Improve changing password (show validation info)."),
        kli18n("Improve register new account (Add reason support)."),
        kli18n("Implement mute/unmute user."),
    };
    return info;
}

QList<WhatsNewInfo> WhatsNewTranslations::createWhatsNewInfo() const
{
    QList<WhatsNewInfo> listInfo;
    {
        WhatsNewInfo info;
        info.setChanges({i18n("Store Message in Local Database (experimental)")});
        info.setNewFeatures({
            i18n("Download Server Icon from Server"),
            i18n("Show Server Error Info in Specific DialogBox"),
            i18n("Allow to Copy Image in Clipboard"),
            i18n("Improve debug support (Allow to show current account permissions)"),
            i18n("Add support for quoted text (text which starts by \'>\')"),
            i18n("Show leader/moderator/owner in channel info."),
            i18n("Video/Sound Message support (kf6 only)."),
            i18n("Import/Export Accounts."),
        });
        info.setBugFixings({i18n("Fix avatar support"),
                            i18n("Fix emoji support (use new ktextaddons/textemoticons)"),
                            i18n("Fix animated gif in reactions"),
                            i18n("Fix kf6 support")});
        info.setVersion(QStringLiteral("2.0"));
        listInfo.append(std::move(info));
    }
    {
        WhatsNewInfo info;
        info.setNewFeatures({i18n("Add Moderation Console (Administration, RC server 6.2)"), i18n("Implement message style"), i18n("Implement Url Preview")});
        info.setBugFixings({
            i18n("Fix markdown support (header + check mark)"),
            i18n("Fix mem leak"),
            i18n("Cache message size (optimization)"),
            i18n("Improve select room or user in message line edit (show icon, inform if user is in channel)"),
            i18n("Fix show commands in message line edit (Hide action when room doesn't support it)."),
            i18n("Fix download sound/video file when it's not in cache."),
            i18n("Fix list of authentication method."),
            i18n("Fix show avatar url."),
        });
        info.setVersion(QStringLiteral("2.1"));
        listInfo.append(std::move(info));
    }
    {
        WhatsNewInfo info;
        info.setNewFeatures({
            i18n("Allow to increase/decrease font (CTRL++/CTRL+-)"),
            i18n("Add channel list style (Condensed/Medium/Extended)"),
            i18n("Add forward message"),
            i18n("Improve mentions support."),
            i18n("Add support for deep linking <a href=\"https://developer.rocket.chat/getting-started/deep-linking\">Deep Linking</a>."),
            i18n("Implement block actions."),
            i18n("Implement personal token authentication. <a href=\"https://bugs.kde.org/show_bug.cgi?id=481400\">Bug 481400</a>"),
            i18n("Add Plasma Activities Support"),
            i18n("Add Report User Support"),
            i18n("Implement Channel Sound Notification."),
            i18n("Implement New Room Sound Notification."),
            i18n("Implement Sorted/Unsorted markdown list."),
        });
        info.setBugFixings({
            i18n("Fix dark mode support."),
            i18n("Fix jitsi support."),
            i18n("Fix translate message in direct channel."),
            i18n("Don't show @here/@all as user."),
            i18n("Reduce memory footprint."),
            i18n("Use RESTAPI for logging."),
            i18n("Allow to send multi files."),
            i18n("Fix preview url."),
        });
        info.setVersion(QStringLiteral("2.2"));
        listInfo.append(std::move(info));
    }
    {
        WhatsNewInfo info;
        info.setNewFeatures({
            i18n("Implement Rocket.Chat Marketplace."),
            i18n("Allow to clean room history."),
            i18n("Allow to check new version."),
            i18n("Implement moderation (administrator mode)."),
            i18n("Add welcome page."),
            i18n("Implement pending users info (administrator mode)."),
            i18n("Use cmark-rc (https://github.com/dfaure/cmark-rc) for markdown support."),
            i18n("Delete oldest files from some cache directories (file-upload and media) so it doesn't grow forever."),
        });
        info.setVersion(QStringLiteral("2.3"));
        info.setBugFixings({
            i18n("Clean market application model after 30 minutes (reduce memory footprint)."),
            i18n("Fix show discussion name in completion."),
            i18n("Fix duplicated messages in search message dialog."),
            i18n("Add delegate in search rooms in team dialog."),
        });

        listInfo.append(std::move(info));
    }
    {
        WhatsNewInfo info;
        QStringList lst;
        for (const KLazyLocalizedString &l : lastNewFeatures()) {
            lst += l.toString();
        }
        info.setNewFeatures(lst);
        info.setVersion(QStringLiteral("2.4"));
        info.setBugFixings({i18n("Fix editing message."),
                            i18n("Show permissions in Rocket.Chat Marketplace."),
                            i18n("Fix reconnect server."),
                            i18n("Fix single application on Windows/MacOs")});

        listInfo.append(std::move(info));
    }
    return listInfo;
}
