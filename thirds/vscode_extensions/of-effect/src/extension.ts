// The module 'vscode' contains the VS Code extensibility API
// Import the module and reference it with the alias vscode in your code below
import * as vscode from 'vscode';
import * as voo from './voo';
import * as path from 'path';
import { LogBody } from './voo/lib/log';

/**
 * 检测Lua扩展sumneko.lua是否大于1.18.0以上，否则提示更新
 */
async function checkLuaExtension(context: vscode.ExtensionContext) {
	var lua = vscode.extensions.getExtension("sumneko.lua");
	if (lua) {
		var version = lua.packageJSON.version;
		if (!version) {
			version = "0";
		}
		if (voo.util.versionEquals(version, "1.18.0") < 0) {
			vscode.window.showInformationMessage("OF语法提示需要Lua扩展，是否更新？", "是", "否").then(result => {
				if (result === "是") {
					vscode.commands.executeCommand('extension.open', 'sumneko.lua');
					vscode.commands.executeCommand('workbench.extensions.installExtension', 'sumneko.lua');
				}
			});
		}
	} else {
		vscode.window.showInformationMessage("OF语法提示需要Lua扩展，是否安装？", "是", "否").then(result => {
			if (result === "是") {
				vscode.commands.executeCommand('extension.open', 'sumneko.lua');
				vscode.commands.executeCommand('workbench.extensions.installExtension', 'sumneko.lua');
			}
		});
	}
}

/**
 * 复制Lua配置到工作空间
 */
async function copySettingsJson(context: vscode.ExtensionContext) {
	let libraryPath = `${context.extensionPath}${path.sep}3rdparty`;
	let config = vscode.workspace.getConfiguration("Lua");
	let oldValue = config.get<Array<string>>("workspace.library");
	if (oldValue) {
		let newValue = Array<string>();
		for (let value of oldValue) {
			if (value === libraryPath) {
				return;
			}
			if (!value.startsWith(context.extensionPath.substring(0, context.extensionPath.lastIndexOf("-")))) {
				newValue.push(value);
			}
		}
		newValue.push(libraryPath);
		config.update("workspace.library", newValue, vscode.ConfigurationTarget.Global);
	} else {
		config.update("workspace.library", [libraryPath], vscode.ConfigurationTarget.Global);
	}
}

export function activate(context: vscode.ExtensionContext) {
	voo.init(context);
	voo.util.log("扩展启动" + context.extensionMode);
	checkLuaExtension(context);
	copySettingsJson(context);
	let logChannel: vscode.OutputChannel;
	voo.net.unregisterEvent();
	voo.net.registerEvent<number>(voo.EVENT.InternalStatus, (status) => {
		voo.util.log("内部状态:" + status);
	});
	voo.net.registerEvent(voo.EVENT.EnterSuccess, () => {
		voo.util.log("连接VOO成功");
	});
	voo.net.registerEvent<LogBody>(voo.EVENT.PrintLog, (log) => {
		if (log?.msg) {
			if (!logChannel) {
				logChannel = vscode.window.createOutputChannel("VOO");
				logChannel.show();
				logChannel.clear();
			}
			logChannel.appendLine(log?.msg);
		}
	});
	voo.net.registerEvent<voo.WindowTypeBody>(voo.EVENT.WindowChanged, (windowType) => {
		if (windowType?.type === voo.WindowType.Editor) {
			voo.window.showMiniPlayer();
		}
	});
	let saveDisposable = vscode.workspace.onDidSaveTextDocument(changeEvent => {
		let workspaceFolders = vscode.workspace.workspaceFolders?.map(item => item.uri.fsPath);
		workspaceFolders?.forEach(folder => {
			if (changeEvent.fileName.startsWith(folder + path.sep)) {
				voo.window.reloadEffect(folder);
			}
		});
	});
	let disposable = vscode.commands.registerCommand('voo.of-effect.reloadEffect', () => {
		let workspaceFolders = vscode.workspace.workspaceFolders?.map(item => item.uri.fsPath);
		workspaceFolders?.forEach(folder => {
			voo.window.reloadEffect(folder);
		});
	});
	context.subscriptions.push(saveDisposable);
	context.subscriptions.push(disposable);
	voo.window.showMiniPlayer();
}

export function deactivate() {
	voo.window.hideMiniPlayer();
}