/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountmanager.h"
#include "config-ruqola.h"
#include "localdatabase/localdatabaseutils.h"
#include "managerdatapaths.h"
#include "notificationhistorymanager.h"
#include "notifications/notifierjob.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "ruqolaglobalconfig.h"
#include "utils.h"
#include <KLocalizedString>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <TextEmoticonsCore/EmojiModelManager>

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
    , mRocketChatAccountModel(new RocketChatAccountModel(this))
    , mRocketChatAccountProxyModel(new RocketChatAccountFilterProxyModel(this))
{
    mRocketChatAccountProxyModel->setSourceModel(mRocketChatAccountModel);
    loadExcludeEmoticons();
    loadAccount();
}

AccountManager::~AccountManager() = default;

void AccountManager::loadExcludeEmoticons()
{
#if HAVE_TEXTADDONS_TEXTEMOTICON_EXCLUDEEMOTICON_SUPPORT
    const QStringList lst = QStringList({QStringLiteral(":face_holding_back_tears:"),
                                         QStringLiteral(":smiling_face_with_tear:"),
                                         QStringLiteral(":disguised_face:"),
                                         QStringLiteral(":face_in_clouds:"),
                                         QStringLiteral(":face_with_peeking_eye:"),
                                         QStringLiteral(":face_with_open_eyes_and_hand_over_mouth:"),
                                         QStringLiteral(":saluting_face:"),
                                         QStringLiteral(":melting_face:"),
                                         QStringLiteral(":dotted_line_face:"),
                                         QStringLiteral(":face_with_diagonal_mouth:"),
                                         QStringLiteral(":face_exhaling:"),
                                         QStringLiteral(":face_with_spiral_eyes:"),
                                         QStringLiteral(":heart_hands:"),
                                         QStringLiteral(":heart_hands_tone1:"),
                                         QStringLiteral(":heart_hands_tone2:"),
                                         QStringLiteral(":heart_hands_tone3:"),
                                         QStringLiteral(":heart_hands_tone4:"),
                                         QStringLiteral(":heart_hands_tone5:"),
                                         QStringLiteral(":handshake_tone1:"),
                                         QStringLiteral(":handshake_tone1_tone2:"),
                                         QStringLiteral(":handshake_tone1_tone3:"),
                                         QStringLiteral(":handshake_tone1_tone4:"),
                                         QStringLiteral(":handshake_tone1_tone5:"),
                                         QStringLiteral(":handshake_tone2_tone1:"),
                                         QStringLiteral(":handshake_tone2:"),
                                         QStringLiteral(":handshake_tone2_tone3:"),
                                         QStringLiteral(":handshake_tone2_tone4:"),
                                         QStringLiteral(":handshake_tone2_tone5:"),
                                         QStringLiteral(":handshake_tone3_tone1:"),
                                         QStringLiteral(":handshake_tone3_tone2:"),
                                         QStringLiteral(":handshake_tone3:"),
                                         QStringLiteral(":handshake_tone3_tone4:"),
                                         QStringLiteral(":handshake_tone3_tone5:"),
                                         QStringLiteral(":handshake_tone4_tone1:"),
                                         QStringLiteral(":handshake_tone4_tone2:"),
                                         QStringLiteral(":handshake_tone4_tone3:"),
                                         QStringLiteral(":handshake_tone4:"),
                                         QStringLiteral(":handshake_tone4_tone5:"),
                                         QStringLiteral(":handshake_tone5_tone1:"),
                                         QStringLiteral(":handshake_tone5_tone2:"),
                                         QStringLiteral(":handshake_tone5_tone3:"),
                                         QStringLiteral(":handshake_tone5_tone4:"),
                                         QStringLiteral(":handshake_tone5:"),
                                         QStringLiteral(":hand_with_index_finger_and_thumb_crossed:"),
                                         QStringLiteral(":hand_with_index_finger_and_thumb_crossed_tone1:"),
                                         QStringLiteral(":hand_with_index_finger_and_thumb_crossed_tone2:"),
                                         QStringLiteral(":hand_with_index_finger_and_thumb_crossed_tone3:"),
                                         QStringLiteral(":hand_with_index_finger_and_thumb_crossed_tone4:"),
                                         QStringLiteral(":hand_with_index_finger_and_thumb_crossed_tone5:"),
                                         QStringLiteral(":pinched_fingers:"),
                                         QStringLiteral(":pinched_fingers_tone2:"),
                                         QStringLiteral(":pinched_fingers_tone1:"),
                                         QStringLiteral(":pinched_fingers_tone3:"),
                                         QStringLiteral(":pinched_fingers_tone4:"),
                                         QStringLiteral(":pinched_fingers_tone5:"),
                                         QStringLiteral(":palm_down_hand:"),
                                         QStringLiteral(":palm_down_hand_tone1:"),
                                         QStringLiteral(":palm_down_hand_tone2:"),
                                         QStringLiteral(":palm_down_hand_tone3:"),
                                         QStringLiteral(":palm_down_hand_tone4:"),
                                         QStringLiteral(":palm_down_hand_tone5:"),
                                         QStringLiteral(":palm_up_hand:"),
                                         QStringLiteral(":palm_up_hand_tone1:"),
                                         QStringLiteral(":palm_up_hand_tone2:"),
                                         QStringLiteral(":palm_up_hand_tone3:"),
                                         QStringLiteral(":palm_up_hand_tone4:"),
                                         QStringLiteral(":palm_up_hand_tone5:"),
                                         QStringLiteral(":leftwards_hand:"),
                                         QStringLiteral(":leftwards_hand_tone1:"),
                                         QStringLiteral(":leftwards_hand_tone2:"),
                                         QStringLiteral(":leftwards_hand_tone3:"),
                                         QStringLiteral(":leftwards_hand_tone4:"),
                                         QStringLiteral(":leftwards_hand_tone5:"),
                                         QStringLiteral(":rightwards_hand:"),
                                         QStringLiteral(":rightwards_hand_tone1:"),
                                         QStringLiteral(":rightwards_hand_tone2:"),
                                         QStringLiteral(":rightwards_hand_tone3:"),
                                         QStringLiteral(":rightwards_hand_tone4:"),
                                         QStringLiteral(":rightwards_hand_tone5:"),
                                         QStringLiteral(":index_pointing_at_the_viewer:"),
                                         QStringLiteral(":index_pointing_at_the_viewer_tone1:"),
                                         QStringLiteral(":index_pointing_at_the_viewer_tone2:"),
                                         QStringLiteral(":index_pointing_at_the_viewer_tone3:"),
                                         QStringLiteral(":index_pointing_at_the_viewer_tone4:"),
                                         QStringLiteral(":index_pointing_at_the_viewer_tone5:"),
                                         QStringLiteral(":biting_lip:"),
                                         QStringLiteral(":anatomical_heart:"),
                                         QStringLiteral(":lungs:"),
                                         QStringLiteral(":people_hugging:"),
                                         QStringLiteral(":person_curly_hair:"),
                                         QStringLiteral(":person_tone1_curly_hair:"),
                                         QStringLiteral(":person_tone2_curly_hair:"),
                                         QStringLiteral(":person_tone3_curly_hair:"),
                                         QStringLiteral(":person_tone4_curly_hair:"),
                                         QStringLiteral(":person_tone5_curly_hair:"),
                                         QStringLiteral(":person_red_hair:"),
                                         QStringLiteral(":person_tone1_red_hair:"),
                                         QStringLiteral(":person_tone2_red_hair:"),
                                         QStringLiteral(":person_tone3_red_hair:"),
                                         QStringLiteral(":person_tone4_red_hair:"),
                                         QStringLiteral(":person_tone5_red_hair:"),
                                         QStringLiteral(":person_white_hair:"),
                                         QStringLiteral(":person_tone1_white_hair:"),
                                         QStringLiteral(":person_tone2_white_hair:"),
                                         QStringLiteral(":person_tone3_white_hair:"),
                                         QStringLiteral(":person_tone4_white_hair:"),
                                         QStringLiteral(":person_tone5_white_hair:"),
                                         QStringLiteral(":person_bald:"),
                                         QStringLiteral(":person_tone1_bald:"),
                                         QStringLiteral(":person_tone2_bald:"),
                                         QStringLiteral(":person_tone3_bald:"),
                                         QStringLiteral(":person_tone4_bald:"),
                                         QStringLiteral(":person_tone5_bald:"),
                                         QStringLiteral(":woman_beard:"),
                                         QStringLiteral(":woman_tone1_beard:"),
                                         QStringLiteral(":woman_tone2_beard:"),
                                         QStringLiteral(":woman_tone3_beard:"),
                                         QStringLiteral(":woman_tone4_beard:"),
                                         QStringLiteral(":woman_tone5_beard:"),
                                         QStringLiteral(":man_beard:"),
                                         QStringLiteral(":man_tone1_beard:"),
                                         QStringLiteral(":man_tone2_beard:"),
                                         QStringLiteral(":man_tone3_beard:"),
                                         QStringLiteral(":man_tone4_beard:"),
                                         QStringLiteral(":man_tone5_beard:"),
                                         QStringLiteral(":health_worker:"),
                                         QStringLiteral(":health_worker_tone1:"),
                                         QStringLiteral(":health_worker_tone2:"),
                                         QStringLiteral(":health_worker_tone3:"),
                                         QStringLiteral(":health_worker_tone4:"),
                                         QStringLiteral(":health_worker_tone5:"),
                                         QStringLiteral(":farmer:"),
                                         QStringLiteral(":farmer_tone1:"),
                                         QStringLiteral(":farmer_tone2:"),
                                         QStringLiteral(":farmer_tone3:"),
                                         QStringLiteral(":farmer_tone4:"),
                                         QStringLiteral(":farmer_tone5:"),
                                         QStringLiteral(":cook:"),
                                         QStringLiteral(":cook_tone1:"),
                                         QStringLiteral(":cook_tone2:"),
                                         QStringLiteral(":cook_tone3:"),
                                         QStringLiteral(":cook_tone4:"),
                                         QStringLiteral(":cook_tone5:"),
                                         QStringLiteral(":student:"),
                                         QStringLiteral(":student_tone1:"),
                                         QStringLiteral(":student_tone2:"),
                                         QStringLiteral(":student_tone3:"),
                                         QStringLiteral(":student_tone4:"),
                                         QStringLiteral(":student_tone5:"),
                                         QStringLiteral(":singer:"),
                                         QStringLiteral(":singer_tone1:"),
                                         QStringLiteral(":singer_tone2:"),
                                         QStringLiteral(":singer_tone3:"),
                                         QStringLiteral(":singer_tone4:"),
                                         QStringLiteral(":singer_tone5:"),
                                         QStringLiteral(":teacher:"),
                                         QStringLiteral(":teacher_tone1:"),
                                         QStringLiteral(":teacher_tone2:"),
                                         QStringLiteral(":teacher_tone3:"),
                                         QStringLiteral(":teacher_tone4:"),
                                         QStringLiteral(":teacher_tone5:"),
                                         QStringLiteral(":factory_worker:"),
                                         QStringLiteral(":factory_worker_tone1:"),
                                         QStringLiteral(":factory_worker_tone2:"),
                                         QStringLiteral(":factory_worker_tone3:"),
                                         QStringLiteral(":factory_worker_tone4:"),
                                         QStringLiteral(":factory_worker_tone5:"),
                                         QStringLiteral(":technologist:"),
                                         QStringLiteral(":technologist_tone1:"),
                                         QStringLiteral(":technologist_tone2:"),
                                         QStringLiteral(":technologist_tone3:"),
                                         QStringLiteral(":technologist_tone4:"),
                                         QStringLiteral(":technologist_tone5:"),
                                         QStringLiteral(":office_worker:"),
                                         QStringLiteral(":office_worker_tone1:"),
                                         QStringLiteral(":office_worker_tone2:"),
                                         QStringLiteral(":office_worker_tone3:"),
                                         QStringLiteral(":office_worker_tone4:"),
                                         QStringLiteral(":office_worker_tone5:"),
                                         QStringLiteral(":mechanic:"),
                                         QStringLiteral(":mechanic_tone1:"),
                                         QStringLiteral(":mechanic_tone2:"),
                                         QStringLiteral(":mechanic_tone3:"),
                                         QStringLiteral(":mechanic_tone4:"),
                                         QStringLiteral(":mechanic_tone5:"),
                                         QStringLiteral(":scientist:"),
                                         QStringLiteral(":scientist_tone1:"),
                                         QStringLiteral(":scientist_tone2:"),
                                         QStringLiteral(":scientist_tone3:"),
                                         QStringLiteral(":scientist_tone4:"),
                                         QStringLiteral(":scientist_tone5:"),
                                         QStringLiteral(":artist:"),
                                         QStringLiteral(":artist_tone1:"),
                                         QStringLiteral(":artist_tone2:"),
                                         QStringLiteral(":artist_tone3:"),
                                         QStringLiteral(":artist_tone4:"),
                                         QStringLiteral(":artist_tone5:"),
                                         QStringLiteral(":firefighter:"),
                                         QStringLiteral(":firefighter_tone1:"),
                                         QStringLiteral(":firefighter_tone2:"),
                                         QStringLiteral(":firefighter_tone3:"),
                                         QStringLiteral(":firefighter_tone4:"),
                                         QStringLiteral(":firefighter_tone5:"),
                                         QStringLiteral(":pilot:"),
                                         QStringLiteral(":pilot_tone1:"),
                                         QStringLiteral(":pilot_tone2:"),
                                         QStringLiteral(":pilot_tone3:"),
                                         QStringLiteral(":pilot_tone4:"),
                                         QStringLiteral(":pilot_tone5:"),
                                         QStringLiteral(":astronaut:"),
                                         QStringLiteral(":astronaut_tone1:"),
                                         QStringLiteral(":astronaut_tone2:"),
                                         QStringLiteral(":astronaut_tone3:"),
                                         QStringLiteral(":astronaut_tone4:"),
                                         QStringLiteral(":astronaut_tone5:"),
                                         QStringLiteral(":judge:"),
                                         QStringLiteral(":judge_tone1:"),
                                         QStringLiteral(":judge_tone2:"),
                                         QStringLiteral(":judge_tone3:"),
                                         QStringLiteral(":judge_tone4:"),
                                         QStringLiteral(":judge_tone5:"),
                                         QStringLiteral(":person_with_veil:"),
                                         QStringLiteral(":person_with_veil_tone1:"),
                                         QStringLiteral(":person_with_veil_tone2:"),
                                         QStringLiteral(":person_with_veil_tone3:"),
                                         QStringLiteral(":person_with_veil_tone4:"),
                                         QStringLiteral(":person_with_veil_tone5:"),
                                         QStringLiteral(":woman_with_veil:"),
                                         QStringLiteral(":woman_with_veil_tone1:"),
                                         QStringLiteral(":woman_with_veil_tone2:"),
                                         QStringLiteral(":woman_with_veil_tone3:"),
                                         QStringLiteral(":woman_with_veil_tone4:"),
                                         QStringLiteral(":woman_with_veil_tone5:"),
                                         QStringLiteral(":man_with_veil:"),
                                         QStringLiteral(":man_with_veil_tone1:"),
                                         QStringLiteral(":man_with_veil_tone2:"),
                                         QStringLiteral(":man_with_veil_tone3:"),
                                         QStringLiteral(":man_with_veil_tone4:"),
                                         QStringLiteral(":man_with_veil_tone5:"),
                                         QStringLiteral(":person_in_tuxedo:"),
                                         QStringLiteral(":person_in_tuxedo_tone1:"),
                                         QStringLiteral(":person_in_tuxedo_tone2:"),
                                         QStringLiteral(":person_in_tuxedo_tone3:"),
                                         QStringLiteral(":person_in_tuxedo_tone4:"),
                                         QStringLiteral(":person_in_tuxedo_tone5:"),
                                         QStringLiteral(":woman_in_tuxedo:"),
                                         QStringLiteral(":woman_in_tuxedo_tone1:"),
                                         QStringLiteral(":woman_in_tuxedo_tone2:"),
                                         QStringLiteral(":woman_in_tuxedo_tone3:"),
                                         QStringLiteral(":woman_in_tuxedo_tone4:"),
                                         QStringLiteral(":woman_in_tuxedo_tone5:"),
                                         QStringLiteral(":person_with_crown:"),
                                         QStringLiteral(":person_with_crown_tone1:"),
                                         QStringLiteral(":person_with_crown_tone2:"),
                                         QStringLiteral(":person_with_crown_tone3:"),
                                         QStringLiteral(":person_with_crown_tone4:"),
                                         QStringLiteral(":person_with_crown_tone5:"),
                                         QStringLiteral(":ninja:"),
                                         QStringLiteral(":ninja_tone1:"),
                                         QStringLiteral(":ninja_tone2:"),
                                         QStringLiteral(":ninja_tone3:"),
                                         QStringLiteral(":ninja_tone4:"),
                                         QStringLiteral(":ninja_tone5:"),
                                         QStringLiteral(":mx_claus:"),
                                         QStringLiteral(":mx_claus_tone1:"),
                                         QStringLiteral(":mx_claus_tone2:"),
                                         QStringLiteral(":mx_claus_tone3:"),
                                         QStringLiteral(":mx_claus_tone4:"),
                                         QStringLiteral(":mx_claus_tone5:"),
                                         QStringLiteral(":troll:"),
                                         QStringLiteral(":pregnant_person:"),
                                         QStringLiteral(":pregnant_person_tone1:"),
                                         QStringLiteral(":pregnant_person_tone2:"),
                                         QStringLiteral(":pregnant_person_tone3:"),
                                         QStringLiteral(":pregnant_person_tone4:"),
                                         QStringLiteral(":pregnant_person_tone5:"),
                                         QStringLiteral(":pregnant_man:"),
                                         QStringLiteral(":pregnant_man_tone1:"),
                                         QStringLiteral(":pregnant_man_tone2:"),
                                         QStringLiteral(":pregnant_man_tone3:"),
                                         QStringLiteral(":pregnant_man_tone4:"),
                                         QStringLiteral(":pregnant_man_tone5:"),
                                         QStringLiteral(":person_feeding_baby:"),
                                         QStringLiteral(":person_feeding_baby_tone1:"),
                                         QStringLiteral(":person_feeding_baby_tone2:"),
                                         QStringLiteral(":person_feeding_baby_tone3:"),
                                         QStringLiteral(":person_feeding_baby_tone4:"),
                                         QStringLiteral(":person_feeding_baby_tone5:"),
                                         QStringLiteral(":woman_feeding_baby:"),
                                         QStringLiteral(":woman_feeding_baby_tone1:"),
                                         QStringLiteral(":woman_feeding_baby_tone2:"),
                                         QStringLiteral(":woman_feeding_baby_tone3:"),
                                         QStringLiteral(":woman_feeding_baby_tone4:"),
                                         QStringLiteral(":woman_feeding_baby_tone5:"),
                                         QStringLiteral(":man_feeding_baby:"),
                                         QStringLiteral(":man_feeding_baby_tone1:"),
                                         QStringLiteral(":man_feeding_baby_tone2:"),
                                         QStringLiteral(":man_feeding_baby_tone3:"),
                                         QStringLiteral(":man_feeding_baby_tone4:"),
                                         QStringLiteral(":man_feeding_baby_tone5:"),
                                         QStringLiteral(":person_in_manual_wheelchair:"),
                                         QStringLiteral(":person_in_manual_wheelchair_tone1:"),
                                         QStringLiteral(":person_in_manual_wheelchair_tone2:"),
                                         QStringLiteral(":person_in_manual_wheelchair_tone3:"),
                                         QStringLiteral(":person_in_manual_wheelchair_tone4:"),
                                         QStringLiteral(":person_in_manual_wheelchair_tone5:"),
                                         QStringLiteral(":person_in_motorized_wheelchair:"),
                                         QStringLiteral(":person_in_motorized_wheelchair_tone1:"),
                                         QStringLiteral(":person_in_motorized_wheelchair_tone2:"),
                                         QStringLiteral(":person_in_motorized_wheelchair_tone3:"),
                                         QStringLiteral(":person_in_motorized_wheelchair_tone4:"),
                                         QStringLiteral(":person_in_motorized_wheelchair_tone5:"),
                                         QStringLiteral(":person_with_probing_cane:"),
                                         QStringLiteral(":person_with_probing_cane_tone1:"),
                                         QStringLiteral(":person_with_probing_cane_tone2:"),
                                         QStringLiteral(":person_with_probing_cane_tone3:"),
                                         QStringLiteral(":person_with_probing_cane_tone4:"),
                                         QStringLiteral(":person_with_probing_cane_tone5:"),
                                         QStringLiteral(":people_holding_hands_tone1_tone2:"),
                                         QStringLiteral(":people_holding_hands_tone1_tone3:"),
                                         QStringLiteral(":people_holding_hands_tone1_tone4:"),
                                         QStringLiteral(":people_holding_hands_tone1_tone5:"),
                                         QStringLiteral(":people_holding_hands_tone2_tone3:"),
                                         QStringLiteral(":people_holding_hands_tone2_tone4:"),
                                         QStringLiteral(":people_holding_hands_tone2_tone5:"),
                                         QStringLiteral(":people_holding_hands_tone3_tone4:"),
                                         QStringLiteral(":people_holding_hands_tone3_tone5:"),
                                         QStringLiteral(":people_holding_hands_tone4_tone5:"),
                                         QStringLiteral(":women_holding_hands_tone1_tone2:"),
                                         QStringLiteral(":women_holding_hands_tone1_tone3:"),
                                         QStringLiteral(":women_holding_hands_tone1_tone4:"),
                                         QStringLiteral(":women_holding_hands_tone1_tone5:"),
                                         QStringLiteral(":women_holding_hands_tone2_tone3:"),
                                         QStringLiteral(":women_holding_hands_tone2_tone4:"),
                                         QStringLiteral(":women_holding_hands_tone2_tone5:"),
                                         QStringLiteral(":women_holding_hands_tone3_tone4:"),
                                         QStringLiteral(":women_holding_hands_tone3_tone5:"),
                                         QStringLiteral(":women_holding_hands_tone4_tone5:"),
                                         QStringLiteral(":men_holding_hands_tone1_tone2:"),
                                         QStringLiteral(":men_holding_hands_tone1_tone3:"),
                                         QStringLiteral(":men_holding_hands_tone1_tone4:"),
                                         QStringLiteral(":men_holding_hands_tone1_tone5:"),
                                         QStringLiteral(":men_holding_hands_tone2_tone3:"),
                                         QStringLiteral(":men_holding_hands_tone2_tone4:"),
                                         QStringLiteral(":men_holding_hands_tone2_tone5:"),
                                         QStringLiteral(":men_holding_hands_tone3_tone4:"),
                                         QStringLiteral(":men_holding_hands_tone3_tone5:"),
                                         QStringLiteral(":men_holding_hands_tone4_tone5:"),
                                         QStringLiteral(":couple_with_heart_tone1:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone1_tone2:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone1_tone3:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone1_tone4:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone1_tone5:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone2_tone1:"),
                                         QStringLiteral(":couple_with_heart_tone2:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone2_tone3:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone2_tone4:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone2_tone5:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone3_tone1:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone3_tone2:"),
                                         QStringLiteral(":couple_with_heart_tone3:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone3_tone4:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone3_tone5:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone4_tone1:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone4_tone2:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone4_tone3:"),
                                         QStringLiteral(":couple_with_heart_tone4:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone4_tone5:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone5_tone1:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone5_tone2:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone5_tone3:"),
                                         QStringLiteral(":couple_with_heart_person_person_tone5_tone4:"),
                                         QStringLiteral(":couple_with_heart_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone1_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone1_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone1_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone1_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone2_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone2_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone2_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone2_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone3_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone3_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone3_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone3_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone4_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone4_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone4_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone4_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone5_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone5_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone5_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone5_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_man_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone1_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone1_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone1_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone1_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone2_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone2_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone2_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone2_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone3_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone3_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone3_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone3_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone4_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone4_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone4_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone4_tone5:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone5_tone1:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone5_tone2:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone5_tone3:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone5_tone4:"),
                                         QStringLiteral(":couple_with_heart_woman_woman_tone5:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone1:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone1_tone2:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone1_tone3:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone1_tone4:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone1_tone5:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone2_tone1:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone2:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone2_tone3:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone2_tone4:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone2_tone5:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone3_tone1:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone3_tone2:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone3:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone3_tone4:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone3_tone5:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone4_tone1:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone4_tone2:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone4_tone3:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone4:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone4_tone5:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone5_tone1:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone5_tone2:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone5_tone3:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone5_tone4:"),
                                         QStringLiteral(":couple_with_heart_man_man_tone5:"),
                                         QStringLiteral(":kiss_tone1:"),
                                         QStringLiteral(":kiss_person_person_tone1_tone2:"),
                                         QStringLiteral(":kiss_person_person_tone1_tone3:"),
                                         QStringLiteral(":kiss_person_person_tone1_tone4:"),
                                         QStringLiteral(":kiss_person_person_tone1_tone5:"),
                                         QStringLiteral(":kiss_person_person_tone2_tone1:"),
                                         QStringLiteral(":kiss_tone2:"),
                                         QStringLiteral(":kiss_person_person_tone2_tone3:"),
                                         QStringLiteral(":kiss_person_person_tone2_tone4:"),
                                         QStringLiteral(":kiss_person_person_tone2_tone5:"),
                                         QStringLiteral(":kiss_person_person_tone3_tone1:"),
                                         QStringLiteral(":kiss_person_person_tone3_tone2:"),
                                         QStringLiteral(":kiss_tone3:"),
                                         QStringLiteral(":kiss_person_person_tone3_tone4:"),
                                         QStringLiteral(":kiss_person_person_tone3_tone5:"),
                                         QStringLiteral(":kiss_person_person_tone4_tone1:"),
                                         QStringLiteral(":kiss_person_person_tone4_tone2:"),
                                         QStringLiteral(":kiss_person_person_tone4_tone3:"),
                                         QStringLiteral(":kiss_tone4:"),
                                         QStringLiteral(":kiss_person_person_tone4_tone5:"),
                                         QStringLiteral(":kiss_person_person_tone5_tone1:"),
                                         QStringLiteral(":kiss_person_person_tone5_tone2:"),
                                         QStringLiteral(":kiss_person_person_tone5_tone3:"),
                                         QStringLiteral(":kiss_person_person_tone5_tone4:"),
                                         QStringLiteral(":kiss_tone5:"),
                                         QStringLiteral(":kiss_woman_man_tone1:"),
                                         QStringLiteral(":kiss_woman_man_tone1_tone2:"),
                                         QStringLiteral(":kiss_woman_man_tone1_tone3:"),
                                         QStringLiteral(":kiss_woman_man_tone1_tone4:"),
                                         QStringLiteral(":kiss_woman_man_tone1_tone5:"),
                                         QStringLiteral(":kiss_woman_man_tone2_tone1:"),
                                         QStringLiteral(":kiss_woman_man_tone2:"),
                                         QStringLiteral(":kiss_woman_man_tone2_tone3:"),
                                         QStringLiteral(":kiss_woman_man_tone2_tone4:"),
                                         QStringLiteral(":kiss_woman_man_tone2_tone5:"),
                                         QStringLiteral(":kiss_woman_man_tone3_tone1:"),
                                         QStringLiteral(":kiss_woman_man_tone3_tone2:"),
                                         QStringLiteral(":kiss_woman_man_tone3:"),
                                         QStringLiteral(":kiss_woman_man_tone3_tone4:"),
                                         QStringLiteral(":kiss_woman_man_tone3_tone5:"),
                                         QStringLiteral(":kiss_woman_man_tone4_tone1:"),
                                         QStringLiteral(":kiss_woman_man_tone4_tone2:"),
                                         QStringLiteral(":kiss_woman_man_tone4_tone3:"),
                                         QStringLiteral(":kiss_woman_man_tone4:"),
                                         QStringLiteral(":kiss_woman_man_tone4_tone5:"),
                                         QStringLiteral(":kiss_woman_man_tone5_tone1:"),
                                         QStringLiteral(":kiss_woman_man_tone5_tone2:"),
                                         QStringLiteral(":kiss_woman_man_tone5_tone3:"),
                                         QStringLiteral(":kiss_woman_man_tone5_tone4:"),
                                         QStringLiteral(":kiss_woman_man_tone5:"),
                                         QStringLiteral(":kiss_woman_woman_tone1:"),
                                         QStringLiteral(":kiss_woman_woman_tone1_tone2:"),
                                         QStringLiteral(":kiss_woman_woman_tone1_tone3:"),
                                         QStringLiteral(":kiss_woman_woman_tone1_tone4:"),
                                         QStringLiteral(":kiss_woman_woman_tone1_tone5:"),
                                         QStringLiteral(":kiss_woman_woman_tone2_tone1:"),
                                         QStringLiteral(":kiss_woman_woman_tone2:"),
                                         QStringLiteral(":kiss_woman_woman_tone2_tone3:"),
                                         QStringLiteral(":kiss_woman_woman_tone2_tone4:"),
                                         QStringLiteral(":kiss_woman_woman_tone2_tone5:"),
                                         QStringLiteral(":kiss_woman_woman_tone3_tone1:"),
                                         QStringLiteral(":kiss_woman_woman_tone3_tone2:"),
                                         QStringLiteral(":kiss_woman_woman_tone3:"),
                                         QStringLiteral(":kiss_woman_woman_tone3_tone4:"),
                                         QStringLiteral(":kiss_woman_woman_tone3_tone5:"),
                                         QStringLiteral(":kiss_woman_woman_tone4_tone1:"),
                                         QStringLiteral(":kiss_woman_woman_tone4_tone2:"),
                                         QStringLiteral(":kiss_woman_woman_tone4_tone3:"),
                                         QStringLiteral(":kiss_woman_woman_tone4:"),
                                         QStringLiteral(":kiss_woman_woman_tone4_tone5:"),
                                         QStringLiteral(":kiss_woman_woman_tone5_tone1:"),
                                         QStringLiteral(":kiss_woman_woman_tone5_tone2:"),
                                         QStringLiteral(":kiss_woman_woman_tone5_tone3:"),
                                         QStringLiteral(":kiss_woman_woman_tone5_tone4:"),
                                         QStringLiteral(":kiss_woman_woman_tone5:"),
                                         QStringLiteral(":kiss_man_man_tone1:"),
                                         QStringLiteral(":kiss_man_man_tone1_tone2:"),
                                         QStringLiteral(":kiss_man_man_tone1_tone3:"),
                                         QStringLiteral(":kiss_man_man_tone1_tone4:"),
                                         QStringLiteral(":kiss_man_man_tone1_tone5:"),
                                         QStringLiteral(":kiss_man_man_tone2_tone1:"),
                                         QStringLiteral(":kiss_man_man_tone2:"),
                                         QStringLiteral(":kiss_man_man_tone2_tone3:"),
                                         QStringLiteral(":kiss_man_man_tone2_tone4:"),
                                         QStringLiteral(":kiss_man_man_tone2_tone5:"),
                                         QStringLiteral(":kiss_man_man_tone3_tone1:"),
                                         QStringLiteral(":kiss_man_man_tone3_tone2:"),
                                         QStringLiteral(":kiss_man_man_tone3:"),
                                         QStringLiteral(":kiss_man_man_tone3_tone4:"),
                                         QStringLiteral(":kiss_man_man_tone3_tone5:"),
                                         QStringLiteral(":kiss_man_man_tone4_tone1:"),
                                         QStringLiteral(":kiss_man_man_tone4_tone2:"),
                                         QStringLiteral(":kiss_man_man_tone4_tone3:"),
                                         QStringLiteral(":kiss_man_man_tone4:"),
                                         QStringLiteral(":kiss_man_man_tone4_tone5:"),
                                         QStringLiteral(":kiss_man_man_tone5_tone1:"),
                                         QStringLiteral(":kiss_man_man_tone5_tone2:"),
                                         QStringLiteral(":kiss_man_man_tone5_tone3:"),
                                         QStringLiteral(":kiss_man_man_tone5_tone4:"),
                                         QStringLiteral(":kiss_man_man_tone5:"),
                                         QStringLiteral(":knot:"),
                                         QStringLiteral(":sewing_needle:"),
                                         QStringLiteral(":thong_sandal:"),
                                         QStringLiteral(":military_helmet:"),
                                         QStringLiteral(":polar_bear:"),
                                         QStringLiteral(":worm:"),
                                         QStringLiteral(":lady_beetle:"),
                                         QStringLiteral(":fly:"),
                                         QStringLiteral(":cockroach:"),
                                         QStringLiteral(":seal:"),
                                         QStringLiteral(":mammoth:"),
                                         QStringLiteral(":bison:"),
                                         QStringLiteral(":black_cat:"),
                                         QStringLiteral(":feather:"),
                                         QStringLiteral(":dodo:"),
                                         QStringLiteral(":beaver:"),
                                         QStringLiteral(":empty_nest:"),
                                         QStringLiteral(":nest_with_eggs:"),
                                         QStringLiteral(":coral:"),
                                         QStringLiteral(":rock:"),
                                         QStringLiteral(":wood:"),
                                         QStringLiteral(":potted_plant:"),
                                         QStringLiteral(":lotus:"),
                                         QStringLiteral(":bubbles:"),
                                         QStringLiteral(":blueberries:"),
                                         QStringLiteral(":olive:"),
                                         QStringLiteral(":bell_pepper:"),
                                         QStringLiteral(":flatbread:"),
                                         QStringLiteral(":tamale:"),
                                         QStringLiteral(":fondue:"),
                                         QStringLiteral(":jar:"),
                                         QStringLiteral(":beans:"),
                                         QStringLiteral(":pouring_liquid:"),
                                         QStringLiteral(":teapot:"),
                                         QStringLiteral(":bubble_tea:"),
                                         QStringLiteral(":boomerang:"),
                                         QStringLiteral(":playground_slide:"),
                                         QStringLiteral(":roller_skate:"),
                                         QStringLiteral(":long_drum:"),
                                         QStringLiteral(":accordion:"),
                                         QStringLiteral(":pickup_truck:"),
                                         QStringLiteral(":crutch:"),
                                         QStringLiteral(":wheel:"),
                                         QStringLiteral(":ring_buoy:"),
                                         QStringLiteral(":hook:"),
                                         QStringLiteral(":hut:"),
                                         QStringLiteral(":mobile_phone:"),
                                         QStringLiteral(":low_battery:"),
                                         QStringLiteral(":coin:"),
                                         QStringLiteral(":identification_card:"),
                                         QStringLiteral(":ladder:"),
                                         QStringLiteral(":screwdriver:"),
                                         QStringLiteral(":carpentry_saw:"),
                                         QStringLiteral(":mouse_trap:"),
                                         QStringLiteral(":headstone:"),
                                         QStringLiteral(":hamsa:"),
                                         QStringLiteral(":x_ray:"),
                                         QStringLiteral(":plunger:"),
                                         QStringLiteral(":toothbrush:"),
                                         QStringLiteral(":bucket:"),
                                         QStringLiteral(":nesting_dolls:"),
                                         QStringLiteral(":mirror:"),
                                         QStringLiteral(":window:"),
                                         QStringLiteral(":magic_wand:"),
                                         QStringLiteral(":piñata:"),
                                         QStringLiteral(":mirror_ball:"),
                                         QStringLiteral(":placard:"),
                                         QStringLiteral(":mending_heart:"),
                                         QStringLiteral(":heart_on_fire:"),
                                         QStringLiteral(":elevator:"),
                                         QStringLiteral(":heavy_equals_sign:"),
                                         QStringLiteral(":transgender_symbol:"),
                                         QStringLiteral(":transgender_flag:"),
                                         QStringLiteral(":flag_bq:"),
                                         QStringLiteral(":flag_fk:"),
                                         QStringLiteral(":flag_gf:"),
                                         QStringLiteral(":flag_tf:"),
                                         QStringLiteral(":flag_gp:"),
                                         QStringLiteral(":flag_xk:"),
                                         QStringLiteral(":flag_mq:"),
                                         QStringLiteral(":flag_yt:"),
                                         QStringLiteral(":flag_nc:"),
                                         QStringLiteral(":flag_re:"),
                                         QStringLiteral(":flag_gs:"),
                                         QStringLiteral(":flag_bl:"),
                                         QStringLiteral(":flag_pm:"),
                                         QStringLiteral(":flag_wf:"),
                                         QStringLiteral(":flag_eh:"),
                                         QStringLiteral(":flag_ea:"),
                                         QStringLiteral(":flag_dg:"),
                                         QStringLiteral(":flag_mf:")});
    TextEmoticonsCore::EmojiModelManager::self()->setExcludeEmoticons(lst);
#endif
}

