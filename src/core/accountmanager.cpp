/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountmanager.h"
#include "job/validateinviteserverjob.h"
#include "localdatabase/localdatabaseutils.h"
#include "managerdatapaths.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "notificationhistorymanager.h"
#include "notifications/notifierjob.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola_debug.h"

#include "ruqola.h"
#include "ruqola_sound_debug.h"
#include "ruqolaglobalconfig.h"
#include "utils.h"

#if HAVE_ACTIVITY_SUPPORT
#include "activities/activitiesmanager.h"
#endif
#include <KLocalizedString>
#include <KNotification>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <TextEmoticonsCore/EmojiModelManager>
#include <TextEmoticonsCore/UnicodeEmoticonManager>

using namespace Qt::Literals::StringLiterals;
namespace
{
constexpr int currentDataBaseVersion = 1;
}

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
    , mRocketChatAccountModel(new RocketChatAccountModel(this))
    , mRocketChatAccountProxyModel(new RocketChatAccountFilterProxyModel(this))
#if HAVE_ACTIVITY_SUPPORT
    , mActivitiesManager(new ActivitiesManager(this))
#endif
{
#if HAVE_ACTIVITY_SUPPORT
    mRocketChatAccountProxyModel->setActivitiesManager(mActivitiesManager);
    // TODO disable/enable account
#endif
    mRocketChatAccountProxyModel->setSourceModel(mRocketChatAccountModel);
#if HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
    TextEmoticonsCore::UnicodeEmoticonManager::self(u":/emoji_ruqola.json"_s);
#else
    loadExcludeEmoticons();
#endif
    loadAccount();
    connect(this, &AccountManager::activitiesChanged, mRocketChatAccountProxyModel, &RocketChatAccountFilterProxyModel::slotActivitiesChanged);
}

AccountManager::~AccountManager() = default;

