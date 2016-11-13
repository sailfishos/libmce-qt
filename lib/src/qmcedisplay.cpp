/*
 * Copyright (C) 2016 Jolla Ltd.
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

#include "qmcedisplay.h"
#include "qmceproxy.h"

#include "mce/mode-names.h"

class QMceDisplay::Private : public QObject {
    Q_OBJECT

public:
    Private(QMceDisplay* aParent);

    void queryDisplayStatus();

private Q_SLOTS:
    void onProxyValidChanged();
    void onQueryFinished(QDBusPendingCallWatcher* aWatcher);
    void updateDisplayStatus(QString aStatus);

public:
    QMceDisplay* iParent;
    QSharedPointer<QMceProxy> iProxy;
    bool iValid;
    QMceDisplay::State iState;
};

QMceDisplay::Private::Private(QMceDisplay* aParent) :
    QObject(aParent),
    iParent(aParent),
    iProxy(QMceProxy::instance()),
    iValid(false),
    iState(DisplayOff)
{
    connect(iProxy->signalProxy(),
        SIGNAL(display_status_ind(QString)),
        SLOT(updateDisplayStatus(QString)));
    connect(iProxy.data(),
        SIGNAL(validChanged()),
        SLOT(onProxyValidChanged()));
    if (iProxy->valid()) {
        queryDisplayStatus();
    }
}

void QMceDisplay::Private::queryDisplayStatus()
{
    connect(new QDBusPendingCallWatcher(
        iProxy->requestProxy()->get_display_status(), this),
        SIGNAL(finished(QDBusPendingCallWatcher*)),
        SLOT(onQueryFinished(QDBusPendingCallWatcher*)));
}

void QMceDisplay::Private::updateDisplayStatus(QString aStatus)
{
    static const QString MCE_DISPLAY_OFF(MCE_DISPLAY_OFF_STRING);
    static const QString MCE_DISPLAY_DIM(MCE_DISPLAY_DIM_STRING);
    static const QString MCE_DISPLAY_ON(MCE_DISPLAY_ON_STRING);
    QMceDisplay::State state;
    if (aStatus == MCE_DISPLAY_OFF) {
        state = DisplayOff;
    } else if (aStatus == MCE_DISPLAY_DIM) {
        state = DisplayDim;
    } else {
        state = DisplayOn;
    }
    if (iState != state) {
        iState = state;
        Q_EMIT iParent->stateChanged();
    }
}

void QMceDisplay::Private::onQueryFinished(QDBusPendingCallWatcher* aWatcher)
{
    QDBusPendingReply<QString> reply(*aWatcher);
    if (!reply.isError()) {
        updateDisplayStatus(reply.value());
        if (!iValid) {
            iValid = true;
            Q_EMIT iParent->validChanged();
        }
    }
    aWatcher->deleteLater();
}

void QMceDisplay::Private::onProxyValidChanged()
{
    if (iProxy->valid()) {
        queryDisplayStatus();
    } else {
        if (iValid) {
            iValid = false;
            Q_EMIT iParent->validChanged();
        }
    }
}

// ==========================================================================
// QMceDisplay
// ==========================================================================

QMceDisplay::QMceDisplay(QObject* aParent) :
    QObject(aParent),
    iPrivate(new Private(this))
{
}

bool QMceDisplay::valid() const
{
    return iPrivate->iValid;
}

QMceDisplay::State QMceDisplay::state() const
{
    return iPrivate->iState;
}

#include "qmcedisplay.moc"
