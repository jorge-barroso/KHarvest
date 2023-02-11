[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/U6U6ECJZ8)
[![liberapay](https://img.shields.io/liberapay/receives/jorge-barroso.svg?logo=liberapay)](https://liberapay.com/jorge-barroso/donate)
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://paypal.me/jorgebaam)

# kharvest

A native [KDE](https://kde.org//) application written with [Kirigami](https://develop.kde.org/frameworks/kirigami//)
that integrates with [Harvest](https://www.getharvest.com) to automate time tracking.

## First Run

You will need to log in the first time you use the application.

On the first run, the app will open the Harvest login page in a browser for you to log in and provide access to the app.
Once you have logged in and see the message "Authentication successful, you may now close this tab" (depending on your
system language) the app will load your data and open up ready to use

## App Screenshots

### Main Window

![Main Window](/assets/main_window.png?raw=true "the main window of the application")

### Adding a Task

![Adding a Task](/assets/add_task_window.png?raw=true "the form to add a new task from the applications")

### Favourites

![Favourites Window](/assets/favourites_window.png?raw=true "the application with a separate dialog to quickly choose one of your saved tasks for quick access")

### System Tray Icon

[//]: # (![System Tray]&#40;/assets/systray_icon.png?raw=true "the app giving you quick access "&#41;)
~Coming Soon~

## Installation

You can build the app by yourself,
or simply grab the prebuilt binary in the [releases section](https://github.com/jorge-barroso/kharvest/releases).

Additionally, in Arch-based distros you can find a package in
the [AUR repository](https://aur.archlinux.org/packages/kharvest).

### Binary File

If you are happy with just grabbing the binary file,
you can do so from the latest release in the [releases section](https://github.com/jorge-barroso/kharvest/releases),
download the binary pack and use it on your system as you prefer.

### DEB and RPM Files

You can install both the deb and rpm packages attached on
every [release](https://github.com/jorge-barroso/kharvest/releases).

### Arch Linux (AUR-enabled distros)

You can use your favourite installation method (yay, software centre, etc.) to
install [harvest-timer-qt](https://aur.archlinux.org/packages/kharvest)

### Dependencies

The app has very few dependencies, but you need to bear these in mind especially if you are going to build it from
sources:

#### Build Dependencies

- Modern C++ compiler ([g++](https://gcc.gnu.org/), [clang](http://clang.org/))
- [CMake](https://cmake.org/)
- Qt5 Tools, the package name of which may vary:
    -
    Ubuntu: [qt5-tools-dev](https://packages.ubuntu.com/search?keywords=qt5-tools-dev&searchon=names&suite=all&section=all)
    - Fedora: [qt5-qttools](https://src.fedoraproject.org/rpms/qt5-qttools)
    - Arch: [qt5-tools](https://archlinux.org/packages/extra/x86_64/qt5-tools/)

#### Runtime Dependencies

##### Required

- The **Qt5 Base** package, some namings for this, depending on distros, are
    -
    Ubuntu: [qtbase5-dev](https://packages.ubuntu.com/search?keywords=qtbase5-dev&searchon=names&suite=all&section=all)
    - Fedora: [qt5-qtbase](https://src.fedoraproject.org/rpms/qt5-qtbase)
    - Arch: [qt5-base](https://archlinux.org/packages/extra/x86_64/qt5-base/)
- Similarly, you will need the **Qt5 SVG** package to display icons:
    -
    Ubuntu: [libqt5svg5](https://packages.ubuntu.com/search?suite=all&section=all&arch=any&keywords=libqt5svg5&searchon=names)
    - Fedora: [qt5-qtsvg](https://src.fedoraproject.org/rpms/qt5-qtsvg)
    - Arch: [qt5-svg](https://archlinux.org/packages/extra/x86_64/qt5-svg)
- A base installation of **KDE Frameworks 5** is required

##### Optional

- If you are running Harvest in a wayland system, installing the **Qt5 Wayland** package to allow Qt to interact
  directly with the compositor:
    -
    Ubuntu: [qtwayland5](https://packages.ubuntu.com/search?suite=all&section=all&arch=any&keywords=qtwayland5&searchon=names)
    - Fedora: [qt5-qtwayland](https://src.fedoraproject.org/rpms/qt5-qtwayland)
    - Arch: [qt5-wayland](https://archlinux.org/packages/extra/x86_64/qt5-wayland)

## Troubleshoot

### Missing Data

If you have logged in and the list of projects/tasks is empty, login to the web portal and verify that you have projects
and tasks assigned to you.

If that fails, you can try logging out and logging in again. You can do either of the following to logout:

1. Log out from the application menu
2. Re-running the login process by clearing the app's data from your keychain