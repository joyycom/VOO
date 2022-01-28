/* eslint-disable @typescript-eslint/naming-convention */

import { API } from "../";
import { Net } from "./net";

enum LogLevel {
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3
}
export class LogBody {
    level: LogLevel;
    msg: string;
    constructor(level: LogLevel, msg: string) {
        this.level = level;
        this.msg = msg;
    }
}
export class Log {
    public static d(msg: string) {
        Net.send(API.PrintLog, new LogBody(LogLevel.Debug, msg));
    }
    public static i(msg: string) {
        Net.send(API.PrintLog, new LogBody(LogLevel.Info, msg));
    }
    public static w(msg: string) {
        Net.send(API.PrintLog, new LogBody(LogLevel.Warn, msg));
    }
    public static e(msg: string) {
        Net.send(API.PrintLog, new LogBody(LogLevel.Error, msg));
    }
}