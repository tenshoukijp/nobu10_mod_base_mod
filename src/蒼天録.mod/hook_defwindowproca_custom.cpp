#include <windows.h>
#include "output_debug_stream.h"
#include "game_window.h"

using namespace std;

/*
WM_ACTIVATE                 ウインドウがアクティブ・非アクティブになるとき
WM_ACTIVATEAPP              別のアプリケーションのウインドウがアクティブ・非アクティブになるとき
WM_ASKCBFORMATNAME          クリップボードのフォーマット名を文字列バッファにコピーを要求
WM_CANCELMODEJOURNAL        ジャーナル処理がキャンセルされた
WM_CANCELMODE               メッセージボックス等のダイアログボックスが表示されようとしている
WM_CAPTURECHAINGED          マウスのキャプチャが変化した
WM_CHANGECBCHAIN            クリップボ－ドビューワチェーンが変更される
WM_CHAR                     キーボードから文字入力を受け取った
WM_CHARTOITEM               WM_CHARに応じてオーナー描画リストボックスが受け取る
WM_CHILDACTIVATE            MDIコントロールの子ウインドウがアクティブ化
WM_CHOOSEFONT_GETLOGFONT    [フォントの指定]ダイアログボックスに送るメッセージ
WM_CLEAR                    テキストボックス・コンボボクスの選択テキストを削除
WM_CLOSE                    ウインドウあるいはアプリケーションをクローズされた
WM_COMMAND                  メニューが選択されたあるいはコントロールにイベントが発生した
WM_COMPAREITEM              並び替えが指定された新しい項目のリストボックスなどで相対的な位置を決定する。
WM_COMPACTING               システムがメモリ不足の状態にある
WM_COPY                     テキストボックス・コンボボックスの選択テキストをクリップボードにコピーしたとき
WM_COPYDATA                 別のプロセス上のウィンドウにデータを送信する
WM_CREATE                   ウインドウが作成中
WM_CTLCOLORBTN              コマンドボタンが描画されようとしている
WM_CTLCOLORDLG              ダイアログが描画されようとしている
WM_CTLCOLOREDIT             テキストボックスが描画されようとしている
WM_CTLCOLORLISTBOX          リストボックスが描画されようとしている
WM_CTLCOLORMSGBOX           メッセージボックスが描画されようとしている
WM_CTLCOLORSCROLLBAR        スクロールバーが描画されようとしている
WM_CTLCOLORSTATIC           ラベルが描画されようとしている
WM_CUT                      テキストボックス・コンボボックスの選択テキストをクリップボードにコピー
WM_DEADCHAR                 アクセント記号のような文字と組合せた文字を受け取った
WM_DELETEITEM               オーナ描画リストボックスの項目が削除された
WM_DESTROY                  ウインドウが破棄された
WM_DESTROYCLIPBOARD         EmptyClipboard()によってクリップボードが空になった
WM_DEVICECHANGE             ハードウェアの構成が変更された
WM_DEVMODECHANGE            WIN.INI のデフォルトのデバイス名が変更された
WM_DISPLAYCHANGE            ディスプレイの解像度が変更された
WM_DRAWCLIPBOARD            クリップボードの内容が変わった
WM_DRAWITEM                 オーナー描画コントロールの外観が変わった
WM_DROPFILES                ファイルマネージャからの Drag & Drop
WM_ENABLE                   ウインドウが有効・無効になった
WM_ENDSESSION               Windows が終了する
WM_ENTERIDLE                ダイアログボックス・メニューが入力待ち
WM_ENTERMENULOOP            モーダルなメニューループに入ったとき
WM_ENTERSIZEMOVE            ウィンドウが移動・サイズ変更した
WM_ERASEBKGND               バックグラウンドを消去(再ペイント)
WM_EXITMENULOOP             モーダルなメニューから抜け出した
WM_EXITSIZEMOVE             ウィンドウの移動・サイズ変更の処理が終了したとき
WM_FONTCHANGE               システムフォントが変更された
WM_GETDLGCODE               コントロールに関連付けられたウインドウに送られるメッセージ
WM_GETFONT                  テキストボックスラベル等が現在使っているフォントのハンドルを設定・取得する
WM_GETHOTKEY                ウィンドウに関連付けられているホットキーを取得する
WM_GETICON                  ウインドウに関連付けられているアイコンのハンドルを取得
WM_GETMINMAXINFO            フォームの最大・最小化時に可能なサイズを取得
WM_GETTEXT                  コントロールのキャプション・テキストボックスのテキストをバッファにコピー
WM_GETTEXTLENGTH            ウィンドウに関連付けられているテキストのサイズ(バイト数)を取得
WM_HELP                     [F1] or [?]の押された場合
WM_HOTKEY                   RegisterHotKey関数で登録されたホットキーが押されたとき
WM_HSCROLL                  水平スクロールバーを調整している
WM_HSCROLLCLIPBOARD         クリップボードビューワで水平スルクールバーが使われた
WM_ICONERASEBKGND           クラスアイコンがある時、アイコンの描画前に通知
WM_IME_CHAR                 IMEが変換後の文字を受取った
WM_IME_COMPOSITION          キー入力によってIMEが確定後の状態を変更した
WM_IME_COMPOSITIONFULL      確定後の文字を表示するウインドウにスペースがない
WM_IME_CONTROL              IMEにコマンドを送る
WM_IME_ENDCOMPOSITION       文字を確定した
WM_IME_KEYDOWN              キーを押した
WM_IME_KEYUP                キーを離した
WM_IME_NOTIFY               IMEウインドウの変化をアプリケーションに知らせる
WM_IME_REQUEST              システムがIMEを変更しようとしている
WM_IME_SELECT               システムがIMEを変更しようとしている
WM_IME_SETCONTEXT           IMEの入力ウインドウがアクティブになった時アプリケーションに通知する
WM_IME_STARTCOMPOSITION     確定する前に通知する
WM_INITDIALOG               ダイアログボックスが表示される
WM_INITMENU                 メニューが表示される直前に通知
WM_INITMENUPOPUP            ポップアップメニューが表示される直前に通知
WM_INPUTLANGCHANGE          言語ロケールが変更されたときトップレベルウィンドウに通知
WM_INPUTLANGCHANGEREQUEST   言語ロケールが変更されたとき、入力フォーカスを持つウィンドウに通知
WM_KEYDOWN                  キーボードのキーが押された
WM_KEYUP                    キーが解放された
WM_KILLFOCUS                フォーカスがなくなる直前に通知
WM_LBUTTONDBLCLK            左のマウスボタンをダブルクリックした
WM_LBUTTONDOWN              左のマウスボタンを押した
WM_LBUTTONUP                左のマウスボタンが解放された
WM_MBUTTONDBLCLK            中央のマウスボタンがダブルクリックされた
WM_MBUTTONDOWN              中央のマウスボタンが押された
WM_MBUTTONUP                中央のマウスボタンが解放された
WM_MDIACTIVATE              MDIで指定した子ウインドウをアクティブにする
WM_MDICASCADE               MDI子ウインドウをカスケード表示にする
WM_MDICREATE                MDI子ウインドウを作成する
WM_MDIDESTROY               MDI子ウインドウを破棄する
WM_MDIGETACTIVE             現在アクティブなMDI子ウインドウのハンドルを取得
WM_MDIICONARRANGE           MDI子ウインドウのアイコンを整列
WM_MDIMAXIMIZE              MDI子ウインドウを最大化
WM_MDINEXT                  次の子ウインドウをアクティブにする
WM_MDIRESTORE               最大化・最小化されている子ウインドウを元のサイズに戻す
WM_MDISETMENU               メニューをMDIフレームウインドウにリンク
WM_MDITILE                  MDI子ウインドウをタイル整列
WM_MEASUREITEM              オーナー描画コントロールが作成されたとき
WM_MENUCHAR                 メニューのアクセスキーの英文字と一致しない文字を受け取った
WM_MENUSELECT               メニューが強調表示・選択された
WM_MOUSEACTIVATE            マウスクリックによりアクティブウインドウが移った
WM_MOUSEMOVE                マウスが移動した
WM_MOUSEWHELL               マウスホイールが回転した
WM_MOVE                     ウインドウが移動した
WM_MOVING                   リサイズ中にウィンドウに送られる
WM_NCACTIVATE               非クライアント領域がアクティブ・非アクティブである
WM_NCCALCSIZE               ウインドウのサイズを再計算する
WM_NCCREATE                 ウインドウのスタイルが変更された時に最初に送られるメッセージ
WM_NCDESTROY                非クライアント領域が破棄
WM_NCHITTEST                非クライアント領域のどの部分でマウスが操作されたか調べる
WM_NCLBUTTONDBCLK           非クライアント領域で左ボタンをダブルクリック
WM_NCLBUTTONDOWN            非クライアント領域で左ボタンを押す
WM_NCLBUTTONUP              非クライアント領域で左ボタンを解放
WM_NCMBUTTONDBCLK           非クライアント領域で中央ボタンをダブルクリック
WM_NCMBUTTONDOWN            非クライアント領域で中央ボタンを押す
WM_NCMBUTTONUP              非クライアント領域で中央ボタンを解放
WM_NCMOUSEMOVE              非クライアント領域でマウスが移動
WM_NCPAINT                  非クライアント領域で再ペイントが必要
WM_NCRBUTTONDBCLK           非クライアント領域で右ボタンをダブルクリック
WM_NCRBUTTONDOWN            非クライアント領域で右ボタンを押す
WM_NCRBUTTONUP              非クライアント領域で右ボタンを解放
WM_NEXTDLGCTL               入力フォーカスを別の子ウインドウに移動
WM_NEXTMENU                 メニューバーまたはシステムメニュー内を左右の矢印キーを使用して移動したとき
WM_NOTIFY                   コントロールにイベントが発生した時、またはコントロールがデータを要求している
WM_NULL                     これ自体は何もしないがメッセージを操作する場合に使う
WM_PAINT                    ウインドウのクライアント領域を再描画しなければならない
WM_PAINTCLIPBOARD           ウインドウのクライアント領域を再描画しなければならない
WM_PAINTICON                ウインドウのクライアント領域を再描画しなければならない
WM_PALETTECHANGED           システムパレットが変更された
WM_PALETTEISCHANGING        システムパレットが別のウインドウによって変更されようとしている
WM_PASTE                    テキストボックス、コンボボックスにおいてクリップボードからコピーするとき送る
WM_PARENTNOTIFY             あるイベントが子ウインドウで発生したことを親ウインドウに通知する
WM_POWER                    システムがパワー節約状態に入る・出ること通知する
WM_POWERBROADCAST           パワー管理イベントを検出するために送られてくる
WM_PRINT                    指定のデバイスコンテキストの再描画を要求
WM_PRINTCLIENT              指定のデバイスコンテキスト内のウィンドウのクライアント領域の描画を要求
WM_QUERYDRAGICON            アイコンを表示しようとする時にクラスアイコンを持たないウインドウに送る
WM_QUERYENDSESSION          Windowsを終了する時に全てのウインドウに問合わせる
WM_QUERYNEWPALETTE          入力フォーカスを受取る時、論理パレットで置換えるかどうか問合わせる
WM_QUERYOPEN                アイコン化されたウインドウが復元する時、直前の位置とサイズを要求されているかどうか問合わせる
WM_QUEUESYNC                ユーザー入力メッセージとフックプロシージャからのメッセージを区別する
WM_QUIT                     アプリケーションが処理する最後のメッセージ
WM_RBUTTONDBCLK             右のマウスボタンをダブルクリックした
WM_RBUTTONDOWN              右マウスボタンを押した
WM_RBUTTONUP                右マウスボタンを解放した
WM_RENDERALLFORMATS         ディレイドレンダリングしているとき、クリップボードのオーナーが破棄される前に送られる
WM_RENDERFORMAT             ディレイドレンダリングをしているデータをアプリケーションが要求したとき
WM_SETCURSOR                マウスをキャプチャしているウインドウがない時、マウスがあるウインドウに送られる
WM_SETFOCUS                 ウインドウが入力フォーカスを得た
WM_SETFONT                  論理フォントを設定する
WM_SETHOTKEY                ホットキーをウィンドウに関連付ける
WM_SETICON                  アイコンをウインドウに関連付ける
WM_SETREDRAW                コントロールの再描画操作を制御する
WM_SETTEXT                  ウインドウ(コントロール)のタイトル・テキストを変更
WM_SETTINGCHANGE            システム全体に影響する設定変更をしたとき
WM_SHOWWINDOW               ウインドウが表示・非表示になった
WM_SIZE                     ウインドウのサイズが変更
WM_SIZECLIPBOARD            クリップボードビューワのクライアント領域のサイズが変更されるとき
WM_SIZING                   ウィンドウのサイズが変更中
WM_SPOOLERSTATUS            印刷キューに印刷ジョブが追加・削除された
WM_STYLECHANGED             SetWindowLong関数によってウインドウスタイルを変更した
WM_STYLECHANGING            SetWindowLong関数によってウインドウスタイルを変更されそう
WM_SYNCPAINT                別のスレッドで作成されたウィンドウが上に重なったとき
WM_SYSCHAR                  [Alt]キーとの組合せ以外はWM_CHARと同じ
WM_SYSCOLORCHANGE           システムカラーが変更された
WM_SYSCOMMAND               システムメニューが操作された
WM_SYSDEADCHAR              アクセント記号のような別の文字と組合せた文字入力を受け取った
WM_SYSKEYDOWN               システム文字を意味するキーを押した
WM_SYSKEYUP                 システム文字を意味するキーが解放された
WM_TIMECHANGE               システムタイマが変更された
WM_TIMER                    SetTimer()で設定した時間を経過
WM_UNDO                     テキストボックスで Undo 操作を実行する
WM_USER                     ユーザーが定義できるメッセージの使用領域を表すだけでこれ自体に意味はない
WM_USERCHANGED              ログオン・ログオフ時に全てのウィンドウに送られる
WM_VKEYTOITEM               リストボックスがWM_KEYDOWNに応答したとき
WM_VSCROLL                  ユーザーが垂直スクロールバーを調整した
WM_VSCROLLCLIPBOARD         クリップボードビューワ内でスクロールイベント発生
WM_WINDOWPOSCHANGED         SetWindowPos()後に位置とサイズを変更したウインドウに送られる
WM_WINDOWPOSCHANGING        SetWindowPos()で位置とサイズを変更しているウインドウに送られる
WM_WININICHANGE             WIN.INIが変更された
*/

