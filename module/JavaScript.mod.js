require("abc.js");

function abc() {
    console.log("これ？");
}

function onCreateメインウィンドウ(arg) {
    console.log("ウィンドウ生成きたよ");
    let a = 1/0;
    console.log(arg.ウィンドウハンドル);
    console.log(arg);
    let rand = new System.Random();
    console.log([1,2,3]);
    console.log("%s", 100);
    console.log([1,2,3]);
    console.log(require);
}

function onRequestフォント(arg) {
    ret = {};
    ret.フォント名 = "蒼天 明朝";
    return ret;
}

function onDestroyメインウィンドウ() {
    console.log("破棄来たよ!!");
}

function onRequest音楽(arg) {
    console.log("要求されているBGMファイルは:" + arg.ファイル名);

    ret = {};
    ret.ファイル名 = "BGM\\07.wav";
    return ret;
}

function onRequest効果音(arg) {
    console.log("要求されている効果音ファイルは:" + arg.ファイル名);

    ret = {};
    ret.ファイル名 = "SE\\SE117.WAV";
    return ret;
}

