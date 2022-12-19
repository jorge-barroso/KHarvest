// SPDX-License-Identifier: GPL-2.0-or-later
// PDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

#include "about.h"

KAboutData AboutType::aboutData() const
{
    return KAboutData::applicationData();
}
