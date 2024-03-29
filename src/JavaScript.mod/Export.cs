using System;

public class IJavaScriptMod
{
    public static void onメインウィンドウ生成後(int hWnd)
    {
        ゲーム.StaticLib.onメインウィンドウ生成後(hWnd);
    }

    public static String onフォント要求時()
    {
        return ゲーム.StaticLib.onフォント要求時();
    }
    public static void onメインウィンドウ破棄前()
    {
        ゲーム.StaticLib.onメインウィンドウ破棄前();
    }

    public static String on音楽要求時(String filepath)
    {
        return ゲーム.StaticLib.on音楽要求時(filepath);
    }

    public static String on効果音要求時(String filepath)
    {
        return ゲーム.StaticLib.on効果音要求時(filepath);
    }
    public static String onファイル要求時(String filepath)
    {
        return ゲーム.StaticLib.onファイル要求時(filepath);
    }

    public static String on顔画像要求時(int KaoID)
    {
        return ゲーム.StaticLib.on顔画像要求時(KaoID);
    }
    public static String on家宝画像要求時(int PicID)
    {
        return ゲーム.StaticLib.on家宝画像要求時(PicID);
    }

    public static String on家紋画像要求時(int PicID)
    {
        return ゲーム.StaticLib.on家紋画像要求時(PicID);
    }

}
