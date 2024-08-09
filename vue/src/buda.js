import jobs from './jobs.js'
import companies from './companies.js'
import houses from './houses.js'
import goods from './goods.js'
import ideals from './ideals.js'

import { reactive } from 'vue'

let ui = { 
  w: window.innerWidth, h: window.innerHeight,
  rem: 16, 
  list :{ item:{ w: 25 } },
};

let site = { title: '玲明艺术创作' };
let page ={ module:{}, msg:'', }; // current page

let modules = [
  { title:'玲明超市', Code: 'Goods', live:true, menu: true, des:'文化传播艺术品、日常生活必需品。所有商品均为纯植物原料，不含动物制品（部分商品含有乳酪）。', },
  { title:'招聘合作', Code: 'Jobs', live:true, menu: true, des:'招聘小时工，工作时间自由，工资日结。', },
  { title:'供货厂商', Code: 'Companies', live:true, des:'供货厂商', },
  { title:'传记工具', title2:'人物传记整理工具', Code:'Lifeditor', live:true, menu: true, des:'整理人物传记的小工具，按时间排序，可以自动计算人的年龄，一边叙述人物的真实经历、心理状态和行为造作，一边用佛法分析它们的因果关系。', },

  { 
    title:'全民兼职', title2:'全民兼职业务员组织', Code: 'Companies', live:false, menu: false, 
    des:'商机无处不在，业务不期而遇，每个人都有自己独特的人脉关系和资源，随时都可能遇到第一手的业务资讯，将它出售给需要的公司，举手之劳，可以繁荣公司的业务，同时让自己获利。如果您能帮助下列公司和个人介绍相关的业务，促成销售交易，您将获得销售额的10%作为您介绍业务的报酬。', 
  },
  { title:'房屋租售', Code: 'Houses', live:false, menu: false, des:'社区及周边的房屋出租、出售、转让信息。', },
  { title:'创业合作', title2:'创业合作 聚会交友', Code: 'Ideals', live:false, menu: false, des:'以创业项目演示为主题的聚会交友活动，招募创业合作伙伴，筹集创业资金。', },
  { title:'项目介绍', title2:'「社区资讯服务」创业项目介绍', Code:'BudaProject', live:false, menu: false, des:'社区资讯服务创业项目的目标是为全世界的每一个小区建造专属的资讯服务网站，收集本小区的人、事、物资讯，让小区的每一个人过上幸福的生活。', },

  { title:'活动详情', Code: 'Ideal', route:'/ideal/:id', live:false, des:'以创业项目演示为主题的聚会交友活动，招募创业合作伙伴，筹集创业资金。', },

  { title:'日志查看', Code: 'Log', des:'查看日志', },
  { title:'本站内容', Code: 'Home', des:'', },
  { title:'最新资讯', Code: 'Home', des:'本站所有最新资讯的综合看板。', },
  { title:'求职信息', Code: 'Partners', des:'社区及周边的求职者信息，包括全职、兼职、小时工等各种求职方式。', },
  { title:'兴趣交友', des:'以兴趣爱好为主题的聚会交友活动。', },
  { title:'友友速递', des:'小伙伴资料库，根据每个人设置的交友偏好条件自动匹配最佳友人。', },
  { title:'追爱助攻', des:'提供帮助小伙伴们追求爱慕对象的服务。', },
  { title:'解忧谋士', des:'提供帮助大家解决生活中遇到的各种困惑烦恼的服务。', },
  { title:'物资流通', des:'二手物品出售、免费捐赠、物物交换、物品出租。', },
  { title:'交通物流', des:'发布顺风车、专车、货运等用车需求。', },
  { title:'关于本站', Code:'About', des:'本站的服务项目、宗旨、发展历史、未来规划。', },
];
for(let a of modules){
  if(a.Code) a.code=a.Code.toLowerCase(); else a.Code=a.code='';
  if(!a.route) { if(a.code=='home') a.route='/'; else a.route=`/${a.code}`; }
  if(!a.title2) a.title2=a.title;
}
let menu_modules=modules.filter(s=>s.live && s.menu);

let buda = reactive({
  ui,
  site, page,
  menu_modules, modules,
  jobs, companies, houses, goods, ideals,
});

site.name=`${site.title}`;

export default buda;