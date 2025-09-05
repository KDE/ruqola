#SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
#SPDX-License-Identifier: MIT

FROM opensuse/tumbleweed
MAINTAINER openSUSE KDE Maintainers <opensuse-kde-bugs@opensuse.org>

# Add Snap repo
ARG OBS_REPO=system:snappy
RUN zypper --non-interactive addrepo --priority 50 --refresh obs://${OBS_REPO}/openSUSE_Tumbleweed ${OBS_REPO}
# Add KDE:Qt6 repo
ARG OBS_REPO=KDE:Qt6
RUN zypper --non-interactive addrepo --priority 50 --refresh obs://${OBS_REPO}/openSUSE_Tumbleweed ${OBS_REPO}
# Update container, import GPG key for KDE:Qt repo
RUN zypper --non-interactive --gpg-auto-import-keys -v dup
# CUPS-devel at least does not like Busybox versions of some things, so ensure it is not used
# Likewise, we have packages that do not want LibreSSL so lock it out too
RUN zypper al busybox busybox-gzip libressl-devel
# Install various other packages
RUN zypper --non-interactive install java-1_8_0-openjdk-headless python3-lxml python3-paramiko python3-PyYAML python3-simplejson wget file tar gzip go rsync
# ffmpegthumbs, needs to be before devel_qt or the ffmpeg-6-mini-libs package will be installed with no codec at all
RUN zypper --non-interactive install ffmpeg-6-libavcodec-devel ffmpeg-6-libavfilter-devel ffmpeg-6-libavformat-devel ffmpeg-6-libavdevice-devel ffmpeg-6-libavutil-devel ffmpeg-6-libswscale-devel ffmpeg-6-libpostproc-devel

# Install build dependencies
RUN zypper --non-interactive install --recommends -t pattern devel_C_C++
# The pattern is likely not enough, so just install all Qt devel packages from KDE:Qt6
RUN zypper -q se --not-installed-only --repo ${OBS_REPO} qt6*devel | tail -n +4 | cut -d "|" -f 2 | xargs zypper --non-interactive in

# Install packages for qdoc index files, needed for building documentation that references Qt types
# zypper in Docker excludes documentation by default, but we really want it here
RUN sed -i s/"rpm.install.excludedocs = yes"/"rpm.install.excludedocs = no"/ /etc/zypp/zypp.conf
RUN zypper --non-interactive install qt6-*-docs-html
RUN sed -i s/"rpm.install.excludedocs = no"/"rpm.install.excludedocs = yes"/ /etc/zypp/zypp.conf

# And some other useful and base packages
RUN zypper --non-interactive in \
    # SCM utils
    git git-lfs \
    # Additional compilers used for various bits of integration (clazy, kdevelop)
    # ccache is used to speed up builds as well for large projects
    clang ccache \
    # Additional linker
    mold \
    # Additional build systems (non-cmake)
    ninja meson \
    # Pip: needed to install various Python modules
    python3-pip \
    # Python bindings for accessibility automation
    python3-atspi \
    # Sphinx documentation tooling for ECM docs
    python3-Sphinx \
    # Ruby for various places
    ruby-devel libffi-devel \
    # Utilities to bring up a headless X server instance
    xvfb-run openbox \
    # Utilities to validate Appstream metadata
    AppStream \
    # basic Qt6 packages, which have no -devel and should be manually installed
    qt6-charts-imports \
    qt6-declarative-tools \
    qt6-tools-qdbus \
    qt6-qt5compat-imports \
    qt6-networkinformation-nm \
    qt6-multimedia-imports \
    qt6-location \
    qt6-positioning-imports \
    qt6-translations \
    # Other basic Qt based libraries
    qca-qt6-devel qt6-mqtt-devel qt6-charts-devel qt6-chartsqml-devel \
    # For building documentation tarballs
    bzip2 \
    # For image thumbnails for the KDE.org/applications subsite
    ImageMagick \
    # Useful tools for static analysis
    clazy cppcheck codespell \
    # Needed for API Documentation generation
    python3-gv graphviz-gd qt6-tools-helpgenerators doxygen \
    # Needed for some unit tests to function correctly
    hicolor-icon-theme \
    # Needed for some projects that use the non-standard catch2 Unittest mechanisms.
    Catch2-devel \
    # Needed by KDE Connect on X11
    libfakekey-devel \
    # Python bindings
    python3-build \
    python3-pyside6 \
    python3-pyside6-devel \
    python3-shiboken6 \
    python3-shiboken6-devel
