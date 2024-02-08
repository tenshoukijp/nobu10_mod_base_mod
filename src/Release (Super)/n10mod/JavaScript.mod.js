

function onCreateメインウィンドウ(arg) {
    console.log("onCreateメインウィンドウ");
}

function onRequestフォント(arg) {
    let ret = {};
    ret.フォント名 = "ＭＳ 明朝"; // HG明朝E
    return ret;
}

function onDestroyメインウィンドウ() {
    console.log("onDestroyメインウィンドウ");
}

function onRequest音楽(arg) {
    console.log("onRequest音楽\n");
    console.log("元のBGM:" + arg.ファイル名 + "\n");
    let ret = {};
    return ret;
}

function onRequest効果音(arg) {
    console.log("元の効果音:" + arg.ファイル名 + "\n");
    /*
    let ret = {};
    if (arg.ファイル名 == "se\\se999.wav") {
        ret.ファイル名 = arg.ファイル名;
        return ret;
    }
    if (arg.ファイル名 == "se\\se000.wav") {
        console.log("一致したので変更する。\n");
        ret.ファイル名 = "se\\se001.wav";
        return ret;
    }
    else if (arg.ファイル名 == "se\\se001.wav") {
        console.log("一致したので変更する。\n");
        ret.ファイル名 = "se\\se002a.wav";
        return ret;
    }
    else {
        console.log("一致しない。\n");
    }
    */
}

function onRequest顔画像(arg) {
    console.log("onRequest顔画像のID:" + arg.画像ID + "\n");
    /*
    let ret = {};
    // 0000.bmp からではなく、 0001.bmp からにする場合など
    ret.ファイル名 = System.String.Format("OVERRIDE\\MYKAODATA\\{0:D4}.bmp", arg.画像ID+1);
    return ret;
    */
}

function onRequest家宝画像(arg) {
    console.log("onRequest家宝画像のID:" + arg.画像ID + "\n");
    /*
    let ret = {};
    // 特定のファイルだけ別のフォルダから読み込ませるなども可能
    ret.ファイル名 = System.String.Format("OVERRIDE_MYITEMCG\\{0:D3}.bmp", arg.画像ID);
    return ret;
    */
}