void AccountManager::loadExcludeEmoticons()
{
#if !HAVE_TEXTEMOTICONSCORE_UNICODEMANAGER_CUSTOM_FILENAME
    const QStringList lst = QStringList({u":face_holding_back_tears:"_s,
                                         u":smiling_face_with_tear:"_s,
                                         u":disguised_face:"_s,
                                         u":face_in_clouds:"_s,
                                         u":face_with_peeking_eye:"_s,
                                         u":face_with_open_eyes_and_hand_over_mouth:"_s,
                                         u":saluting_face:"_s,
                                         u":melting_face:"_s,
                                         u":dotted_line_face:"_s,
                                         u":face_with_diagonal_mouth:"_s,
                                         u":face_exhaling:"_s,
                                         u":face_with_spiral_eyes:"_s,
                                         u":heart_hands:"_s,
                                         u":heart_hands_tone1:"_s,
                                         u":heart_hands_tone2:"_s,
                                         u":heart_hands_tone3:"_s,
                                         u":heart_hands_tone4:"_s,
                                         u":heart_hands_tone5:"_s,
                                         u":handshake_tone1:"_s,
                                         u":handshake_tone1_tone2:"_s,
                                         u":handshake_tone1_tone3:"_s,
                                         u":handshake_tone1_tone4:"_s,
                                         u":handshake_tone1_tone5:"_s,
                                         u":handshake_tone2_tone1:"_s,
                                         u":handshake_tone2:"_s,
                                         u":handshake_tone2_tone3:"_s,
                                         u":handshake_tone2_tone4:"_s,
                                         u":handshake_tone2_tone5:"_s,
                                         u":handshake_tone3_tone1:"_s,
                                         u":handshake_tone3_tone2:"_s,
                                         u":handshake_tone3:"_s,
                                         u":handshake_tone3_tone4:"_s,
                                         u":handshake_tone3_tone5:"_s,
                                         u":handshake_tone4_tone1:"_s,
                                         u":handshake_tone4_tone2:"_s,
                                         u":handshake_tone4_tone3:"_s,
                                         u":handshake_tone4:"_s,
                                         u":handshake_tone4_tone5:"_s,
                                         u":handshake_tone5_tone1:"_s,
                                         u":handshake_tone5_tone2:"_s,
                                         u":handshake_tone5_tone3:"_s,
                                         u":handshake_tone5_tone4:"_s,
                                         u":handshake_tone5:"_s,
                                         u":hand_with_index_finger_and_thumb_crossed:"_s,
                                         u":hand_with_index_finger_and_thumb_crossed_tone1:"_s,
                                         u":hand_with_index_finger_and_thumb_crossed_tone2:"_s,
                                         u":hand_with_index_finger_and_thumb_crossed_tone3:"_s,
                                         u":hand_with_index_finger_and_thumb_crossed_tone4:"_s,
                                         u":hand_with_index_finger_and_thumb_crossed_tone5:"_s,
                                         u":pinched_fingers:"_s,
                                         u":pinched_fingers_tone2:"_s,
                                         u":pinched_fingers_tone1:"_s,
                                         u":pinched_fingers_tone3:"_s,
                                         u":pinched_fingers_tone4:"_s,
                                         u":pinched_fingers_tone5:"_s,
                                         u":palm_down_hand:"_s,
                                         u":palm_down_hand_tone1:"_s,
                                         u":palm_down_hand_tone2:"_s,
                                         u":palm_down_hand_tone3:"_s,
                                         u":palm_down_hand_tone4:"_s,
                                         u":palm_down_hand_tone5:"_s,
                                         u":palm_up_hand:"_s,
                                         u":palm_up_hand_tone1:"_s,
                                         u":palm_up_hand_tone2:"_s,
                                         u":palm_up_hand_tone3:"_s,
                                         u":palm_up_hand_tone4:"_s,
                                         u":palm_up_hand_tone5:"_s,
                                         u":leftwards_hand:"_s,
                                         u":leftwards_hand_tone1:"_s,
                                         u":leftwards_hand_tone2:"_s,
                                         u":leftwards_hand_tone3:"_s,
                                         u":leftwards_hand_tone4:"_s,
                                         u":leftwards_hand_tone5:"_s,
                                         u":rightwards_hand:"_s,
                                         u":rightwards_hand_tone1:"_s,
                                         u":rightwards_hand_tone2:"_s,
                                         u":rightwards_hand_tone3:"_s,
                                         u":rightwards_hand_tone4:"_s,
                                         u":rightwards_hand_tone5:"_s,
                                         u":index_pointing_at_the_viewer:"_s,
                                         u":index_pointing_at_the_viewer_tone1:"_s,
                                         u":index_pointing_at_the_viewer_tone2:"_s,
                                         u":index_pointing_at_the_viewer_tone3:"_s,
                                         u":index_pointing_at_the_viewer_tone4:"_s,
                                         u":index_pointing_at_the_viewer_tone5:"_s,
                                         u":biting_lip:"_s,
                                         u":anatomical_heart:"_s,
                                         u":lungs:"_s,
                                         u":people_hugging:"_s,
                                         u":person_curly_hair:"_s,
                                         u":person_tone1_curly_hair:"_s,
                                         u":person_tone2_curly_hair:"_s,
                                         u":person_tone3_curly_hair:"_s,
                                         u":person_tone4_curly_hair:"_s,
                                         u":person_tone5_curly_hair:"_s,
                                         u":person_red_hair:"_s,
                                         u":person_tone1_red_hair:"_s,
                                         u":person_tone2_red_hair:"_s,
                                         u":person_tone3_red_hair:"_s,
                                         u":person_tone4_red_hair:"_s,
                                         u":person_tone5_red_hair:"_s,
                                         u":person_white_hair:"_s,
                                         u":person_tone1_white_hair:"_s,
                                         u":person_tone2_white_hair:"_s,
                                         u":person_tone3_white_hair:"_s,
                                         u":person_tone4_white_hair:"_s,
                                         u":person_tone5_white_hair:"_s,
                                         u":person_bald:"_s,
                                         u":person_tone1_bald:"_s,
                                         u":person_tone2_bald:"_s,
                                         u":person_tone3_bald:"_s,
                                         u":person_tone4_bald:"_s,
                                         u":person_tone5_bald:"_s,
                                         u":woman_beard:"_s,
                                         u":woman_tone1_beard:"_s,
                                         u":woman_tone2_beard:"_s,
                                         u":woman_tone3_beard:"_s,
                                         u":woman_tone4_beard:"_s,
                                         u":woman_tone5_beard:"_s,
                                         u":man_beard:"_s,
                                         u":man_tone1_beard:"_s,
                                         u":man_tone2_beard:"_s,
                                         u":man_tone3_beard:"_s,
                                         u":man_tone4_beard:"_s,
                                         u":man_tone5_beard:"_s,
                                         u":health_worker:"_s,
                                         u":health_worker_tone1:"_s,
                                         u":health_worker_tone2:"_s,
                                         u":health_worker_tone3:"_s,
                                         u":health_worker_tone4:"_s,
                                         u":health_worker_tone5:"_s,
                                         u":farmer:"_s,
                                         u":farmer_tone1:"_s,
                                         u":farmer_tone2:"_s,
                                         u":farmer_tone3:"_s,
                                         u":farmer_tone4:"_s,
                                         u":farmer_tone5:"_s,
                                         u":cook:"_s,
                                         u":cook_tone1:"_s,
                                         u":cook_tone2:"_s,
                                         u":cook_tone3:"_s,
                                         u":cook_tone4:"_s,
                                         u":cook_tone5:"_s,
                                         u":student:"_s,
                                         u":student_tone1:"_s,
                                         u":student_tone2:"_s,
                                         u":student_tone3:"_s,
                                         u":student_tone4:"_s,
                                         u":student_tone5:"_s,
                                         u":singer:"_s,
                                         u":singer_tone1:"_s,
                                         u":singer_tone2:"_s,
                                         u":singer_tone3:"_s,
                                         u":singer_tone4:"_s,
                                         u":singer_tone5:"_s,
                                         u":teacher:"_s,
                                         u":teacher_tone1:"_s,
                                         u":teacher_tone2:"_s,
                                         u":teacher_tone3:"_s,
                                         u":teacher_tone4:"_s,
                                         u":teacher_tone5:"_s,
                                         u":factory_worker:"_s,
                                         u":factory_worker_tone1:"_s,
                                         u":factory_worker_tone2:"_s,
                                         u":factory_worker_tone3:"_s,
                                         u":factory_worker_tone4:"_s,
                                         u":factory_worker_tone5:"_s,
                                         u":technologist:"_s,
                                         u":technologist_tone1:"_s,
                                         u":technologist_tone2:"_s,
                                         u":technologist_tone3:"_s,
                                         u":technologist_tone4:"_s,
                                         u":technologist_tone5:"_s,
                                         u":office_worker:"_s,
                                         u":office_worker_tone1:"_s,
                                         u":office_worker_tone2:"_s,
                                         u":office_worker_tone3:"_s,
                                         u":office_worker_tone4:"_s,
                                         u":office_worker_tone5:"_s,
                                         u":mechanic:"_s,
                                         u":mechanic_tone1:"_s,
                                         u":mechanic_tone2:"_s,
                                         u":mechanic_tone3:"_s,
                                         u":mechanic_tone4:"_s,
                                         u":mechanic_tone5:"_s,
                                         u":scientist:"_s,
                                         u":scientist_tone1:"_s,
                                         u":scientist_tone2:"_s,
                                         u":scientist_tone3:"_s,
                                         u":scientist_tone4:"_s,
                                         u":scientist_tone5:"_s,
                                         u":artist:"_s,
                                         u":artist_tone1:"_s,
                                         u":artist_tone2:"_s,
                                         u":artist_tone3:"_s,
                                         u":artist_tone4:"_s,
                                         u":artist_tone5:"_s,
                                         u":firefighter:"_s,
                                         u":firefighter_tone1:"_s,
                                         u":firefighter_tone2:"_s,
                                         u":firefighter_tone3:"_s,
                                         u":firefighter_tone4:"_s,
                                         u":firefighter_tone5:"_s,
                                         u":pilot:"_s,
                                         u":pilot_tone1:"_s,
                                         u":pilot_tone2:"_s,
                                         u":pilot_tone3:"_s,
                                         u":pilot_tone4:"_s,
                                         u":pilot_tone5:"_s,
                                         u":astronaut:"_s,
                                         u":astronaut_tone1:"_s,
                                         u":astronaut_tone2:"_s,
                                         u":astronaut_tone3:"_s,
                                         u":astronaut_tone4:"_s,
                                         u":astronaut_tone5:"_s,
                                         u":judge:"_s,
                                         u":judge_tone1:"_s,
                                         u":judge_tone2:"_s,
                                         u":judge_tone3:"_s,
                                         u":judge_tone4:"_s,
                                         u":judge_tone5:"_s,
                                         u":person_with_veil:"_s,
                                         u":person_with_veil_tone1:"_s,
                                         u":person_with_veil_tone2:"_s,
                                         u":person_with_veil_tone3:"_s,
                                         u":person_with_veil_tone4:"_s,
                                         u":person_with_veil_tone5:"_s,
                                         u":woman_with_veil:"_s,
                                         u":woman_with_veil_tone1:"_s,
                                         u":woman_with_veil_tone2:"_s,
                                         u":woman_with_veil_tone3:"_s,
                                         u":woman_with_veil_tone4:"_s,
                                         u":woman_with_veil_tone5:"_s,
                                         u":man_with_veil:"_s,
                                         u":man_with_veil_tone1:"_s,
                                         u":man_with_veil_tone2:"_s,
                                         u":man_with_veil_tone3:"_s,
                                         u":man_with_veil_tone4:"_s,
                                         u":man_with_veil_tone5:"_s,
                                         u":person_in_tuxedo:"_s,
                                         u":person_in_tuxedo_tone1:"_s,
                                         u":person_in_tuxedo_tone2:"_s,
                                         u":person_in_tuxedo_tone3:"_s,
                                         u":person_in_tuxedo_tone4:"_s,
                                         u":person_in_tuxedo_tone5:"_s,
                                         u":woman_in_tuxedo:"_s,
                                         u":woman_in_tuxedo_tone1:"_s,
                                         u":woman_in_tuxedo_tone2:"_s,
                                         u":woman_in_tuxedo_tone3:"_s,
                                         u":woman_in_tuxedo_tone4:"_s,
                                         u":woman_in_tuxedo_tone5:"_s,
                                         u":person_with_crown:"_s,
                                         u":person_with_crown_tone1:"_s,
                                         u":person_with_crown_tone2:"_s,
                                         u":person_with_crown_tone3:"_s,
                                         u":person_with_crown_tone4:"_s,
                                         u":person_with_crown_tone5:"_s,
                                         u":ninja:"_s,
                                         u":ninja_tone1:"_s,
                                         u":ninja_tone2:"_s,
                                         u":ninja_tone3:"_s,
                                         u":ninja_tone4:"_s,
                                         u":ninja_tone5:"_s,
                                         u":mx_claus:"_s,
                                         u":mx_claus_tone1:"_s,
                                         u":mx_claus_tone2:"_s,
                                         u":mx_claus_tone3:"_s,
                                         u":mx_claus_tone4:"_s,
                                         u":mx_claus_tone5:"_s,
                                         u":troll:"_s,
                                         u":pregnant_person:"_s,
                                         u":pregnant_person_tone1:"_s,
                                         u":pregnant_person_tone2:"_s,
                                         u":pregnant_person_tone3:"_s,
                                         u":pregnant_person_tone4:"_s,
                                         u":pregnant_person_tone5:"_s,
                                         u":pregnant_man:"_s,
                                         u":pregnant_man_tone1:"_s,
                                         u":pregnant_man_tone2:"_s,
                                         u":pregnant_man_tone3:"_s,
                                         u":pregnant_man_tone4:"_s,
                                         u":pregnant_man_tone5:"_s,
                                         u":person_feeding_baby:"_s,
                                         u":person_feeding_baby_tone1:"_s,
                                         u":person_feeding_baby_tone2:"_s,
                                         u":person_feeding_baby_tone3:"_s,
                                         u":person_feeding_baby_tone4:"_s,
                                         u":person_feeding_baby_tone5:"_s,
                                         u":woman_feeding_baby:"_s,
                                         u":woman_feeding_baby_tone1:"_s,
                                         u":woman_feeding_baby_tone2:"_s,
                                         u":woman_feeding_baby_tone3:"_s,
                                         u":woman_feeding_baby_tone4:"_s,
                                         u":woman_feeding_baby_tone5:"_s,
                                         u":man_feeding_baby:"_s,
                                         u":man_feeding_baby_tone1:"_s,
                                         u":man_feeding_baby_tone2:"_s,
                                         u":man_feeding_baby_tone3:"_s,
                                         u":man_feeding_baby_tone4:"_s,
                                         u":man_feeding_baby_tone5:"_s,
                                         u":person_in_manual_wheelchair:"_s,
                                         u":person_in_manual_wheelchair_tone1:"_s,
                                         u":person_in_manual_wheelchair_tone2:"_s,
                                         u":person_in_manual_wheelchair_tone3:"_s,
                                         u":person_in_manual_wheelchair_tone4:"_s,
                                         u":person_in_manual_wheelchair_tone5:"_s,
                                         u":person_in_motorized_wheelchair:"_s,
                                         u":person_in_motorized_wheelchair_tone1:"_s,
                                         u":person_in_motorized_wheelchair_tone2:"_s,
                                         u":person_in_motorized_wheelchair_tone3:"_s,
                                         u":person_in_motorized_wheelchair_tone4:"_s,
                                         u":person_in_motorized_wheelchair_tone5:"_s,
                                         u":person_with_probing_cane:"_s,
                                         u":person_with_probing_cane_tone1:"_s,
                                         u":person_with_probing_cane_tone2:"_s,
                                         u":person_with_probing_cane_tone3:"_s,
                                         u":person_with_probing_cane_tone4:"_s,
                                         u":person_with_probing_cane_tone5:"_s,
                                         u":people_holding_hands_tone1_tone2:"_s,
                                         u":people_holding_hands_tone1_tone3:"_s,
                                         u":people_holding_hands_tone1_tone4:"_s,
                                         u":people_holding_hands_tone1_tone5:"_s,
                                         u":people_holding_hands_tone2_tone3:"_s,
                                         u":people_holding_hands_tone2_tone4:"_s,
                                         u":people_holding_hands_tone2_tone5:"_s,
                                         u":people_holding_hands_tone3_tone4:"_s,
                                         u":people_holding_hands_tone3_tone5:"_s,
                                         u":people_holding_hands_tone4_tone5:"_s,
                                         u":women_holding_hands_tone1_tone2:"_s,
                                         u":women_holding_hands_tone1_tone3:"_s,
                                         u":women_holding_hands_tone1_tone4:"_s,
                                         u":women_holding_hands_tone1_tone5:"_s,
                                         u":women_holding_hands_tone2_tone3:"_s,
                                         u":women_holding_hands_tone2_tone4:"_s,
                                         u":women_holding_hands_tone2_tone5:"_s,
                                         u":women_holding_hands_tone3_tone4:"_s,
                                         u":women_holding_hands_tone3_tone5:"_s,
                                         u":women_holding_hands_tone4_tone5:"_s,
                                         u":men_holding_hands_tone1_tone2:"_s,
                                         u":men_holding_hands_tone1_tone3:"_s,
                                         u":men_holding_hands_tone1_tone4:"_s,
                                         u":men_holding_hands_tone1_tone5:"_s,
                                         u":men_holding_hands_tone2_tone3:"_s,
                                         u":men_holding_hands_tone2_tone4:"_s,
                                         u":men_holding_hands_tone2_tone5:"_s,
                                         u":men_holding_hands_tone3_tone4:"_s,
                                         u":men_holding_hands_tone3_tone5:"_s,
                                         u":men_holding_hands_tone4_tone5:"_s,
                                         u":couple_with_heart_tone1:"_s,
                                         u":couple_with_heart_person_person_tone1_tone2:"_s,
                                         u":couple_with_heart_person_person_tone1_tone3:"_s,
                                         u":couple_with_heart_person_person_tone1_tone4:"_s,
                                         u":couple_with_heart_person_person_tone1_tone5:"_s,
                                         u":couple_with_heart_person_person_tone2_tone1:"_s,
                                         u":couple_with_heart_tone2:"_s,
                                         u":couple_with_heart_person_person_tone2_tone3:"_s,
                                         u":couple_with_heart_person_person_tone2_tone4:"_s,
                                         u":couple_with_heart_person_person_tone2_tone5:"_s,
                                         u":couple_with_heart_person_person_tone3_tone1:"_s,
                                         u":couple_with_heart_person_person_tone3_tone2:"_s,
                                         u":couple_with_heart_tone3:"_s,
                                         u":couple_with_heart_person_person_tone3_tone4:"_s,
                                         u":couple_with_heart_person_person_tone3_tone5:"_s,
                                         u":couple_with_heart_person_person_tone4_tone1:"_s,
                                         u":couple_with_heart_person_person_tone4_tone2:"_s,
                                         u":couple_with_heart_person_person_tone4_tone3:"_s,
                                         u":couple_with_heart_tone4:"_s,
                                         u":couple_with_heart_person_person_tone4_tone5:"_s,
                                         u":couple_with_heart_person_person_tone5_tone1:"_s,
                                         u":couple_with_heart_person_person_tone5_tone2:"_s,
                                         u":couple_with_heart_person_person_tone5_tone3:"_s,
                                         u":couple_with_heart_person_person_tone5_tone4:"_s,
                                         u":couple_with_heart_tone5:"_s,
                                         u":couple_with_heart_woman_man_tone1:"_s,
                                         u":couple_with_heart_woman_man_tone1_tone2:"_s,
                                         u":couple_with_heart_woman_man_tone1_tone3:"_s,
                                         u":couple_with_heart_woman_man_tone1_tone4:"_s,
                                         u":couple_with_heart_woman_man_tone1_tone5:"_s,
                                         u":couple_with_heart_woman_man_tone2_tone1:"_s,
                                         u":couple_with_heart_woman_man_tone2:"_s,
                                         u":couple_with_heart_woman_man_tone2_tone3:"_s,
                                         u":couple_with_heart_woman_man_tone2_tone4:"_s,
                                         u":couple_with_heart_woman_man_tone2_tone5:"_s,
                                         u":couple_with_heart_woman_man_tone3_tone1:"_s,
                                         u":couple_with_heart_woman_man_tone3_tone2:"_s,
                                         u":couple_with_heart_woman_man_tone3:"_s,
                                         u":couple_with_heart_woman_man_tone3_tone4:"_s,
                                         u":couple_with_heart_woman_man_tone3_tone5:"_s,
                                         u":couple_with_heart_woman_man_tone4_tone1:"_s,
                                         u":couple_with_heart_woman_man_tone4_tone2:"_s,
                                         u":couple_with_heart_woman_man_tone4_tone3:"_s,
                                         u":couple_with_heart_woman_man_tone4:"_s,
                                         u":couple_with_heart_woman_man_tone4_tone5:"_s,
                                         u":couple_with_heart_woman_man_tone5_tone1:"_s,
                                         u":couple_with_heart_woman_man_tone5_tone2:"_s,
                                         u":couple_with_heart_woman_man_tone5_tone3:"_s,
                                         u":couple_with_heart_woman_man_tone5_tone4:"_s,
                                         u":couple_with_heart_woman_man_tone5:"_s,
                                         u":couple_with_heart_woman_woman_tone1:"_s,
                                         u":couple_with_heart_woman_woman_tone1_tone2:"_s,
                                         u":couple_with_heart_woman_woman_tone1_tone3:"_s,
                                         u":couple_with_heart_woman_woman_tone1_tone4:"_s,
                                         u":couple_with_heart_woman_woman_tone1_tone5:"_s,
                                         u":couple_with_heart_woman_woman_tone2_tone1:"_s,
                                         u":couple_with_heart_woman_woman_tone2:"_s,
                                         u":couple_with_heart_woman_woman_tone2_tone3:"_s,
                                         u":couple_with_heart_woman_woman_tone2_tone4:"_s,
                                         u":couple_with_heart_woman_woman_tone2_tone5:"_s,
                                         u":couple_with_heart_woman_woman_tone3_tone1:"_s,
                                         u":couple_with_heart_woman_woman_tone3_tone2:"_s,
                                         u":couple_with_heart_woman_woman_tone3:"_s,
                                         u":couple_with_heart_woman_woman_tone3_tone4:"_s,
                                         u":couple_with_heart_woman_woman_tone3_tone5:"_s,
                                         u":couple_with_heart_woman_woman_tone4_tone1:"_s,
                                         u":couple_with_heart_woman_woman_tone4_tone2:"_s,
                                         u":couple_with_heart_woman_woman_tone4_tone3:"_s,
                                         u":couple_with_heart_woman_woman_tone4:"_s,
                                         u":couple_with_heart_woman_woman_tone4_tone5:"_s,
                                         u":couple_with_heart_woman_woman_tone5_tone1:"_s,
                                         u":couple_with_heart_woman_woman_tone5_tone2:"_s,
                                         u":couple_with_heart_woman_woman_tone5_tone3:"_s,
                                         u":couple_with_heart_woman_woman_tone5_tone4:"_s,
                                         u":couple_with_heart_woman_woman_tone5:"_s,
                                         u":couple_with_heart_man_man_tone1:"_s,
                                         u":couple_with_heart_man_man_tone1_tone2:"_s,
                                         u":couple_with_heart_man_man_tone1_tone3:"_s,
                                         u":couple_with_heart_man_man_tone1_tone4:"_s,
                                         u":couple_with_heart_man_man_tone1_tone5:"_s,
                                         u":couple_with_heart_man_man_tone2_tone1:"_s,
                                         u":couple_with_heart_man_man_tone2:"_s,
                                         u":couple_with_heart_man_man_tone2_tone3:"_s,
                                         u":couple_with_heart_man_man_tone2_tone4:"_s,
                                         u":couple_with_heart_man_man_tone2_tone5:"_s,
                                         u":couple_with_heart_man_man_tone3_tone1:"_s,
                                         u":couple_with_heart_man_man_tone3_tone2:"_s,
                                         u":couple_with_heart_man_man_tone3:"_s,
                                         u":couple_with_heart_man_man_tone3_tone4:"_s,
                                         u":couple_with_heart_man_man_tone3_tone5:"_s,
                                         u":couple_with_heart_man_man_tone4_tone1:"_s,
                                         u":couple_with_heart_man_man_tone4_tone2:"_s,
                                         u":couple_with_heart_man_man_tone4_tone3:"_s,
                                         u":couple_with_heart_man_man_tone4:"_s,
                                         u":couple_with_heart_man_man_tone4_tone5:"_s,
                                         u":couple_with_heart_man_man_tone5_tone1:"_s,
                                         u":couple_with_heart_man_man_tone5_tone2:"_s,
                                         u":couple_with_heart_man_man_tone5_tone3:"_s,
                                         u":couple_with_heart_man_man_tone5_tone4:"_s,
                                         u":couple_with_heart_man_man_tone5:"_s,
                                         u":kiss_tone1:"_s,
                                         u":kiss_person_person_tone1_tone2:"_s,
                                         u":kiss_person_person_tone1_tone3:"_s,
                                         u":kiss_person_person_tone1_tone4:"_s,
                                         u":kiss_person_person_tone1_tone5:"_s,
                                         u":kiss_person_person_tone2_tone1:"_s,
                                         u":kiss_tone2:"_s,
                                         u":kiss_person_person_tone2_tone3:"_s,
                                         u":kiss_person_person_tone2_tone4:"_s,
                                         u":kiss_person_person_tone2_tone5:"_s,
                                         u":kiss_person_person_tone3_tone1:"_s,
                                         u":kiss_person_person_tone3_tone2:"_s,
                                         u":kiss_tone3:"_s,
                                         u":kiss_person_person_tone3_tone4:"_s,
                                         u":kiss_person_person_tone3_tone5:"_s,
                                         u":kiss_person_person_tone4_tone1:"_s,
                                         u":kiss_person_person_tone4_tone2:"_s,
                                         u":kiss_person_person_tone4_tone3:"_s,
                                         u":kiss_tone4:"_s,
                                         u":kiss_person_person_tone4_tone5:"_s,
                                         u":kiss_person_person_tone5_tone1:"_s,
                                         u":kiss_person_person_tone5_tone2:"_s,
                                         u":kiss_person_person_tone5_tone3:"_s,
                                         u":kiss_person_person_tone5_tone4:"_s,
                                         u":kiss_tone5:"_s,
                                         u":kiss_woman_man_tone1:"_s,
                                         u":kiss_woman_man_tone1_tone2:"_s,
                                         u":kiss_woman_man_tone1_tone3:"_s,
                                         u":kiss_woman_man_tone1_tone4:"_s,
                                         u":kiss_woman_man_tone1_tone5:"_s,
                                         u":kiss_woman_man_tone2_tone1:"_s,
                                         u":kiss_woman_man_tone2:"_s,
                                         u":kiss_woman_man_tone2_tone3:"_s,
                                         u":kiss_woman_man_tone2_tone4:"_s,
                                         u":kiss_woman_man_tone2_tone5:"_s,
                                         u":kiss_woman_man_tone3_tone1:"_s,
                                         u":kiss_woman_man_tone3_tone2:"_s,
                                         u":kiss_woman_man_tone3:"_s,
                                         u":kiss_woman_man_tone3_tone4:"_s,
                                         u":kiss_woman_man_tone3_tone5:"_s,
                                         u":kiss_woman_man_tone4_tone1:"_s,
                                         u":kiss_woman_man_tone4_tone2:"_s,
                                         u":kiss_woman_man_tone4_tone3:"_s,
                                         u":kiss_woman_man_tone4:"_s,
                                         u":kiss_woman_man_tone4_tone5:"_s,
                                         u":kiss_woman_man_tone5_tone1:"_s,
                                         u":kiss_woman_man_tone5_tone2:"_s,
                                         u":kiss_woman_man_tone5_tone3:"_s,
                                         u":kiss_woman_man_tone5_tone4:"_s,
                                         u":kiss_woman_man_tone5:"_s,
                                         u":kiss_woman_woman_tone1:"_s,
                                         u":kiss_woman_woman_tone1_tone2:"_s,
                                         u":kiss_woman_woman_tone1_tone3:"_s,
                                         u":kiss_woman_woman_tone1_tone4:"_s,
                                         u":kiss_woman_woman_tone1_tone5:"_s,
                                         u":kiss_woman_woman_tone2_tone1:"_s,
                                         u":kiss_woman_woman_tone2:"_s,
                                         u":kiss_woman_woman_tone2_tone3:"_s,
                                         u":kiss_woman_woman_tone2_tone4:"_s,
                                         u":kiss_woman_woman_tone2_tone5:"_s,
                                         u":kiss_woman_woman_tone3_tone1:"_s,
                                         u":kiss_woman_woman_tone3_tone2:"_s,
                                         u":kiss_woman_woman_tone3:"_s,
                                         u":kiss_woman_woman_tone3_tone4:"_s,
                                         u":kiss_woman_woman_tone3_tone5:"_s,
                                         u":kiss_woman_woman_tone4_tone1:"_s,
                                         u":kiss_woman_woman_tone4_tone2:"_s,
                                         u":kiss_woman_woman_tone4_tone3:"_s,
                                         u":kiss_woman_woman_tone4:"_s,
                                         u":kiss_woman_woman_tone4_tone5:"_s,
                                         u":kiss_woman_woman_tone5_tone1:"_s,
                                         u":kiss_woman_woman_tone5_tone2:"_s,
                                         u":kiss_woman_woman_tone5_tone3:"_s,
                                         u":kiss_woman_woman_tone5_tone4:"_s,
                                         u":kiss_woman_woman_tone5:"_s,
                                         u":kiss_man_man_tone1:"_s,
                                         u":kiss_man_man_tone1_tone2:"_s,
                                         u":kiss_man_man_tone1_tone3:"_s,
                                         u":kiss_man_man_tone1_tone4:"_s,
                                         u":kiss_man_man_tone1_tone5:"_s,
                                         u":kiss_man_man_tone2_tone1:"_s,
                                         u":kiss_man_man_tone2:"_s,
                                         u":kiss_man_man_tone2_tone3:"_s,
                                         u":kiss_man_man_tone2_tone4:"_s,
                                         u":kiss_man_man_tone2_tone5:"_s,
                                         u":kiss_man_man_tone3_tone1:"_s,
                                         u":kiss_man_man_tone3_tone2:"_s,
                                         u":kiss_man_man_tone3:"_s,
                                         u":kiss_man_man_tone3_tone4:"_s,
                                         u":kiss_man_man_tone3_tone5:"_s,
                                         u":kiss_man_man_tone4_tone1:"_s,
                                         u":kiss_man_man_tone4_tone2:"_s,
                                         u":kiss_man_man_tone4_tone3:"_s,
                                         u":kiss_man_man_tone4:"_s,
                                         u":kiss_man_man_tone4_tone5:"_s,
                                         u":kiss_man_man_tone5_tone1:"_s,
                                         u":kiss_man_man_tone5_tone2:"_s,
                                         u":kiss_man_man_tone5_tone3:"_s,
                                         u":kiss_man_man_tone5_tone4:"_s,
                                         u":kiss_man_man_tone5:"_s,
                                         u":knot:"_s,
                                         u":sewing_needle:"_s,
                                         u":thong_sandal:"_s,
                                         u":military_helmet:"_s,
                                         u":polar_bear:"_s,
                                         u":worm:"_s,
                                         u":lady_beetle:"_s,
                                         u":fly:"_s,
                                         u":cockroach:"_s,
                                         u":seal:"_s,
                                         u":mammoth:"_s,
                                         u":bison:"_s,
                                         u":black_cat:"_s,
                                         u":feather:"_s,
                                         u":dodo:"_s,
                                         u":beaver:"_s,
                                         u":empty_nest:"_s,
                                         u":nest_with_eggs:"_s,
                                         u":coral:"_s,
                                         u":rock:"_s,
                                         u":wood:"_s,
                                         u":potted_plant:"_s,
                                         u":lotus:"_s,
                                         u":bubbles:"_s,
                                         u":blueberries:"_s,
                                         u":olive:"_s,
                                         u":bell_pepper:"_s,
                                         u":flatbread:"_s,
                                         u":tamale:"_s,
                                         u":fondue:"_s,
                                         u":jar:"_s,
                                         u":beans:"_s,
                                         u":pouring_liquid:"_s,
                                         u":teapot:"_s,
                                         u":bubble_tea:"_s,
                                         u":boomerang:"_s,
                                         u":playground_slide:"_s,
                                         u":roller_skate:"_s,
                                         u":long_drum:"_s,
                                         u":accordion:"_s,
                                         u":pickup_truck:"_s,
                                         u":crutch:"_s,
                                         u":wheel:"_s,
                                         u":ring_buoy:"_s,
                                         u":hook:"_s,
                                         u":hut:"_s,
                                         u":mobile_phone:"_s,
                                         u":low_battery:"_s,
                                         u":coin:"_s,
                                         u":identification_card:"_s,
                                         u":ladder:"_s,
                                         u":screwdriver:"_s,
                                         u":carpentry_saw:"_s,
                                         u":mouse_trap:"_s,
                                         u":headstone:"_s,
                                         u":hamsa:"_s,
                                         u":x_ray:"_s,
                                         u":plunger:"_s,
                                         u":toothbrush:"_s,
                                         u":bucket:"_s,
                                         u":nesting_dolls:"_s,
                                         u":mirror:"_s,
                                         u":window:"_s,
                                         u":magic_wand:"_s,
                                         u":piñata:"_s,
                                         u":mirror_ball:"_s,
                                         u":placard:"_s,
                                         u":mending_heart:"_s,
                                         u":heart_on_fire:"_s,
                                         u":elevator:"_s,
                                         u":heavy_equals_sign:"_s,
                                         u":transgender_symbol:"_s,
                                         u":transgender_flag:"_s,
                                         u":flag_bq:"_s,
                                         u":flag_fk:"_s,
                                         u":flag_gf:"_s,
                                         u":flag_tf:"_s,
                                         u":flag_gp:"_s,
                                         u":flag_xk:"_s,
                                         u":flag_mq:"_s,
                                         u":flag_yt:"_s,
                                         u":flag_nc:"_s,
                                         u":flag_re:"_s,
                                         u":flag_gs:"_s,
                                         u":flag_bl:"_s,
                                         u":flag_pm:"_s,
                                         u":flag_wf:"_s,
                                         u":flag_eh:"_s,
                                         u":flag_ea:"_s,
                                         u":flag_dg:"_s,
                                         u":flag_mf:"_s,
                                         u":yo_yo:"_s});
    TextEmoticonsCore::EmojiModelManager::self()->setExcludeEmoticons(lst);
#endif
}