/*
メッセージの範囲

メッセージを通じて何らかの通知や要求を行いたい場合、 WM_COPYDATAを利用するのが必ずしも効果的とは限りません。
どのようなメッセージであれ、それを当事者間が通信の合図と認識できればよいのですから、
独自に定義した数値をメッセージとして利用するのもまた一興です。

ただし、このような場合、その定義した数値が既存のメッセージと重複してはなりませんから、
予め利用可能なメッセージの範囲を知っておかなければなりません。


メッセージの範囲

意味

0からWM_USER - 1    システムによって定義されているメッセージ。
WM_USERから0x7FFF   個々のウインドウクラスのためのメッセージ。 たとえば、ツールチップコントロールをアクティブにするTTM_ACTIVATEは、 WM_USER + 1として定義されている。
WM_APPから0xBFFF    アプリケーションが自由に使ってよいとされる範囲。
0xC000から0xFFFF    RegisterWindowMessageが返す値。
0xFFFFより大きい値  将来のために予約されている。

注目すべきところはWM_APPです。 このメッセージから0xBFFFまでの値はアプリケーションが自由に使うことが許されているため、
オリジナルなメッセージとしてアプリケーション間の通信に利用できます。
WM_COPYDATAはデータの送信を目的としたメッセージですが、単純に通知や要求などを行いたい場合はWM_APPのほうが適切でしょう。

#define WM_LAYOUTCHANGE WM_APP
・
・
PostMessage(hwndTarget, WM_LAYOUTCHANGE, 0, 0); // SendMessageでもよい


このコードでは、WM_APPを意図的にWM_LAYOUTCHANGEという形で定義していますが、
これはメッセージの役割を名前から推測できるようにするためです。
たとえば、対象となるウインドウにレイアウトの変更を促すのであれば、
それに即した名前を使ったほうがコードは分かりやすくなります。
wParamとlParamは自由に使うことができますが、
あくまで送れるのは4バイトのデータが2つまでとなります。
複数のオリジナルメッセージが必要な場合は、WM_APP+n(nは0から0xBFFF)という形で定義します。
*/

