; CapStep 截图工具 - Inno Setup 安装脚本
; 使用前请先运行 deploy.bat 生成发布文件
; 然后使用 Inno Setup 编译此脚本生成安装程序
; 
; 命令行编译示例（可指定版本号）：
; iscc /DMyAppVersion=1.0.1 installer.iss

#define MyAppName "CapStep截图工具"
#ifndef MyAppVersion
  #define MyAppVersion "1.0.0"
#endif
#define MyAppPublisher "CapStep Team"
#define MyAppURL "https://gitee.com/yourusername/CapStep"
#define MyAppExeName "CapStep.exe"
#define DistDir "..\dist\CapStep-" + MyAppVersion

[Setup]
; 应用信息
AppId={{8B5C9F4E-3D2A-4A5B-9C8D-1E2F3A4B5C6D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}

; 安装目录
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes

; 输出设置
OutputDir=..\dist
OutputBaseFilename=CapStep-{#MyAppVersion}-Setup
Compression=lzma2/ultra64
SolidCompression=yes

; 界面设置
WizardStyle=modern
SetupIconFile=..\resources\icons\app.ico
UninstallDisplayIcon={app}\{#MyAppExeName}

; 权限
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

; 语言
ShowLanguageDialog=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
; Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"  ; Inno Setup 6 不包含此文件

[Tasks]
Name: "desktopicon"; Description: "创建桌面快捷方式"; GroupDescription: "附加选项:"
Name: "quicklaunchicon"; Description: "创建快速启动栏快捷方式"; GroupDescription: "附加选项:"; Flags: unchecked
Name: "autostart"; Description: "开机自动启动"; GroupDescription: "附加选项:"; Flags: unchecked

[Files]
; 复制所有文件
Source: "{#DistDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; 开始菜单快捷方式
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 0
Name: "{group}\卸载 {#MyAppName}"; Filename: "{uninstallexe}"

; 桌面快捷方式
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 0; Tasks: desktopicon

; 快速启动栏快捷方式
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 0; Tasks: quicklaunchicon

[Registry]
; 开机自启动（添加到注册表 Run 键）
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "{#MyAppName}"; ValueData: """{app}\{#MyAppExeName}"""; Tasks: autostart; Flags: uninsdeletevalue

[Run]
; 安装完成后选项
Filename: "{app}\{#MyAppExeName}"; Description: "立即运行 {#MyAppName}"; Flags: nowait postinstall skipifsilent

[Code]
// 关闭运行中的进程
function KillProcess(const ProcessName: String): Boolean;
var
  ResultCode: Integer;
begin
  // 使用 taskkill 关闭进程
  Exec('taskkill.exe', '/F /IM ' + ProcessName, '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
  Result := (ResultCode = 0) or (ResultCode = 128); // 0=成功，128=进程不存在
end;

// 安装前初始化
function InitializeSetup(): Boolean;
var
  MsgResult: Integer;
  ProcessClosed: Boolean;
begin
  Result := True;
  
  // 检查进程是否运行
  if CheckForMutexes('CapStep.lock') then
  begin
    MsgResult := MsgBox('检测到 CapStep 正在运行。' + #13#10 + 
                        '需要先关闭程序才能继续安装。' + #13#10#13#10 + 
                        '点击"是"自动关闭程序并继续安装。' + #13#10 + 
                        '点击"否"取消安装。', 
                        mbConfirmation, MB_YESNO);
    
    if MsgResult = IDYES then
    begin
      // 尝试关闭进程
      ProcessClosed := KillProcess('CapStep.exe');
      
      if ProcessClosed then
      begin
        // 等待进程完全退出
        Sleep(2000);
        Result := True;
      end
      else
      begin
        MsgBox('无法关闭运行中的程序，请手动关闭后重试。', mbError, MB_OK);
        Result := False;
      end;
    end
    else
    begin
      // 用户选择取消
      Result := False;
    end;
  end;
end;

// 卸载前清理
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then
  begin
    // 卸载前关闭进程
    KillProcess('CapStep.exe');
    Sleep(1000);
  end;
  
  if CurUninstallStep = usPostUninstall then
  begin
    // 删除锁文件
    DelTree(ExpandConstant('{tmp}\CapStep.lock'), False, True, False);
  end;
end;