int AccountManager::accountNumber() const
{
    return mRocketChatAccountModel->accountNumber();
}

bool AccountManager::showMessage(const ParseRocketChatUrlUtils::ParsingInfo &parseInfo)
{
    auto rocketChatAccount = mRocketChatAccountModel->accountFromServerUrl(parseInfo.serverHost);
    if (rocketChatAccount) {
        // const QString path{parseUrl.path()};
        const QByteArray messageId = parseInfo.messageId.toLatin1();
        qCDebug(RUQOLA_LOG) << " parseUrl " << parseInfo;
        // https://<server url>/channel/python?msg=sn3gEQom7NcLxTg5h
        setCurrentAccount(rocketChatAccount->accountName());
        // qDebug() << " account->accountName() : " << account->accountName();
        Q_EMIT mCurrentAccount->raiseWindow();
        Q_EMIT mCurrentAccount->selectChannelAndMessage(messageId, parseInfo.roomId, parseInfo.roomIdType, parseInfo.channelType);
        return true;
    }
    return false;
}

void AccountManager::disconnectAccount(RocketChatAccount *account)
{
    disconnect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
    disconnect(account, &RocketChatAccount::roomNeedAttention, this, &AccountManager::roomNeedAttention);
    disconnect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
    disconnect(account, &RocketChatAccount::activitiesChanged, this, &AccountManager::activitiesChanged);
    // TODO connect(account, &RocketChatAccount::notification
}

