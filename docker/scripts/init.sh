#SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
#SPDX-License-Identifier: MIT

#!/bin/bash

# Init Refresh and Install Required Base Tools
sudo zypper --non-interactive refresh
sudo zypper --non-interactive install -y grep nano

# Function to add a repository if missing
add_repo_if_missing() {
	repo_url=$1
	repo_alias=$2

	if sudo zypper lr | grep -q "$repo_alias"; then
		echo "✅ Repository '$repo_alias' already exists. Skipping..."
	else
		echo "➕ Adding repository '$repo_alias'..."
		sudo zypper ar -f "$repo_url" "$repo_alias"
	fi
}

add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Qt6/openSUSE_Tumbleweed/" "KDE-Qt6"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Frameworks/openSUSE_Tumbleweed/" "KDE-Frameworks"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Frameworks5/openSUSE_Factory/" "KDE-Frameworks5"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Applications/KDE_Frameworks5_openSUSE_Tumbleweed/" "KDE-Applications"
add_repo_if_missing "https://download.opensuse.org/repositories/KDE:/Extra/openSUSE_Tumbleweed/" "KDE-Extra"
add_repo_if_missing "https://download.opensuse.org/repositories/M17N:/fonts/openSUSE_Tumbleweed/" "M17N-Fonts"
add_repo_if_missing "http://dl.google.com/linux/chrome/rpm/stable/x86_64" "google-chrome"
add_repo_if_missing "https://download.opensuse.org/tumbleweed/repo/oss/" "openSUSE-OSS"
add_repo_if_missing "https://download.opensuse.org/repositories/system:/snappy/openSUSE_Tumbleweed" "system:snappy"

echo "🔄 Refreshing repositories..."
sudo zypper --non-interactive refresh

if sudo zypper list-updates | grep -q "up"; then
	echo "🚀 System updates are available. Performing upgrade..."
	sudo zypper -v dup --allow-vendor-change
else
	echo "✅ No system updates available. Skipping upgrade..."
fi

sudo zypper --non-interactive refresh

echo "📦 Installing  Core build tools..."
sudo zypper --non-interactive install --auto-agree-with-licenses \
	cmake \
	make \
	ninja \
	meson \
	extra-cmake-modules \
	clang \
	ccache \
	gdb \
	gcc-c++

echo "📦 Installing Qt6 base and modules..."
sudo zypper --non-interactive install --auto-agree-with-licenses \
	qt6-base-devel \
	qt6-core-devel \
	qt6-charts-devel \
	qt6-chartsqml-devel \
	qt6-charts-imports \
	qt6-declarative-tools \
	qt6-tools-qdbus \
	qt6-qt5compat-imports \
	qt6-networkinformation-nm \
	qt6-multimedia-imports \
	qt6-multimedia-devel \
	qt6-multimediawidgets-devel \
	qt6-location \
	qt6-positioning-imports \
	qt6-translations \
	qt6-websockets-devel \
	qt6-networkauth-devel \
	qt6-sql-devel \
	qt6-sql-mysql \
	qt6-sql-postgresql \
	qt6-sql-sqlite \
	libQt6Sql6 \
	qtkeychain-qt6-devel \
	qtkeychain-qt6-lang \
	qtkeychain-qt6-debugsource \
	qt6-wayland-devel \
	qt6-wayland \
	qca-qt6-devel

echo "📦 Installing KDE Frameworks 6 development libraries..."
sudo zypper --non-interactive install --auto-agree-with-licenses \
	kf6-kcoreaddons-devel \
	kf6-ki18n-devel \
	kf6-kcrash-devel \
	kf6-knotifications-devel \
	kf6-kwidgetsaddons-devel \
	kf6-ktextwidgets-devel \
	kf6-kiconthemes-devel \
	kf6-syntax-highlighting-devel \
	kf6-knotifyconfig-devel \
	kf6-kio-devel \
	kf6-kitemviews-devel \
	kf6-kidletime-devel \
	kf6-prison-devel \
	kf6-karchive-devel \
	kf6-kcodecs-devel \
	kf6-kdbusaddons-devel \
	kf6-purpose-devel \
	kf6-kstatusnotifieritem-devel \
	kf6-kuserfeedback-devel \
	kf6-kuserfeedback \
	kf6-kdoctools-devel \
	kf6-networkmanager-qt-devel \
	libKF6UserFeedbackCore6 \
	libKF6UserFeedbackWidgets6 \
	ktextaddons-devel \
	kdbusaddons-devel \
	kdbusaddons-tools

echo "📦 Installing Optional / GUI / helper packages..."
sudo zypper --non-interactive install --auto-agree-with-licenses \
	xclock

sudo zypper --non-interactive install libxcb1 libxcb-render0 libxcb-shm0 libxcb-xfixes0 libxcb-shape0 libxcb-glx0 libxcb-icccm4 libxcb-composite0 libxcb-cursor0 libxcb-damage0 libxcb-dbe0 libxcb-devel libxcb-dpms0 libxcb-dri2-0 libxcb-dri3-0 libxcb-res0 libxcb-screensaver0 libxcb-sync1 libxcb-xf86dri0 libxcb-xinerama0 libxcb-xinput0 libxcb-xkb1 libxcb-xtest0 libxcb-xv0 libxcb-xvmc0

sudo zypper --non-interactive install fontconfig fontconfig-devel fonts-KanjiStrokeOrders libkfont0

echo "🔗 Creating symlinks for clang-format versions..."
for version in $(ls /usr/bin/clang-format-[0-9]*.* 2>/dev/null); do
	link_name="/usr/bin/$(basename "$version" | cut -d. -f1)"
	if [ ! -e "$link_name" ]; then
		echo "Creating symlink: $link_name -> $version"
		sudo ln -s "$version" "$link_name"
	fi
done

echo "🆔 Generating D-Bus machine ID..."
sudo dbus-uuidgen >/etc/machine-id

echo "🔧 Fixing X11 permissions..."
sudo mkdir -p /tmp/.X11-unix/
sudo chown root:root /tmp/.X11-unix/
sudo chmod 1777 /tmp/.X11-unix/

# Define the variables
echo 'export LC_ALL=C.UTF-8' >>$HOME/.bashrc
echo 'export XDG_RUNTIME_DIR=/tmp/runtime-$USER' >>$HOME/.bashrc
echo 'mkdir -p $XDG_RUNTIME_DIR' >>$HOME/.bashrc
echo 'chmod 0700 $XDG_RUNTIME_DIR' >>$HOME/.bashrc
echo 'export $(dbus-launch)' >>$HOME/.bashrc
echo 'export QT_QPA_PLATFORM=xcb' >>$HOME/.bashrc

source $HOME/.bashrc

echo "Init setup complete! Happy Hacking! :)"
