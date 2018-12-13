/*
 * Copyright (C) 2018 Jolla Ltd.
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

import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Mce 1.0

ApplicationWindow {
    initialPage: Component {
        Page {
            Column {
                width: parent.width
                spacing: Theme.paddingLarge
                Label {
                    text: "CableState"
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Valid: " + mceCableState.valid
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Connected: " + mceCableState.connected
                }
                Label {
                    text: "ChargerState"
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Valid: " + mceChargerState.valid
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Charging: " + mceChargerState.charging
                }
                Label {
                    text: "BatteryLevel"
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Valid: " + mceBatteryLevel.valid
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Percent: " + mceBatteryLevel.percent
                }
                Label {
                    text: "PowerSaveMode"
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Valid: " + mcePowerSaveMode.valid
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Active: " + mcePowerSaveMode.active
                }
                Label {
                    text: "BatteryStatus"
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Valid: " + mceBatteryStatus.valid
                }
                Label {
                    x: Theme.paddingLarge
                    text: "Status: " + mceBatteryStatus.status + "/" + mceBatteryStatus.name
                }
            }
            MceCableState {
                id: mceCableState
            }
            MceChargerState {
                id: mceChargerState
            }
            MceBatteryLevel {
                id: mceBatteryLevel
            }
            McePowerSaveMode {
                id: mcePowerSaveMode
            }
            MceBatteryStatus {
                id: mceBatteryStatus
                property string name: {
                    if (!valid) return "INVALID";
                    if (status == MceBatteryStatus.Full) { return "FULL"; }
                    if (status == MceBatteryStatus.Ok)   { return "OK"; }
                    if (status == MceBatteryStatus.Low)  { return "LOW"; }
                    if (status == MceBatteryStatus.Empty){ return "EMPTY"; }
                    return "UNKNOWN"
                }
            }
        }
    }
}