WNDPROC wpOrigWndProc = NULL;

// Subclass procedure 
LRESULT APIENTRY NB10WndProcCustom(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam)
{

	if (Msg == WM_CLOSE) {
		OutputDebugStream("WM_CLOSE\n");
	}
	else if (Msg == WM_DESTROY) {
		OutputDebugStream("WM_DESTROY\n");
		onDestroyWindow();
	}
	else if (Msg == WM_COMMAND) {
	}
	// マウス系の処理全て
	else if (WM_MOUSEFIRST <= Msg && Msg <= WM_MOUSELAST) {
	}
	// アプリ左上のアイコンを右クリした時に出てくるシステムコマンドを選んだとき
	else if (Msg == WM_SYSCOMMAND) {
	}
	else if (Msg == WM_NCMOUSEMOVE) {
	}
	else if (Msg == WM_MENUSELECT) {
	}
	else if (Msg == WM_INITMENUPOPUP) {
	}
	else if (WM_KEYFIRST <= Msg && Msg <= WM_KEYLAST) {
	}
	else if (Msg == WM_CONTEXTMENU) {
	}
	else if (Msg == WM_QUERYOPEN) {
	}
	else if (Msg == WM_PAINT) {
	}
	else if (Msg == WM_KILLFOCUS) {
	}
	else if (Msg == WM_SETFOCUS) {
	}
	else if (Msg == WM_COPYDATA) {
	}
	else if (WM_ACTIVATE) {
	}

	return CallWindowProc(wpOrigWndProc, hWnd, Msg, wParam, lParam);
}

using PFNSETWINDOWLONGA = LONG(WINAPI*)(HWND, int, LONG);

extern PROC pfnOrigSetWindowLongA;

// 画像系のハック
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // ウィンドウのハンドル
	UINT Msg,       // メッセージの識別子
	WPARAM wParam,  // メッセージの最初のパラメータ
	LPARAM lParam   // メッセージの 2 番目のパラメータ
)
{
	if (Msg == WM_CREATE) {
		char pszClassName[256] = { 0 };
		GetClassName(hWnd, pszClassName, _countof(pszClassName));
		if (string(pszClassName) == NB10_WINDOW_CLASS_NAME) {

			// ウィンドウ生成のタイミングで、ウィンドウプロシージャをこのMod内のもので指しはさむ
			wpOrigWndProc = (WNDPROC)((PFNSETWINDOWLONGA)pfnOrigSetWindowLongA)(hWnd, GWL_WNDPROC, (LONG)NB10WndProcCustom);

			onCreateWindow(hWnd);
		}
	}

	return 0;
}