#if HAVE_ACTIVITY_SUPPORT
ActivitiesManager *AccountManager::activitiesManager() const
{
    return mActivitiesManager;
}
#endif

void AccountManager::connectToAccount(RocketChatAccount *account)
{
    connect(account, &RocketChatAccount::notification, this, [this, account](const NotificationInfo &info) {
        NotificationHistoryManager::self()->addNotification(info);

        switch (info.notificationType()) {
        case NotificationInfo::NotificationType::StandardMessage: {
            if (const Room *room = account->room(info.roomId())) {
                const QByteArray audioNotificationId = room->notificationOptions().audioNotificationValue();
                account->playSound(audioNotificationId);
            } else {
                qCWarning(RUQOLA_SOUND_LOG) << "Room doesn't exist!" << info.roomId();
            }
            auto job = new NotifierJob;
            job->setInfo(info);
            connect(job, &NotifierJob::switchToAccountAndRoomName, this, &AccountManager::slotSwitchToAccountAndRoomName);
            connect(job, &NotifierJob::sendReply, this, [account](const QString &str, const QByteArray &roomId, const QByteArray &tmId) {
                if (tmId.isEmpty()) {
                    account->sendMessage(roomId, str);
                } else {
                    account->replyOnThread(roomId, tmId, str);
                }
                // qDebug() << " str" << str << " Room Name " << roomName;
            });
            job->start();
            break;
        }
        case NotificationInfo::NotificationType::ConferenceCall: {
            break;
        }
        }
    });
    connect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
    connect(account, &RocketChatAccount::roomNeedAttention, this, &AccountManager::roomNeedAttention);
    connect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
    connect(account, &RocketChatAccount::activitiesChanged, this, &AccountManager::activitiesChanged);
}