# Use mold as the default linker, as it is magnitudes faster than ld.bfd
RUN /usr/sbin/update-alternatives --set ld /usr/bin/ld.mold
# Install components needed for the CI tooling to operate (python-gitlab, gcovr, cppcheck_codequality) as well as other CI jobs (check-jsonschema)
# as well as reuse (for unit tests), doxyqml (for building QML documentation) and cheroot/wsgidav/ftpd (for KIO unit tests)
# We also bring in chai and pygdbmi which is used by DrKonqi unit tests
RUN pip install --break-system-packages python-gitlab gcovr cppcheck_codequality reuse doxyqml cheroot wsgidav check-jsonschema chai pygdbmi \
    yamllint==1.33.0
RUN gem install ftpd
# KDE stuff also depends on the following
RUN zypper --non-interactive in --allow-vendor-change \
    # kdesrc-build
    perl-JSON perl-YAML-LibYAML perl-IO-Socket-SSL perl-JSON-XS \
    # modemmanager-qt
    ModemManager-devel \
    # networkmanager-qt
    NetworkManager-devel \
    # kcoreaddons
    lsof \
    # kauth
    polkit-devel \
    # kwindowsystem
    xcb-*-devel \
    # karchive
    libzstd-devel \
    # prison
    libdmtx-devel qrencode-devel \
    # kimageformats
    openexr-devel libavif-devel libheif-devel libraw-devel jxrlib-devel libjxl-devel \
    # kwayland and kwin
    wayland-devel \
    wayland-protocols-devel \
    libdisplay-info-devel \
    libei-devel \
    libxcvt-devel \
    # baloo/kfilemetadata (some for okular)
    libattr-devel libexiv2-devel libtag-devel libtag-devel libepub-devel libpoppler-qt6-devel lmdb-devel \
    # kdoctools
    perl-URI docbook_4 docbook-xsl-stylesheets libxml2-devel libxslt-devel perl-URI \
    # kio
    libacl-devel libmount-devel libblkid-devel liburing-devel \
    # various projects need OpenSSL
    libopenssl-devel \
    # kdnssd
    libavahi-devel libavahi-glib-devel libavahi-gobject-devel \
    # khelpcenter and pim
    libxapian-devel \
    # sonnet
    aspell \
    aspell-devel \
    hunspell-devel \
    libvoikko-devel \
    # kio-extras and krdc, kio-fuse
    libssh-devel fuse3-devel libseccomp-devel djvulibre ms-gsl-devel libtirpc-devel libproxy-devel \
    # plasma-pa
    libpulse-devel libcanberra-devel pipewire-pulseaudio \
    # user-manager
    libpwquality-devel \
    # sddm-kcm
    libXcursor-devel \
    # plasma-workspace
    libappindicator3-devel \
    libXtst-devel \
    umockdev-devel \
    xdotool \
    # breeze-plymouth
    plymouth-devel \
    # kde-gtk-config/breeze-gtk
    gsettings-desktop-schemas gtk4-devel gtk3-devel gtk2-devel python3-cairo sassc \
    # plasma-desktop/discover
    itstool \
    appstream-qt6-devel \
    PackageKit PackageKit-devel \
    packagekitqt6-devel \
    fwupd-devel \
    libsnapd-qt-2-1-devel \
    # plasma-desktop
    xf86-input-synaptics-devel xf86-input-evdev-devel xf86-input-libinput-devel libxkbfile-devel libxkbregistry-devel xorg-x11-server-sdk xdg-user-dirs shared-mime-info \
    # kimpanel
    ibus-devel scim-devel \
    # libksane
    sane-backends-devel \
    # pim
    libical-devel libxerces-c-devel \
    # <misc>
    alsa-devel fftw3-devel adobe-sourcecodepro-fonts \
    # choqok
    qtkeychain-qt6-devel \
    # krita
    eigen3-devel OpenColorIO-devel dejavu-fonts gnu-free-fonts libraqm-devel libunibreak-devel \
    quazip-qt6-devel \
    # kaccounts
    libaccounts-qt6-devel \
    libaccounts-glib-devel \
    libsignon-qt6-devel \
    intltool \
    # skrooge
    sqlcipher sqlcipher-devel sqlite3-devel sqlite3 libofx-devel poppler-tools \
    # kwin
    libepoxy-devel Mesa-demo Mesa-demo-x xorg-x11-server-extra dmz-icon-theme-cursors libgbm-devel weston \
    xorg-x11-server-wayland \
    # kgamma5
    libXxf86vm-devel \
    # kgraphviewer
    graphviz-devel \
    # drkonqi
    at-spi2-core which libgirepository-1_0-1 typelib-1_0-Atspi-2_0 gobject-introspection-devel \
    # kcalc
    mpfr-devel \
    mpc-devel \
    # kdevelop
    gdb \
    libstdc++6-pp \
    # labplot
    gsl-devel liblz4-devel libcerf-devel hdf5-devel netcdf-devel libmatio-devel liborcus-devel libixion-devel readstat-devel \
    # kalzium
    # avogadrolibs-devel \ TODO pulls in Qt5
    openbabel-devel \
    ocaml-facile-devel \
    # kuserfeedback
    php8 \
    # digikam
    # QtAV-devel \ TODO not available for Qt6 yet / pulls in Qt5
    opencv-devel exiftool \
    # wacomtablet
    libwacom-devel \
    xf86-input-wacom-devel \
    # rust-qt-binding-generator
    rust rust-std \
    cargo \
    # kdevelop
    clang \
    clang-devel \
    llvm-devel \
    subversion-devel \
    python3-devel \
    # clang-format job with different clang major versions
    clang15 \
    clang17 \
    clang18 \
    clang19 \
    # clazy
    clang-devel-static \
    # libkleo
    libqgpgmeqt6-devel \
    # akonadi
    mariadb qt6-sql-mysql \
    # libkdegames
    openal-soft-devel \
    libsndfile-devel \
    # kscd
    # libmusicbrainz5-devel \ package no longer available
    # audiocd-kio
    cdparanoia-devel \
    # ark
    libarchive-devel libzip-tools libzip-devel \
    # k3b
    flac-devel \
    libmad-devel \
    libmp3lame-devel \
    libogg-devel libvorbis-devel \
    libsamplerate-devel \
    # kamera
    libgphoto2-devel \
    # signon-kwallet-extension
    signond-libs-devel \
    # kdenlive
    libmlt-devel \
    libmlt7-data \
    libmlt7-module-qt6 \
    melt \
    rttr-devel \
    # print-manager
    cups-devel \
    system-config-printer-dbus-service \
    # krfb
    LibVNCServer-devel \
    # kscd
    libdiscid-devel \
    # minuet
    fluidsynth-devel \
    # kajongg
    python3-Twisted \
    # okular
    texlive-latex libdjvulibre-devel libmarkdown-devel chmlib-devel \
    # ksmtp tests
    cyrus-sasl-plain \
    # kdb
    libmariadb-devel postgresql-devel \
    # Gwenview
    cfitsio-devel \
    # Calligra, Krita and probably other things elsewhere too
    libboost_*-devel \
    # Amarok
    gmock gtest libcurl-devel libofa-devel libgpod-devel libmtp-devel loudmouth-devel \
    libmariadbd-devel \
    # liblastfm-qt5-devel TODO not available for qt6 yet
    # Cantor
    libspectre-devel \
    python3-numpy \
    python3-matplotlib \
    octave \
    maxima \
    libqalculate-devel \
    # julia-devel \ ### package no longer provided by OpenSUSE
    # KPat
    freecell-solver-devel black-hole-solver-devel \
    # RKWard
    R-base-devel gcc-fortran \
    # Kaffeine
    libdvbv5-devel \
    vlc-devel \
    libXss-devel \
    # Keysmith
    libsodium-devel \
    # Plasma Phone Components
    libphonenumber-devel \
    # kquickcharts
    glslang-devel \
    # xdg-desktop-portal-kde
    pipewire pipewire-devel \
    # Spectacle
    kImageAnnotator-Qt6-devel kColorPicker-Qt6-devel \
    # upnp-lib-qt
    kdsoap-qt6-devel \
    # KSysGuard
    libnl3-devel \
    # Kjournald
    systemd-devel systemd-journal-remote \
    # Smb4k
    libsmbclient-devel \
    # ksystemstats
    libsensors4-devel \
    # kitinerary, qrca
    zxing-cpp-devel \
    # ki18n
    iso-codes-devel \
    iso-codes-lang \
    # Neochat
    qcoro-qt6-devel \
    libQuotient-qt6-devel \
    cmark cmark-devel \
    # KWave
    audiofile-devel id3lib-devel \
    # elf-dissector
    libdwarf-devel \
    # trojita
    libmimetic-devel \
    # plasma-pass
    liboath-devel \
    # Krita
    Vc-devel libmypaint-devel libheif-devel openjpeg2-devel \
    # Skanpage
    tesseract-ocr-devel leptonica-devel \
    # kup
    libgit2-devel \
    # plasma-nm
    mobile-broadband-provider-info \
    # Spacebar
    c-ares-devel \
    # kxstitch
    ImageMagick-devel \
    libMagick++-devel \
    # plasma-dialer (kde-telephony-daemon)
    callaudiod-devel \
    # discover, flatpak-kcm
    flatpak-devel \
    # kmymoney
    aqbanking-devel \
    # xdg-portal-test-kde
    gstreamer-devel \
    gstreamermm-devel \
    # Haruna
    mpv-devel \
    # kscreenlocker
    libpamtest-devel \
    # NeoChat
    olm-devel \
    # Sink/kube
    flatbuffers-devel \
    # Marble
    libshp-devel \
    # KRdc
    freerdp-devel winpr-devel freerdp2-devel winpr2-devel \
    # Glaxnimate
    potrace-devel \
    # selenium-webdriver-at-spi
    python3-opencv3 \
    # kinfocenter Appium test using selenium-webdriver-at-spi
    wayland-utils \
    # lightdm-kde-greeter
    lightdm-gobject-devel \
    # kstars
    wcslib-devel libXISF-devel libnova-devel erfa-devel indi-devel cfitsio-devel stellarsolver-devel \
    # Kaidan
    kdsingleapplication-qt6-devel \
    libomemo-c-devel \
    libQXmppQt6-devel \
    # marknote
    md4c-devel \
    # kinfocenter
    pciutils-devel \
    # karp (KDE-arranger-for-PDFs)
    qpdf-devel \
    # konvex
    assimp-devel \
    # plasma-camera
    libcamera-devel \
    # audiotube
    python3-pybind11-devel

