/*
 * Copyright (c) 2019 - 2022 Jolla Ltd.
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

import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Mce 1.0

ApplicationWindow {
    id: root
    initialPage: Component {
        Page {
            Column {
                width: parent.width
                spacing: Theme.paddingLarge
                Label {
                    text: "MCE properties"
                }
                DetailItem {
                    label: "BatteryLevel"
                    value: mceBatteryLevel.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "BatteryState"
                    value: mceBatteryState.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "BatteryStatus"
                    value: mceBatteryStatus.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "CableState"
                    value: mceCableState.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "CallState"
                    value: mceCallState.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "ChargerState"
                    value: mceChargerState.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "ChargerType"
                    value: mceChargerType.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "ChargingState"
                    value: mceChargingState.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "Display"
                    value: mceDisplay.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "NameOwner"
                    value: mceNameOwner.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "PowerSaveMode"
                    value: mcePowerSaveMode.text
                    onValueChanged: console.log("####", label, value)
                }
                DetailItem {
                    label: "TkLock"
                    value: mceTkLock.text
                    onValueChanged: console.log("####", label, value)
                }
            }
        }
    }
    function validRepr(valid) {
      return valid ? "" : "[invalid]"
    }
    MceBatteryLevel {
        id: mceBatteryLevel
        readonly property string validRepr: root.validRepr(valid)
        readonly property string text: validRepr + percent + "%"
    }
    MceBatteryState {
        id: mceBatteryState
        readonly property string validRepr: root.validRepr(valid)
        readonly property string valueRepr: {
            if (value === MceBatteryState.Unknown)
                return "Unknown"
            if (value === MceBatteryState.Charging)
                return "Charging"
            if (value === MceBatteryState.Discharging)
                return "Discharging"
            if (value === MceBatteryState.NotCharging)
                return "NotCharging"
            if (value === MceBatteryState.Full)
                return "Full"
            return "???";
        }
        readonly property string text: validRepr + valueRepr
    }
    MceBatteryStatus {
        id: mceBatteryStatus
        readonly property string validRepr: root.validRepr(valid)
        readonly property string statusRepr: {
            if (status === MceBatteryStatus.Empty)
                return "Empty"
            if (status === MceBatteryStatus.Low)
                return "Low"
            if (status === MceBatteryStatus.Ok)
                return "Ok"
            if (status === MceBatteryStatus.Full)
                return "Full"
            return "???";
        }
        readonly property string text: validRepr + statusRepr
    }
    MceCableState {
        id: mceCableState
        readonly property string validRepr: root.validRepr(valid)
        readonly property string connectedRepr: {
          return connected ? "Connected" : "Disconnected"
        }
        readonly property string text: validRepr + connectedRepr
    }
    MceCallState {
        id: mceCallState
        readonly property string validRepr: root.validRepr(valid)
        readonly property string stateRepr: {
            if (state === MceCallState.None)
                return "None"
            if (state === MceCallState.Ringing)
                return "Ringing"
            if (state === MceCallState.Active)
                return "Active"
            if (state === MceCallState.Service)
                return "Service"
            return "???"
        }
        readonly property string typeRepr: {
            if (type === MceCallState.Normal)
                return "Normal"
            if (type === MceCallState.Emergency)
                return "Emergency"
            return "???"
        }
        readonly property string text: validRepr + stateRepr + ":" + typeRepr
    }
    MceChargerState {
        id: mceChargerState
        readonly property string validRepr: root.validRepr(valid)
        readonly property string chargingRepr: {
          return charging ? "Charging" : "NotCharging"
        }
        readonly property string text: validRepr + chargingRepr
    }
    MceChargerType {
        readonly property string typeRepr: {
            if (type === MceChargerType.None)
                return "None"
            if (type === MceChargerType.USB)
                return "USB"
            if (type === MceChargerType.DCP)
                return "DCP"
            if (type === MceChargerType.HVDCP)
                return "HVDCP"
            if (type === MceChargerType.CDP)
                return "CDP"
            if (type === MceChargerType.Wireless)
                return "Wireless"
            if (type === MceChargerType.Other)
                return "Other"
            return "???"
        }
        id: mceChargerType
        readonly property string validRepr: root.validRepr(valid)
        readonly property string text: validRepr + typeRepr
    }
    MceChargingState {
        id: mceChargingState
        readonly property string validRepr: root.validRepr(valid)
        readonly property string valueRepr: {
            if (value === MceChargingState.Enabled)
                return "Enabled"
            if (value === MceChargingState.Disabled)
                return "Disabled"
            if (value === MceChargingState.Unknown)
                return "Unknown"
            return "???"
        }
        readonly property string text: validRepr + valueRepr
    }
    MceDisplay {
        id: mceDisplay
        readonly property string validRepr: root.validRepr(valid)
        readonly property string stateRepr: {
            if (state === MceDisplay.DisplayOff)
                return "DisplayOff"
            if (state === MceDisplay.DisplayDim)
                return "DisplayDim"
            if (state === MceDisplay.DisplayOn)
                return "DisplayOn"
            return "???"
        }
        readonly property string text: validRepr + stateRepr
    }
    MceNameOwner {
        id: mceNameOwner
        readonly property string validRepr: root.validRepr(valid)
        readonly property string text: validRepr + nameOwner
    }
    McePowerSaveMode {
        id: mcePowerSaveMode
        readonly property string validRepr: root.validRepr(valid)
        readonly property string activeRepr: active ? "Active" : "Inactive"
        readonly property string text: validRepr + activeRepr
    }
    MceTkLock {
        id: mceTkLock
        readonly property string validRepr: root.validRepr(valid)
        readonly property string lockedRepr: locked ? "Locked" : "Unlocked"
        readonly property string text: validRepr + lockedRepr
    }
}
