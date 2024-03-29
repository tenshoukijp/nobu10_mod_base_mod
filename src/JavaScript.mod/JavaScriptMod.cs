using Microsoft.ClearScript;
using System;
using System.Dynamic;



namespace ゲーム;

internal partial class StaticLib
{
    public static void onメインウィンドウ生成後(int hWnd)
    {
        try
        {
            DoFile("JavaScript.mod.js");
            dynamic arg = new ExpandoObject();
            dynamic ret = new ExpandoObject();
            arg.ウィンドウハンドル = hWnd;
            engine.Script.onメインウィンドウ生成後(arg, ret);
        }
        catch (Exception e)
        {
            OutputDebugStream("onメインウィンドウ生成後Error:" + e.Message);
        }
    }

    public static void onメインウィンドウ破棄前()
    {
        if (engine != null)
        {
            try
            {
                dynamic ret = new ExpandoObject();
                dynamic arg = new ExpandoObject();
                engine.Script.onメインウィンドウ破棄前(arg, ret);
            }
            catch (Exception e)
            {
                OutputDebugStream("onメインウィンドウ破棄前Error:" + e.Message);
            }
        }
        DestroyEngine();
    }


    public static String onフォント要求時()
    {
        try
        {
            dynamic ret = new ExpandoObject();
            dynamic arg = new ExpandoObject();
            engine.Script.onフォント要求時(arg, ret);
            if (ret is Undefined) {
                return "";
            }
            if (ret.フォント名 is Undefined)
            {
                return "";
            }
            OutputDebugStream("フォント名:" + ret.フォント名.ToString());
            return ret.フォント名;
        }
        catch (Exception e)
        {
            OutputDebugStream("onフォント要求時Error:" + e.Message);
        }
        return "";
    }

    public static String on音楽要求時(String filepath)
    {
        try
        {
            dynamic arg = new ExpandoObject();
            dynamic ret = new ExpandoObject();
            arg.ファイル名 = filepath;
            engine.Script.on音楽要求時(arg, ret);
            if (ret is Undefined)
            {
                return "";
            }
            if (ret.ファイル名 is Undefined)
            {
                return "";
            }
            return ret.ファイル名;
        }
        catch (Exception e)
        {
            OutputDebugStream("on音楽要求時Error:" + e.Message);
        }
        return "";
    }

    public static String on効果音要求時(String filepath)
    {
        try
        {
            dynamic arg = new ExpandoObject();
            dynamic ret = new ExpandoObject();
            arg.ファイル名 = filepath;
            engine.Script.on効果音要求時(arg, ret);
            if (ret is Undefined)
            {
                return "";
            }
            if (ret.ファイル名 is Undefined)
            {
                return "";
            }
            return ret.ファイル名;
        }
        catch (Exception e)
        {
            OutputDebugStream("on効果音要求時Error:" + e.Message);
        }
        return "";
    }

    public static String on顔画像要求時(int iKaoID)
    {
        try
        {
            dynamic arg = new ExpandoObject();
            dynamic ret = new ExpandoObject();
            arg.画像番号 = iKaoID;
            engine.Script.on顔画像要求時(arg, ret);
            if (ret is Undefined)
            {
                return "";
            }
            if (ret.ファイル名 is Undefined)
            {
                return "";
            }
            return ret.ファイル名;
        }
        catch (Exception e)
        {
            OutputDebugStream("on顔画像要求時Error:" + e.Message);
        }
        return "";
    }

    public static String on家宝画像要求時(int iKahouPicID)
    {
        try
        {
            dynamic arg = new ExpandoObject();
            dynamic ret = new ExpandoObject();
            arg.画像番号 = iKahouPicID;
            engine.Script.on家宝画像要求時(arg, ret);
            if (ret is Undefined)
            {
                return "";
            }
            if (ret.ファイル名 is Undefined)
            {
                return "";
            }
            return ret.ファイル名;
        }
        catch (Exception e)
        {
            OutputDebugStream("on家宝画像要求時Error:" + e.Message);
        }
        return "";
    }

    public static String on家紋画像要求時(int iKamonID)
    {
        try
        {
            dynamic arg = new ExpandoObject();
            dynamic ret = new ExpandoObject();
            arg.画像番号 = iKamonID;
            engine.Script.on家紋画像要求時(arg);
            if (ret is Undefined)
            {
                return "";
            }
            if (ret.ファイル名 is Undefined)
            {
                return "";
            }
            return ret.ファイル名;
        }
        catch (Exception e)
        {
            OutputDebugStream("on家紋画像要求時Error:" + e.Message);
        }
        return "";
    }



    public static String onファイル要求時(string filename)
    {
        try
        {
            dynamic arg = new ExpandoObject();
            dynamic ret = new ExpandoObject();
            arg.ファイル名 = filename;
            engine.Script.onファイル要求時(arg, ret);
            if (ret is Undefined)
            {
                return "";
            }
            if (ret.ファイル名 is Undefined)
            {
                return "";
            }
            OutputDebugStream("ファイル名:" + ret.ファイル名);
            return ret.ファイル名;
        }
        catch (Exception e)
        {
            OutputDebugStream("onファイル要求時Error:" + e.Message);
        }
        return "";
    }

}