int AccountManager::accountNumber() const
{
    return mRocketChatAccountModel->accountNumber();
}

bool AccountManager::showMessage(const ParseMessageUrlUtils &parseUrl)
{
    auto account = mRocketChatAccountModel->accountFromServerUrl(parseUrl.serverHost());
    if (account) {
        // const QString path{parseUrl.path()};
        const QString messageId = parseUrl.messageId();
        qCDebug(RUQOLA_LOG) << " parseUrl " << parseUrl;
        // https://<server url>/channel/python?msg=sn3gEQom7NcLxTg5h
        setCurrentAccount(account->accountName());
        // qDebug() << " account->accountName() : " << account->accountName();
        Q_EMIT mCurrentAccount->raiseWindow();
        Q_EMIT mCurrentAccount->selectChannelAndMessage(messageId, parseUrl.roomId(), parseUrl.roomIdType(), parseUrl.channelType());
        return true;
    }
    return false;
}

void AccountManager::openMessageUrl(const QString &messageUrl)
{
    ParseMessageUrlUtils parseUrl;
    if (parseUrl.parseUrl(messageUrl)) {
        if (showMessage(parseUrl)) {
            return;
        }
    }

    Q_EMIT messageUrlNotFound(i18n("Server not found: %1", messageUrl));
    // TODO report error
}

