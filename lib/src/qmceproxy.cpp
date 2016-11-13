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

#include "qmceproxy.h"

#include "mce/dbus-names.h"

class QMceProxy::Private : public QObject {
    Q_OBJECT

public:
    Private(QMceProxy* aParent);

    QMceRequestProxy* requestProxy();
    QMceSignalProxy* signalProxy();

private Q_SLOTS:
    void onServiceRegistered();
    void onServiceUnregistered();

public:
    QMceProxy* iParent;
    bool iValid;

private:
    QDBusConnection iBus;
    QString iService;
    QMceRequestProxy* iRequestProxy;
    QMceSignalProxy* iSignalProxy;
};

QMceProxy::Private::Private(QMceProxy* aParent) :
    QObject(aParent),
    iParent(aParent),
    iBus(QDBusConnection::systemBus()),
    iService(MCE_SERVICE),
    iRequestProxy(NULL),
    iSignalProxy(NULL)
{
    iValid = iBus.interface()->isServiceRegistered(iService);
    QDBusServiceWatcher* watcher = new QDBusServiceWatcher(iService, iBus,
        QDBusServiceWatcher::WatchForRegistration |
        QDBusServiceWatcher::WatchForUnregistration, this);
    connect(watcher,
        SIGNAL(serviceRegistered(QString)),
        SLOT(onServiceRegistered()));
    connect(watcher,
        SIGNAL(serviceUnregistered(QString)),
        SLOT(onServiceUnregistered()));
}

QMceRequestProxy* QMceProxy::Private::requestProxy()
{
    if (!iRequestProxy) {
        iRequestProxy = new QMceRequestProxy(iService,
            MCE_REQUEST_PATH, iBus, this);
    }
    return iRequestProxy;
}

QMceSignalProxy* QMceProxy::Private::signalProxy()
{
    if (!iSignalProxy) {
        iSignalProxy = new QMceSignalProxy(iService,
            MCE_SIGNAL_PATH, iBus, this);
    }
    return iSignalProxy;
}

void QMceProxy::Private::onServiceRegistered()
{
    if (!iValid) {
        iValid = true;
        Q_EMIT iParent->validChanged();
    }
}

void QMceProxy::Private::onServiceUnregistered()
{
    if (iValid) {
        iValid = false;
        Q_EMIT iParent->validChanged();
    }
}

// ==========================================================================
// QMceProxy
// ==========================================================================

QMceProxy::QMceProxy() :
    iPrivate(new Private(this))
{
}

bool QMceProxy::valid() const
{
    return iPrivate->iValid;
}

QMceRequestProxy* QMceProxy::requestProxy()
{
    return iPrivate->requestProxy();
}

QMceSignalProxy* QMceProxy::signalProxy()
{
    return iPrivate->signalProxy();
}

QSharedPointer<QMceProxy> QMceProxy::instance()
{
    static QWeakPointer<QMceProxy> sSharedInstance;
    QSharedPointer<QMceProxy> ref = sSharedInstance;
    if (ref.isNull()) {
        ref = QSharedPointer<QMceProxy>(new QMceProxy, &QObject::deleteLater);
        sSharedInstance = ref;
    }
    return ref;
}

#include "qmceproxy.moc"
