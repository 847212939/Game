using Client.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assets.Scripts.UIPrefabsMgr
{

    // 配置管理类
    internal class LoadConfigMgr : ConfigMgr
    {
        private bool m_loadCfg;
        private static LoadConfigMgr m_LoadConfigMgr = null!;

        public PrefabsCfg m_PrefabsCfg;    // 预制体

        public static LoadConfigMgr GetInstance()
        {
            if (m_LoadConfigMgr == null)
            {
                m_LoadConfigMgr = new LoadConfigMgr();
            }

            return m_LoadConfigMgr;
        }

        private LoadConfigMgr() : base()
        {
            m_loadCfg = false;
            m_PrefabsCfg = new PrefabsCfg();
        }

        public void InitLoadConfigMgr()
        {
            if (m_loadCfg) return;

            m_PrefabsCfg.Read(this);

            m_loadCfg = true;   // 放在最后表示加载配置完成
        }
    }
}
