using Client.Util;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

namespace Assets.Scripts.UIPrefabsMgr
{
    using PrefabsDir = Dictionary<uint, Dictionary<string, string>>;

    internal class PrefabsCfg
    {
        private PrefabsDir m_PrefabsDir;

        public PrefabsCfg()
        {
            m_PrefabsDir = new PrefabsDir();
        }

        public void Read(LoadConfigMgr loadConfigMgr)
        {
            string data;

            loadConfigMgr.LoadDataString("PrefabsCfg", "ReadBaseCfg", out data);
            if (data.Length <= 0)
            {
                UnityEngine.Debug.Log("ReadBaseCfg is err");
                return;
            }

            Netmsg cout = new Netmsg(data);
            uint size = cout.ReadUInt32();
            for (int i = 0; i < size; i++)
            {
                uint scentid = cout.ReadUInt32();
                uint cnt = cout.ReadUInt32();
                for (int j = 0; j < cnt; j++)
                {
                    string name = cout.ReadString();
                    string path = cout.ReadString();

                    Dictionary<string, string> tmpDir = null;
                    if (m_PrefabsDir.ContainsKey(scentid))
                    {
                        tmpDir = m_PrefabsDir[scentid];
                    }
                    else
                    {
                        tmpDir = new Dictionary<string, string>();
                        m_PrefabsDir.Add(scentid, tmpDir);
                    }

                    if (tmpDir.ContainsKey(name))
                    {
                        continue;
                    }

                    tmpDir.Add(name, path);
                }
            }
        }

        public PrefabsDir GetPrefabsCfgDir()
        {
            return m_PrefabsDir;
        }
    }
}