void AccountManager::connectToAccount(RocketChatAccount *account)
{
    connect(account, &RocketChatAccount::notification, this, [this, account](const NotificationInfo &info) {
        NotificationHistoryManager::self()->addNotification(info);

        switch (info.notificationType()) {
        case NotificationInfo::StandardMessage: {
            auto job = new NotifierJob;
            job->setInfo(info);
            connect(job, &NotifierJob::switchToAccountAndRoomName, this, &AccountManager::slotSwitchToAccountAndRoomName);
            connect(job, &NotifierJob::sendReply, this, [account](const QString &str, const QString &roomId, const QString &tmId) {
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
        case NotificationInfo::ConferenceCall: {
            break;
        }
        }
    });
    connect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
    connect(account, &RocketChatAccount::roomNeedAttention, this, &AccountManager::roomNeedAttention);
    connect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
}

void AccountManager::slotSwitchToAccountAndRoomName(const QString &accountName, const QString &roomName, const QString &channelType)
{
    setCurrentAccount(accountName);
    QString linkRoom;
    if (channelType == QLatin1Char('c')) {
        linkRoom = QStringLiteral("ruqola:/room/%1").arg(roomName);
    } else {
        linkRoom = QStringLiteral("ruqola:/user/%1").arg(roomName);
    }
    Q_EMIT mCurrentAccount->raiseWindow();
    Q_EMIT mCurrentAccount->openLinkRequested(linkRoom);
}

void AccountManager::loadAccount()
{
    qCDebug(RUQOLA_LOG) << " void AccountManager::loadAccount()" << ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()),
                    QStringList() << QStringLiteral("ruqola.conf"),
                    QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    QVector<RocketChatAccount *> lstAccounts;
    while (it.hasNext()) {
        const QString val = it.next();
        qCDebug(RUQOLA_LOG) << "Account found list.at(i)" << val;
        auto account = new RocketChatAccount(val);
        if (account->settings()->isValid()) {
            connectToAccount(account);
            lstAccounts.append(account);
        } else {
            account->deleteLater();
        }
    }

    // New account => empty list.
    if (lstAccounts.isEmpty()) {
        qCDebug(RUQOLA_LOG) << "Empty list. Create a default rocketchataccount";
        auto account = new RocketChatAccount();
        if (account->accountEnabled()) {
            connectToAccount(account);
        }
        lstAccounts.append(account);
    }

    mRocketChatAccountModel->setAccounts(lstAccounts);

    QSettings settings;
    const QString previousAccount = settings.value(QStringLiteral("currentAccount"), QString()).toString();
    if (previousAccount.isEmpty()) {
        // Use first one
        mCurrentAccount = mRocketChatAccountModel->account(0);
    } else {
        selectAccount(previousAccount);
        if (!mCurrentAccount) {
            // Use first one
            mCurrentAccount = mRocketChatAccountModel->account(0);
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

void AccountManager::addAccount(const AccountManagerInfo &info)
{
    // TODO verify if account exist or not ?
    auto account = new RocketChatAccount();
    account->setAccountName(info.accountName);
    account->setUserName(info.userName);
    account->setServerUrl(info.serverUrl);
    account->setAccountEnabled(info.enabled);
    account->setPassword(info.password);
    if (info.enabled) {
        connectToAccount(account);
    }
    addAccount(account);
}

void AccountManager::modifyAccount(const AccountManagerInfo &info)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(info.accountName);
    if (account) {
        account->setDisplayName(info.displayName);
        account->setUserName(info.userName);
        account->setServerUrl(info.serverUrl);
        account->setAccountEnabled(info.enabled);
        if (!info.enabled) {
            // TODO fixme
            // disconnect(account, &RocketChatAccount::notification, this, &AccountManager::notification);
            disconnect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
            disconnect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
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

void AccountManager::addAccount(RocketChatAccount *account)
{
    mRocketChatAccountModel->insertAccount(account);
}

void AccountManager::selectAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        mCurrentAccount = account;
    } else {
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system.";
    }
}

void AccountManager::setCurrentAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        if (mCurrentAccount != account) {
            QSettings settings;
            settings.setValue(QStringLiteral("currentAccount"), accountName);
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
    const QString directory = LocalDatabaseUtils::localAccountDatabasePath() + accountName;
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
        AccountManager::AccountDisplayInfo info;
        info.name = account->displayName();
        info.icon = Utils::iconFromAccount(account);
        lst.append(std::move(info));
    }
    return lst;
}

#include "moc_accountmanager.cpp"
