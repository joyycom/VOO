/* eslint-disable @typescript-eslint/naming-convention */
import * as childProcess from 'child_process';
import * as WebSocket from 'ws';


export enum API {
    VooEnter = "VooEnter",
    PrintLog = "PrintLog"
}
export enum EVENT {
    InternalStatus = "InternalStatus",
    EnterSuccess = "EnterSuccess"
}
export enum InternalStatus {
    VooNotInstalled = 0,
    VooVersionNotSupported = 1
}
enum LogLevel {
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3
}
export class Log {
    public static d(msg: string) {
        Net.send(API.PrintLog, { level: LogLevel.Debug, msg: msg });
    }
    public static i(msg: string) {
        Net.send(API.PrintLog, { level: LogLevel.Info, msg: msg });
    }
    public static w(msg: string) {
        Net.send(API.PrintLog, { level: LogLevel.Warn, msg: msg });
    }
    public static e(msg: string) {
        Net.send(API.PrintLog, { level: LogLevel.Error, msg: msg });
    }
}
export class Net {
    private event?: string | EVENT;
    private tag?: number;
    public static sendMessage(message: string) {
        NetCore.getInstance().send(message);
    }
    public static send<T>(api: string | API, args?: T) {
        let sendBody = { api: api, args: args };
        NetCore.getInstance().send(sendBody);
    }
    public static on<T>(event: string | EVENT, callback: (data?: T) => void): Net {
        let net = new Net();
        net.event = event;
        net.tag = new Date().getTime();
        NetCore.getInstance().on(event, callback, net.tag);
        return net;
    }
    public static off(event?: string, tag?: number) {
        NetCore.getInstance().off(event, tag);
    }
    public off() {
        NetCore.getInstance().off(this.event, this.tag);
    }
}
export class Util {
    public static log(str: string) {
        console.log(`VOO,${str}`);
    }
    /*
    * JavaScript实现版本号比较
    * 传入两个字符串，当前版本号：curV；比较版本号：reqV
    * 调用方法举例：Version('5.12.3','5.12.2')，将返回true
    * */
    public static versionEquals(curV: string, reqV: string): number {
        var arr1 = curV.split('.');
        var arr2 = reqV.split('.');
        //将两个版本号拆成数字
        var minL = Math.min(arr1.length, arr2.length);
        var pos = 0;        //当前比较位
        var diff = 0;        //当前为位比较是否相等

        //逐个比较如果当前位相等则继续比较下一位
        while (pos < minL) {
            diff = parseInt(arr1[pos]) - parseInt(arr2[pos]);
            if (diff !== 0) {
                break;
            }
            pos++;
        }
        return diff;
    }
}
interface SendBody<T> {
    api: string | API;
    args: T;
}
interface RecvBody<T> {
    event: string | EVENT;
    data: T;
}
class NetCore {
    private static _instance = new NetCore();
    public static getInstance() {
        return this._instance;
    }
    private static readonly longTime: number = 60000;
    private static readonly shortTime: number = 10000;
    private mWaitMessages: Array<any> = [];
    private mListenerList: Map<string | EVENT, Map<number, (data?: any) => void>> = new Map();
    //0是未连接，1是连接成功，2是连接中，3是退出连接
    private mConnectStatue: number = 0;
    private mSocket!: WebSocket;
    private mTimeout: any;
    isConnected(): boolean {
        return this.mConnectStatue === 1;
    }
    send<T>(message: T) {
        if (!this.isConnected()) {
            this.mWaitMessages.push(message);
        }
        this.connect();
        if (this.isConnected() && this.mSocket) {
            let json = JSON.stringify(message);
            this.mSocket.send(json);
        }
    }
    on<T>(event: string | EVENT, callback: (data?: T) => void, tag?: number) {
        this.connect();
        if (!tag) {
            tag = new Date().getTime();
        }
        if (this.mListenerList.has(event)) {
            this.mListenerList.get(event)?.set(tag, callback);
        } else {
            let map: Map<number, (data?: T) => void> = new Map();
            map.set(tag, callback);
            this.mListenerList.set(event, map);
        }
    }
    off(event?: string | EVENT, tag?: number) {
        if (event) {
            if (tag) {
                this.mListenerList.get(event)?.delete(tag);
            } else {
                this.mListenerList.delete(event);
            }
        } else {
            this.mListenerList.clear();
        }
    }
    private connect(): void {
        if (this.mConnectStatue === 0) {
            this.mConnectStatue = 2;
            this.retry(0);
        }
    }
    private retry(time: number): void {
        if (this.mTimeout) {
            clearInterval(this.mTimeout);
        }
        this.mTimeout = setInterval(() => {
            if (this.mConnectStatue === 3) {
                clearInterval(this.mTimeout);
                return;
            }
            if (this.isConnected()) {
                if (time !== NetCore.longTime) {
                    clearInterval(this.mTimeout);
                    this.retry(NetCore.longTime);
                }
                return;
            }
            Util.log(`retryConnectVoo`);
            this.findPid();
            if (time !== NetCore.shortTime) {
                clearInterval(this.mTimeout);
                this.retry(NetCore.shortTime);
            }
        }, time);
    }
    private runVOO() {
        childProcess.exec('taskkill /F /IM VOO.exe');
        const cmd = process.platform === 'win32' ? `reg query HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\VOO` : ``;
        childProcess.exec(cmd, (err, stdout, stderr) => {
            if (err) {
                Util.log("VOO未安装");
                this.callback(EVENT.InternalStatus, InternalStatus.VooNotInstalled);
                this.mConnectStatue = 3;
                return;
            }
            var processPath: string = "";
            stdout.split('\n').filter((line) => {
                const p = line.trim().split('    ');
                if (p.length < 3) {
                    return;
                }
                if (p[0] === "InstallLocation") {
                    processPath = p[2] + "\\VOO.exe";
                } else if (p[0] === "DisplayVersion") {
                    if (Util.versionEquals(p[2], "1.3.0") < 0) {
                        Util.log("该VOO版本不支持");
                        this.callback(EVENT.InternalStatus, InternalStatus.VooVersionNotSupported);
                        this.mConnectStatue = 3;
                    }
                }
            });
            if (this.mConnectStatue !== 3 && processPath !== "") {
                childProcess.exec(processPath);
                Util.log(`run:${processPath}`);
            }
        });
    }
    private findPid() {
        let processName = "VOO.exe";
        const cmd = process.platform === 'win32' ? `tasklist -V|findstr "${processName}" ` : `ps aux | grep ${processName}`;
        Util.log(`findPid:${cmd}`);
        childProcess.exec(cmd, (err, stdout, stderr) => {
            if (err) {
                this.runVOO();
                return;
            }
            var hasPort = false;
            stdout.split('\n').filter((line) => {
                const p = line.trim().split(/\s+/);
                // p[0] 应用程序名称  p[1] 应用程序PID
                if (p.length > 1 && p[1]) {
                    hasPort = true;
                    this.findPort(p[1]);
                }
            });
            Util.log(`checkPort:${hasPort}`);
            if (!hasPort) {
                this.runVOO();
            }
        });
    }
    private findPort(pid: string) {
        const cmd = process.platform === 'win32' ? `netstat -ano | findstr "${pid}" ` : ``;
        Util.log(`findPort:${cmd}`);
        childProcess.exec(cmd, (err, stdout, stderr) => {
            if (err) {
                return;
            }
            stdout.split('\n').filter((line) => {
                const p = line.trim().split(/\s+/);
                if (p[0] && p[1].startsWith("[::]:")) {
                    const p2 = p[1].split('[::]:');
                    if (p2.length > 1 && p2[1]) {
                        this.socketConnect(p2[1]);
                    }
                }
            });
        });
    }
    private socketConnect(port: string) {
        this.mSocket = new WebSocket("ws://localhost:" + port);
        this.mSocket.onmessage = (event: WebSocket.MessageEvent) => {
            Util.log("socket,onmessage:" + event.data);
            try {
                let recvBody: RecvBody<any> = JSON.parse(event.data.toString());
                if (recvBody.event === EVENT.EnterSuccess) {
                    this.mConnectStatue = 1;
                    if (this.mWaitMessages.length > 0) {
                        this.mWaitMessages.forEach((value) => {
                            this.send(value);
                        });
                        this.mWaitMessages.splice(0, this.mWaitMessages.length);
                    }
                }
                this.callback(recvBody.event, recvBody.data);
            } catch (error) {
                //Util.log("socket,onmessage,error:" + error);
            }
        };
        this.mSocket.onopen = (event: WebSocket.Event) => {
            Util.log("socket,onopen");
            let sendBody = { api: API.VooEnter };
            let json = JSON.stringify(sendBody);
            this.mSocket.send(json);
        };
        this.mSocket.onclose = (event: WebSocket.CloseEvent) => {
            Util.log("socket,onclose");
            this.mConnectStatue = 0;
            this.connect();
        };
        this.mSocket.onerror = (event: WebSocket.ErrorEvent) => {
            Util.log("socket,onerror");
            this.mConnectStatue = 0;
            this.connect();
        };
    }
    private callback(event: string | EVENT, data?: any) {
        if (this.mListenerList.has(event)) {
            this.mListenerList.get(event)?.forEach((value) => {
                value(data);
            });
        }
    }
}