#Download KDE Frameworks and Applications repos and install
RUN curl -o /etc/zypp/repos.d/KDE_Frameworks.repo \
	https://download.opensuse.org/repositories/KDE:/Frameworks/openSUSE_Tumbleweed/KDE:Frameworks.repo
RUN curl -o /etc/zypp/repos.d/KDE_Applications.repo \
	https://download.opensuse.org/repositories/KDE:/Applications/KDE_Frameworks5_openSUSE_Tumbleweed/KDE:Applications.repo

# Refresh repositories to get the latest package list
RUN zypper --non-interactive refresh

# Install required KDE Frameworks 6 development libraries
RUN zypper --non-interactive --no-gpg-checks install extra-cmake-modules
RUN zypper --non-interactive --no-gpg-checks install kf6-kcoreaddons-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-ki18n-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kcrash-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-knotifications-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kwidgetsaddons-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-ktextwidgets-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kiconthemes-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-syntax-highlighting-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-knotifyconfig-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kio-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kiconthemes-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kitemviews-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kidletime-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-prison-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-karchive-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kcodecs-devel
RUN zypper --non-interactive --no-gpg-checks install ktextaddons-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kdbusaddons-devel
RUN zypper --non-interactive --no-gpg-checks install kdbusaddons-devel
RUN zypper --non-interactive --no-gpg-checks install kdbusaddons-tools
RUN zypper --non-interactive --no-gpg-checks install kf6-purpose-devel
RUN zypper --non-interactive --no-gpg-checks install kf6-kstatusnotifieritem-devel

#GUI with X server
RUN zypper --non-interactive --no-gpg-checks install xclock

# create symlinks, because clang-format-15.0.7 is an awfully prescise executable name
RUN for version in /usr/bin/clang-format-[0-9]*.*; do ln -s "$version" "/usr/bin/$(basename "$version" | cut -d. -f1)"; done

# For D-Bus to be willing to start it needs a Machine ID
RUN dbus-uuidgen > /etc/machine-id
# Certain X11 based software is very particular about permissions and ownership around /tmp/.X11-unix/ so ensure this is right
RUN mkdir /tmp/.X11-unix/ && chown root:root /tmp/.X11-unix/ && chmod 1777 /tmp/.X11-unix/
# We need a user account to do things as, and we need specific group memberships to be able to access video/render DRM nodes
RUN groupadd -g 44 host-video && groupadd -g 109 host-render && useradd -d /home/user/ -u 1000 --user-group --create-home -G video,host-video,host-render --shell /usr/bin/bash user

# Switch to our unprivileged user account
USER user