void AccountManager::slotSwitchToAccountAndRoomName(const QString &accountName, const QByteArray &roomId, const QString &channelType)
{
    setCurrentAccount(accountName);
    QString linkRoom;
    if (channelType == u'c') {
        linkRoom = u"ruqola:/room/%1"_s.arg(QString::fromLatin1(roomId));
    } else {
        linkRoom = u"ruqola:/user/%1"_s.arg(QString::fromLatin1(roomId));
    }
    Q_EMIT mCurrentAccount->raiseWindow();
    Q_EMIT mCurrentAccount->openLinkRequested(linkRoom);
}

bool AccountManager::needToHandleDataMigration() const
{
    const bool needMigration = RuqolaGlobalConfig::self()->databaseVersion() < currentDataBaseVersion;
    return needMigration;
}

void AccountManager::loadAccount()
{
    const bool needDatabaseMigration = needToHandleDataMigration();
    if (needDatabaseMigration) {
        RuqolaGlobalConfig::self()->setDatabaseVersion(currentDataBaseVersion);
        RuqolaGlobalConfig::self()->save();
    }

    qCDebug(RUQOLA_LOG) << " void AccountManager::loadAccount()" << ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()),
                    QStringList() << u"ruqola.conf"_s,
                    QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    QList<RocketChatAccount *> lstAccounts;
    while (it.hasNext()) {
        const QString val = it.next();
        qCDebug(RUQOLA_LOG) << "Account found list.at(i)" << val;
        auto account = new RocketChatAccount(val, needDatabaseMigration);
        if (account->settings()->isValid()) {
            if (account->accountEnabled()) {
                connectToAccount(account);
            }
            lstAccounts.append(account);
        } else {
            account->deleteLater();
        }
    }

    mRocketChatAccountModel->setAccounts(lstAccounts);

    QSettings settings;
    const QString currentAccount = settings.value("currentAccount"_L1, QString()).toString();
    if (currentAccount.isEmpty()) {
        auto account = mRocketChatAccountModel->account(0);
        if (account && account->accountEnabled()) {
            // Use first one
            mCurrentAccount = account;
        }
    } else {
        selectAccount(currentAccount);
        if (!mCurrentAccount) {
            // Use first one
            auto account = mRocketChatAccountModel->account(0);
            if (account && account->accountEnabled()) {
                mCurrentAccount = account;
            }
        }
    }
    mRocketChatAccountProxyModel->setAccountOrder(RuqolaGlobalConfig::self()->accountOrder());
}

