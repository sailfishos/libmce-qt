/*
 * Copyright (C) 2016-2019 Jolla Ltd.
 * Copyright (c) 2019 Open Mobile Platform LLC.
 *
 * You may use this file under the terms of BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of Jolla Ltd nor the names of its contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * any official policies, either expressed or implied.
 */

#include "qmcebatterystate.h"
#include "qmceproxy.h"

#include <mce/mode-names.h>

// ==========================================================================
// QMceBatteryState::Private
// ==========================================================================

class QMceBatteryState::Private : public QObject {
    Q_OBJECT
public:
    Private(QMceBatteryState* aParent);
    bool valid() const;
    QMceBatteryState::State value() const;
private:
    void queryValue();
    void setValid(bool valid);
private Q_SLOTS:
    void onProxyValidChanged();
    void onQueryFinished(QDBusPendingCallWatcher* aWatcher);
    void updateValue(QString state);
private:
    QMceBatteryState* iParent;
    QSharedPointer<QMceProxy> iProxy;
    bool iValid;
    QMceBatteryState::State iValue;
};

QMceBatteryState::Private::Private(QMceBatteryState* aParent) :
    QObject(aParent),
    iParent(aParent),
    iProxy(QMceProxy::instance()),
    iValid(false),
    iValue(Unknown)
{
    connect(iProxy->signalProxy(),
            SIGNAL(battery_state_ind(QString)),
            SLOT(updateValue(QString)));
    connect(iProxy.data(),
            SIGNAL(validChanged()),
            SLOT(onProxyValidChanged()));
    if (iProxy->valid()) {
        queryValue();
    }
}

bool QMceBatteryState::Private::valid() const
{
    return iValid;
}

void QMceBatteryState::Private::setValid(bool valid)
{
    if (iValid != valid) {
        iValid = valid;
        Q_EMIT iParent->validChanged();
    }
}

QMceBatteryState::State QMceBatteryState::Private::value() const
{
    return iValue;
}

void QMceBatteryState::Private::updateValue(QString state)
{
    QMceBatteryState::State value = Unknown;

    if (state == QStringLiteral(MCE_BATTERY_STATE_CHARGING)) {
        value = Charging;
    } else if (state == QStringLiteral(MCE_BATTERY_STATE_DISCHARGING)) {
        value = Discharging;
    } else if (state == QStringLiteral(MCE_BATTERY_STATE_NOT_CHARGING)) {
        value = NotCharging;
    } else if (state == QStringLiteral(MCE_BATTERY_STATE_FULL)) {
        value = Full;
    }

    if (iValue != value) {
        iValue = value;
        Q_EMIT iParent->stateChanged();
    }
    setValid(true);
}

void QMceBatteryState::Private::queryValue()
{
    connect(new QDBusPendingCallWatcher(
        iProxy->requestProxy()->get_battery_state(), this),
        SIGNAL(finished(QDBusPendingCallWatcher*)),
        SLOT(onQueryFinished(QDBusPendingCallWatcher*)));
}

void QMceBatteryState::Private::onQueryFinished(QDBusPendingCallWatcher* aWatcher)
{
    QDBusPendingReply<QString> reply(*aWatcher);
    if (!reply.isError()) {
        updateValue(reply.value());
    }
    aWatcher->deleteLater();
}

void QMceBatteryState::Private::onProxyValidChanged()
{
    if (iProxy->valid()) {
        queryValue();
    } else {
        setValid(false);
    }
}

// ==========================================================================
// QMceBatteryState
// ==========================================================================

QMceBatteryState::QMceBatteryState(QObject* aParent) :
    QObject(aParent),
    iPrivate(new Private(this))
{
}

bool QMceBatteryState::valid() const
{
    return iPrivate->valid();
}

QMceBatteryState::State QMceBatteryState::state() const
{
    return iPrivate->value();
}

#include "qmcebatterystate.moc"
