//using XLua;
//using UnityEngine;
//using System.IO;

//namespace Assets.Scripts.UIPrefabsMgr
//{
//    [CSharpCallLua]
//    public delegate void LuaCallBackFunc(out string data);

//    internal class ConfigMgr
//    {
//        private LuaEnv m_LuaEnv;

//        public ConfigMgr()
//        {
//            m_LuaEnv = new LuaEnv();
//            m_LuaEnv.AddLoader(LuaCustomLoader);
//            m_LuaEnv.DoString("require('LoadScript/LoadScript')");
//        }

//        private byte[] LuaCustomLoader(ref string fileName)
//        {
//            string path = Application.dataPath + "/Config/" + fileName + ".lua";

//            if (File.Exists(path))
//            {
//                return File.ReadAllBytes(path);
//            }
//            else
//            {
//                Debug.Log("MyCustonLoader重定向文件失败，文件名为" + fileName);
//            }

//            return null;
//        }

//        public void LoadDataString(string LuaFileName, string luaFuncName, out string data)
//        {
//            m_LuaEnv.DoString("require('" + LuaFileName + "')");

//            LuaCallBackFunc CallBackFunc = m_LuaEnv.Global.Get<LuaCallBackFunc>(luaFuncName);
//            CallBackFunc(out data);
//        }
//    }
//}