RocketChatAccountFilterProxyModel *AccountManager::rocketChatAccountProxyModel() const
{
    return mRocketChatAccountProxyModel;
}

RocketChatAccount *AccountManager::account() const
{
    return mCurrentAccount;
}

void AccountManager::changeEnableState(RocketChatAccount *account, bool enabled)
{
    if (enabled) {
        connectToAccount(account);
    } else {
        disconnectAccount(account);
    }
}

void AccountManager::addInvitedAccount(const AccountManagerInfo &info)
{
    auto job = new ValidateInviteServerJob(this);
    job->setInfo(info);
    connect(job, &ValidateInviteServerJob::tokenIsInvalid, this, []() {
        auto notification = new KNotification(u"Invite-Account-Invalid"_s, KNotification::CloseOnTimeout);
        notification->setTitle(i18n("Invalid invite token"));
        notification->setText(i18n("Invite Token is invalid. Creating account canceled."));
        notification->sendEvent();
    });
    connect(job, &ValidateInviteServerJob::tokenIsValid, this, [](const AccountManager::AccountManagerInfo &info) {
        Q_EMIT Ruqola::self()->addInviteServer(info);
    });
    job->start();
}

void AccountManager::addAccount(const AccountManagerInfo &info)
{
    const QString newAccountName = Utils::createUniqueAccountName(accountsName(), info.accountName);
    auto account = new RocketChatAccount();
    account->setAccountName(newAccountName);
    account->setServerUrl(info.serverUrl);
    auto settings = account->settings();
    settings->setAccountEnabled(info.enabled);
    settings->setActivities(info.activitiesSettings.activities);
    settings->setActivityEnabled(info.activitiesSettings.enabled);
    settings->setInviteToken(info.inviteToken);
    if (info.authMethodType == AuthenticationManager::AuthMethodType::Password) {
        settings->setUserName(info.userName);
        settings->setPassword(info.password);
    } else if (info.authMethodType == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
        settings->setAuthToken(info.token);
        settings->setUserId(info.userId);
    } else {
        // TODO for other authMethodType ?
        // google used ?
        // Fb ?
        // Gitlab ?
        // GitHub ?
    }
    settings->setAuthMethodType(info.authMethodType);
    if (info.enabled) {
        connectToAccount(account);
    }
    addAccount(account);
}

