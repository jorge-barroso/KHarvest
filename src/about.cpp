// SPDX-License-Identifier: GPL-3.0-only
// PDX-FileCopyrightText: 2023 Jorge Barroso <jorge_barroso_11 at hotmail dot com>

#include "about.h"

KAboutData AboutType::aboutData() const
{
    return KAboutData::applicationData();
}
