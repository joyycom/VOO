import * as childProcess from 'child_process';
import * as fs from 'fs';
import * as WebSocket from 'ws';
import * as voo from '../';
import { Util } from "./util";

export class InternalDescriptor {
    name: string;
    version: string;
    constructor(name: string, version: string) {
        this.name = name;
        this.version = version;
    }
}

export class Net {
    private event?: string;
    private tag?: number;
    public static sendMessage(message: string) {
        NetCore.getInstance().send(message);
    }
    public static send<T>(api: string | voo.API, args?: T) {
        let _api: string;
        if (api instanceof InternalDescriptor) {
            _api = api.name;
            let vooInfo = NetCore.getInstance().getVooInfo();
            if (!vooInfo || Util.versionEquals(vooInfo.version, api.version) < 0) {
                Util.log(`该VOO版本不支持 ${_api} API`);
                return new Net();
            }
        } else {
            _api = api.toString();
        }
        let sendBody = { api: _api, args: args };
        NetCore.getInstance().send(sendBody);
    }
    public static on<T>(event: string | voo.EVENT, callback: (data?: T) => void): Net {
        let _event: string;
        if (event instanceof InternalDescriptor) {
            _event = event.name;
            let vooInfo = NetCore.getInstance().getVooInfo();
            if (!vooInfo || Util.versionEquals(vooInfo.version, event.version) < 0) {
                Util.log(`该VOO版本不支持 ${_event} 事件`);
                return new Net();
            }
        } else {
            _event = event.toString();
        }
        let net = new Net();
        net.event = _event;
        net.tag = new Date().getTime();
        NetCore.getInstance().on(_event, callback, net.tag);
        return net;
    }
    public static off(event?: string | voo.EVENT, tag?: number) {
        if (event) {
            let _event: string;
            if (event instanceof InternalDescriptor) {
                _event = event.name;
            } else {
                _event = event.toString();
            }
            NetCore.getInstance().off(_event, tag);
        } else {
            NetCore.getInstance().off(event, tag);
        }
    }
    public off() {
        NetCore.getInstance().off(this.event, this.tag);
    }
}

interface SendBody<T> {
    api: string;
    args: T;
}
interface RecvBody<T> {
    event: string;
    data: T;
}

class VooInfo {
    processPath: string;
    version: string;
    constructor(processPath: string, version: string) {
        this.processPath = processPath;
        this.version = version;
    }
}

