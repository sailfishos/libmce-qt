/*
 * Copyright (C) 2019 Jolla Ltd.
 * Copyright (c) 2019 Open Mobile Platform LLC.
 * Contact: Slava Monich <slava.monich@jolla.com>
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

#include "qmcechargertype.h"
#include "qmceproxy.h"

#include <mce/mode-names.h>

// ==========================================================================
// QMceChargerType::Private
// ==========================================================================

class QMceChargerType::Private : public QObject
{
    Q_OBJECT
public:
    Private(QMceChargerType *aParent);

    bool valid() const;
    QMceChargerType::Type value() const;

private:
    void queryValue();
    void setValid(bool valid);

private Q_SLOTS:
    void onNameOwnerChanged();
    void onQueryFinished(QDBusPendingCallWatcher *aWatcher);
    void updateValue(QString type);

private:
    QMceChargerType* iParent;
    QSharedPointer<QMceProxy> iProxy;
    bool iValid;
    QMceChargerType::Type iValue;
};

QMceChargerType::Private::Private(QMceChargerType *aParent)
    : QObject(aParent)
    , iParent(aParent)
    , iProxy(QMceProxy::instance())
    , iValid(false)
    , iValue(None)
{
    QObject::connect(iProxy->signalProxy(),
                     &QMceSignalProxy::charger_type_ind,
                     this,
                     &QMceChargerType::Private::updateValue);
    QObject::connect(iProxy.data(),
                     &QMceProxy::nameOwnerChanged,
                     this,
                     &QMceChargerType::Private::onNameOwnerChanged);
    onNameOwnerChanged();
}

bool QMceChargerType::Private::valid() const
{
    return iValid;
}

void QMceChargerType::Private::setValid(bool valid)
{
    if (iValid != valid) {
        iValid = valid;
        Q_EMIT iParent->validChanged();
    }
}

QMceChargerType::Type QMceChargerType::Private::value() const
{
    return iValue;
}

void QMceChargerType::Private::updateValue(QString type)
{
    bool valid = true;
    QMceChargerType::Type value = Other;

    if (type == QStringLiteral(MCE_CHARGER_TYPE_NONE))
        value = None;
    else if (type == QStringLiteral(MCE_CHARGER_TYPE_USB))
        value = USB;
    else if (type == QStringLiteral(MCE_CHARGER_TYPE_DCP))
        value = DCP;
    else if (type == QStringLiteral(MCE_CHARGER_TYPE_HVDCP))
        value = HVDCP;
    else if (type == QStringLiteral(MCE_CHARGER_TYPE_CDP))
        value = CDP;
    else if (type == QStringLiteral(MCE_CHARGER_TYPE_WIRELESS))
        value = Wireless;
    else if (type == QStringLiteral(MCE_CHARGER_TYPE_OTHER))
        value = Other;
    else
        valid = false;

    if (valid) {
        if (iValue != value) {
            iValue = value;
            Q_EMIT iParent->typeChanged();
        }
    }
    setValid(valid);
}

void QMceChargerType::Private::queryValue()
{
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(
        iProxy->requestProxy()->get_charger_type(), this);
    QObject::connect(watcher,
                     &QDBusPendingCallWatcher::finished,
                     this,
                     &QMceChargerType::Private::onQueryFinished);
}

void QMceChargerType::Private::onQueryFinished(QDBusPendingCallWatcher* aWatcher)
{
    QDBusPendingReply<QString> reply(*aWatcher);
    if (!reply.isError()) {
        updateValue(reply.value());
    }
    aWatcher->deleteLater();
}

void QMceChargerType::Private::onNameOwnerChanged()
{
    if (iProxy->hasNameOwner()) {
        queryValue();
    } else {
        setValid(false);
    }
}

// ==========================================================================
// QMceChargerType
// ==========================================================================

QMceChargerType::QMceChargerType(QObject *aParent)
    : QObject(aParent)
    , iPrivate(new Private(this))
{
}

bool QMceChargerType::valid() const
{
    return iPrivate->valid();
}

QMceChargerType::Type QMceChargerType::type() const
{
    return iPrivate->value();
}

#include "qmcechargertype.moc"
