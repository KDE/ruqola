# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
macro(add_textedit_texttospeech_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h TEST_NAME ${_name}
        NAME_PREFIX "texttospeech-"
        LINK_LIBRARIES Qt::Test KF5::TextEditTextToSpeech Qt::Widgets
        )
endmacro()

add_textedit_texttospeech_unittest( texttospeechwidgettest.cpp )
add_textedit_texttospeech_unittest( texttospeechconfigwidgettest.cpp )
add_textedit_texttospeech_unittest( texttospeechactionstest.cpp )
add_textedit_texttospeech_unittest( texttospeechsliderwidgettest.cpp )
add_textedit_texttospeech_unittest( texttospeechutiltest.cpp )
