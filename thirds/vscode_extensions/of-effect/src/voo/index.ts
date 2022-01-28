/* eslint-disable @typescript-eslint/naming-convention */
import * as vscode from 'vscode';
import { Log } from "./lib/log";
import { Util } from "./lib/util";
import { Net, InternalDescriptor } from "./lib/net";

export class API {
    //任务类API
    static VooEnter = new InternalDescriptor("VooEnter", "1.3.0");
    static PrintLog = new InternalDescriptor("PrintLog", "1.3.0");
    //窗口类API
    static ReloadEffect = new InternalDescriptor("ReloadEffect", "1.3.0");
    static ShowMiniPlayer = new InternalDescriptor("ShowMiniPlayer", "1.3.0");
    static HideMiniPlayer = new InternalDescriptor("HideMiniPlayer", "1.3.0");
}

export class EVENT {
    //任务类EVENT
    static InternalStatus = new InternalDescriptor("InternalStatus", "1.3.0");
    static EnterSuccess = new InternalDescriptor("EnterSuccess", "1.3.0");
    static PrintLog = new InternalDescriptor("PrintLog", "1.3.0");
    //窗口类EVENT
    static WindowChanged = new InternalDescriptor("WindowChanged", "1.3.0");
}

export enum InternalStatus {
    VooNotInstalled = 0,
    VooVersionNotSupported = 1
}
export class WindowTypeBody {
    type!: WindowType;
};
export enum WindowType {
    Null = 0,
    Guide = 1,
    Editor = 2
};
export namespace log {
    export function d(msg: string) {
        Log.d(msg);
    }
    export function i(msg: string) {
        Log.i(msg);
    }
    export function w(msg: string) {
        Log.w(msg);
    }
    export function e(msg: string) {
        Log.e(msg);
    }
}
export class util {
    static context: vscode.ExtensionContext;
    static log(msg: any) {
        Util.log(msg);
    }
    static versionEquals(curV: string, reqV: string): number {
        return Util.versionEquals(curV, reqV);
    }
}
export namespace window {
    export function reloadEffect(path: string) {
        Net.send(API.ReloadEffect, path);
    }
    export function showMiniPlayer() {
        Net.send(API.ShowMiniPlayer);
    }
    export function hideMiniPlayer() {
        Net.send(API.HideMiniPlayer);
    }
}
export namespace net {
    export function registerEvent<T>(event: string | EVENT, callback: (data?: T) => void): Disposable {
        let netObject = Net.on(event, callback);
        return {
            dispose: function (): void {
                netObject.off();
            }
        };;
    }
    export function unregisterEvent(event?: string | EVENT, tag?: number) {
        Net.off(event, tag);
    }
    export function sendMessage(api: string | API, message: any) {
        Net.send(api, message);
    }
}
export interface Disposable {
    dispose: () => void
}
export function init(context: vscode.ExtensionContext) {
    util.context = context;
}