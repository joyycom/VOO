import * as vscode from 'vscode';
import * as voo from '../';

export class Util {
    private static logChannel: vscode.OutputChannel;
    public static log(str: any) {
        if (voo.util.context.extensionMode === 2) {
            console.log(str);
        } else {
            if (!Util.logChannel && voo.util.context) {
                Util.logChannel = vscode.window.createOutputChannel(voo.util.context.extension.packageJSON.name);
                Util.logChannel.clear();
            }
            if (Util.logChannel) {
                Util.logChannel.appendLine(str);
            }
        }
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