void AccountManager::modifyAccount(const AccountManagerInfo &info)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(info.accountName);
    if (account) {
        auto settings = account->settings();
        settings->setDisplayName(info.displayName);
        account->setServerUrl(info.serverUrl);
        settings->setAccountEnabled(info.enabled);
        settings->setAuthMethodType(info.authMethodType);
        settings->setActivities(info.activitiesSettings.activities);
        settings->setActivityEnabled(info.activitiesSettings.enabled);
        if (info.authMethodType == AuthenticationManager::AuthMethodType::Password) {
            settings->setUserName(info.userName);
            // TODO add password ???
        } else if (info.authMethodType == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
            settings->setAuthToken(info.token);
            settings->setUserId(info.userId);
        } else {
            // TODO ????
        }
        if (!info.enabled && account->accountEnabled()) {
            changeEnableState(account, false);
        } else if (info.enabled && !account->accountEnabled()) {
            changeEnableState(account, true);
        }
    }
}

RocketChatAccount *AccountManager::accountFromName(const QString &accountName)
{
    return mRocketChatAccountModel->account(accountName);
}

QStringList AccountManager::accountsName() const
{
    return mRocketChatAccountModel->accountsName();
}

bool AccountManager::isEmpty() const
{
    return mRocketChatAccountModel->isEmpty();
}