class NetCore {
    private static _instance = new NetCore();
    public static getInstance() {
        return this._instance;
    }
    private static readonly longTime: number = 60000;
    private static readonly shortTime: number = 10000;
    private static readonly autoRunVOO: boolean = false;
    private mInfoRequested: boolean = false;
    private mVooInfo?: VooInfo;
    private mWaitMessages: Array<any> = [];
    private mListenerList: Map<string, Map<number, (data?: any) => void>> = new Map();
    //0是未连接，1是连接成功，2是连接中，3是退出连接
    private mConnectStatue: number = 0;
    private mSocket!: WebSocket;
    private mTimeout: any;
    isConnected(): boolean {
        return this.mConnectStatue === 1;
    }
    getVooInfo(): VooInfo | undefined {
        if (!this.mInfoRequested) {
            this.mInfoRequested = true;
            var processPath: string = "";
            var version: string = "";
            try {
                if (process.platform === 'win32') {
                    const cmd = `reg query HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\VOO`;
                    let stdout = childProcess.execSync(cmd).toString();
                    stdout.split('\n').filter((line) => {
                        const p = line.trim().split('    ');
                        if (p.length < 3) {
                            return;
                        }
                        if (p[0] === "InstallLocation") {
                            processPath = p[2] + "\\VOO.exe";
                        } else if (p[0] === "DisplayVersion") {
                            version = p[2];
                        }
                    });
                } else {
                    let path = "/Applications/VOO.app";
                    if (fs.existsSync(path)) {
                        processPath = path;
                        const cmd = `/usr/libexec/PlistBuddy -c "Print CFBundleShortVersionString" /Applications/VOO.app/Contents/Info.plist`;
                        let stdout = childProcess.execSync(cmd).toString().trim();
                        if (Util.versionEquals(stdout, "1.0.0") >= 0) {
                            version = stdout;
                        }
                    }
                }
            }
            catch (error) {
            }
            if (processPath !== "" && version !== "") {
                this.mVooInfo = new VooInfo(processPath, version);
            }
        }
        return this.mVooInfo;
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
    on<T>(event: string, callback: (data?: T) => void, tag?: number) {
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
    off(event?: string, tag?: number) {
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
    connect(): void {
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
        childProcess.exec('taskkill /F /IM VOO.exe', (err, stdout, stderr) => {
            let vooInfo = this.getVooInfo();
            if (vooInfo) {
                childProcess.exec(vooInfo.processPath);
                Util.log(`run:${vooInfo.processPath}`);
            }
        });
    }
    private findPid() {
        let vooInfo = this.getVooInfo();
        if (!vooInfo || Util.versionEquals(vooInfo.version, "1.3.0") < 0) {
            Util.log("该VOO版本不支持");
            this.callback(voo.EVENT.InternalStatus, voo.InternalStatus.VooVersionNotSupported);
            this.mConnectStatue = 3;
            return;
        }
        const cmd = process.platform === 'win32' ? `${voo.util.context.extensionPath}\\bin\\qprocess.exe | findstr voo.exe` : `lsof -t -c VOO`;
        Util.log(`findPid:${cmd}`);
        childProcess.exec(cmd, (err, stdout, stderr) => {
            if (err) {
                if (NetCore.autoRunVOO) {
                    this.runVOO();
                } else {
                    Util.log(`VOO没启动，请自行打开`);
                }
                return;
            }
            var hasPort = false;
            if (process.platform === 'win32') {
                stdout.split('\n').filter((line) => {
                    const p = line.trim().split(/\s+/);
                    if (p.length > 3 && p[3]) {
                        hasPort = true;
                        this.findPort(p[3]);
                    }
                });
            } else {
                hasPort = true;
                this.findPort(stdout.trim());
            }
            Util.log(`checkPort:${hasPort}`);
            if (!hasPort) {
                if (NetCore.autoRunVOO) {
                    this.runVOO();
                } else {
                    Util.log(`VOO没启动，请自行打开`);
                }
            }
        });
    }
    private findPort(pid: string) {
        const cmd = process.platform === 'win32' ? `netstat -ano | findstr ${pid}` : `netstat -anvp tcp | grep ${pid}`;
        Util.log(`findPort:${cmd}`);
        childProcess.exec(cmd, (err, stdout, stderr) => {
            if (err) {
                return;
            }
            stdout.split('\n').filter((line) => {
                const p = line.trim().split(/\s+/);
                if (process.platform === 'win32') {
                    if (p[0] && p[1].startsWith("[::]:")) {
                        const p2 = p[1].split('[::]:');
                        if (p2.length > 1 && p2[1]) {
                            this.socketConnect(p2[1]);
                        }
                    }
                } else {
                    if (p[3] && p[3].startsWith("*.")) {
                        this.socketConnect(p[3].replace("*.", ""));
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
                if (recvBody.event === voo.EVENT.EnterSuccess.name) {
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
            Util.log("socket,onopen,port:" + port);
            let sendBody = { api: voo.API.VooEnter.name };
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
    private callback(event: string | voo.EVENT, data?: any) {
        let _event: string;
        if (event instanceof InternalDescriptor) {
            _event = event.name;
        } else {
            _event = event.toString();
        }
        if (this.mListenerList.has(_event)) {
            this.mListenerList.get(_event)?.forEach((value) => {
                value(data);
            });
        }
    }
}