
function onメインウィンドウ生成後(arg, ret) {
    let guid = System.Guid.NewGuid().ToString();
    console.log("GUID:" + guid);

    let isFileExist = System.IO.File.Exists("./OVERRIDE/aaa.txt");
    if (isFileExist) {
        console.log("aaa.txtがある");
    } else {
        console.log("aaa.txtはない");
    }
    console.log("onメインウィンドウ生成後");
}

function onフォント要求時(arg, ret) {
    ret.フォント名 = "蒼天 明朝";
}

function onメインウィンドウ破棄前(arg, ret) {
    console.log("onメインウィンドウ破棄前");
}

function on音楽要求時(arg, ret) {
    console.log("onRequest音楽\n");
    console.log("元のBGM:" + arg.ファイル名 + "\n");
}

function on効果音要求時(arg, ret) {
    console.log("元の効果音:" + arg.ファイル名 + "\n");
    /*
    let ret = {};
    if (arg.ファイル名 == "se\\se999.wav") {
        ret.ファイル名 = arg.ファイル名;
    }
    if (arg.ファイル名 == "se\\se000.wav") {
        console.log("一致したので変更する。\n");
        ret.ファイル名 = "se\\se001.wav";
    }
    else if (arg.ファイル名 == "se\\se001.wav") {
        console.log("一致したので変更する。\n");
        ret.ファイル名 = "se\\se002a.wav";
    }
    else {
        console.log("一致しない。\n");
    }
    */
}

function on顔画像要求時(arg, ret) {
    console.log("onRequest顔画像のID:" + arg.画像番号 + "\n");
    /*
    // 0000.bmp からではなく、 0001.bmp からにする場合など
    ret.ファイル名 = System.String.Format("OVERRIDE\\MYKAODATA\\{0:D4}.bmp", arg.画像ID+1);
    */
}

function on家宝画像要求時(arg, ret) {
    console.log("onRequest家宝画像のID:" + arg.画像番号 + "\n");
    /*
    let ret = {};
    // 特定のファイルだけ別のフォルダから読み込ませるなども可能
    ret.ファイル名 = System.String.Format("OVERRIDE_MYITEMCG\\{0:D3}.bmp", arg.画像ID);
    */
}

