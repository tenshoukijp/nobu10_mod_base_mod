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
    ret.フォント名 = "将星 明朝";
    return ret;
}

function onDestroyメインウィンドウ() {
    console.log("破棄来たよ!!");
}

function onRequest音楽(arg) {
}

function onRequest効果音(arg) {
}