void AccountManager::addAccount(RocketChatAccount *account)
{
    const bool wasEmpty = isEmpty();
    mRocketChatAccountModel->insertAccount(account);
    const QString accountName = account->accountName();
    if (wasEmpty) {
        setCurrentAccount(accountName);
    }
}

void AccountManager::selectAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account && account->accountEnabled()) {
        mCurrentAccount = account;
    } else {
        mCurrentAccount = nullptr;
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system.";
    }
}

void AccountManager::setCurrentAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        if (mCurrentAccount != account) {
            QSettings settings;
            settings.setValue("currentAccount"_L1, accountName);
            settings.sync();
            mCurrentAccount = account;
            Q_EMIT currentAccountChanged();
        }
    } else {
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system. Fallback to default one.";
    }
}

QString AccountManager::currentAccount() const
{
    return mCurrentAccount ? mCurrentAccount->accountName() : QString();
}

void AccountManager::removeLogs(const QString &accountName)
{
    const QString directory = LocalDatabaseUtils::localMessageLoggerPath() + accountName;
    removeDirectory(directory);
}

void AccountManager::removeDatabaseAccount(const QString &accountName)
{
    const QString directory = LocalDatabaseUtils::localAccountsDatabasePath() + accountName;
    removeDirectory(directory);
}

void AccountManager::removeDirectory(const QString &directory)
{
    QDir dir(directory);
    if (dir.exists()) {
        if (!dir.removeRecursively()) {
            qCWarning(RUQOLA_LOG) << " Impossible to remove directory : " << directory;
        }
    }
}

void AccountManager::removeAccount(const QString &accountName, bool removeLogFiles)
{
    auto account = mRocketChatAccountModel->removeAccount(accountName);
    if (mRocketChatAccountModel->accountNumber() > 0) {
        mCurrentAccount = mRocketChatAccountModel->account(0);
        removeDatabaseAccount(accountName);
        // TODO remove others files
        if (removeLogFiles) {
            removeLogs(accountName);
        }
    } else {
        // TODO create new dummy account !
    }
    Q_EMIT currentAccountChanged();
    if (account) {
        account->deleteLater();
    }
}

RocketChatAccountModel *AccountManager::rocketChatAccountModel() const
{
    return mRocketChatAccountModel;
}

QList<AccountManager::AccountDisplayInfo> AccountManager::accountDisplayInfoSorted() const
{
    QList<AccountManager::AccountDisplayInfo> lst;
    auto model = rocketChatAccountProxyModel();
    for (int i = 0; i < model->rowCount(); ++i) {
        const auto index = model->index(i, 0);
        auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
        if (account->accountEnabled()) {
            AccountManager::AccountDisplayInfo info;
            info.name = account->settings()->displayName();
            info.icon = Utils::iconFromAccount(account);
            lst.append(std::move(info));
        }
    }
    return lst;
}

QDebug operator<<(QDebug d, const AccountManager::AccountManagerInfo &t)
{
    d.space() << "authenticationInfos" << t.authenticationInfos;
    d.space() << "displayName" << t.displayName;
    d.space() << "accountName" << t.accountName;
    d.space() << "userName" << t.userName;
    d.space() << "serverUrl" << t.serverUrl;
    d.space() << "token" << t.token;
    d.space() << "userId" << t.userId;
    d.space() << "authMethodType" << t.authMethodType;
    d.space() << "canResetPassword" << t.canResetPassword;
    d.space() << "enabled" << t.enabled;
    d.space() << "canRegisterAccount" << t.canRegisterAccount;
    d.space() << "activities" << t.activitiesSettings.activities;
    d.space() << "activities enabled" << t.activitiesSettings.enabled;
    d.space() << "invite token" << t.inviteToken;
    return d;
}

#include "moc_accountmanager.cpp"
