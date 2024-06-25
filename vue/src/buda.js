import jobs from './jobs.js'
import houses from './houses.js'
import goods from './goods.js'

import { reactive } from 'vue'

let ui = { 
  w: window.innerWidth, h: window.innerHeight,
  rem: 16, 
  list :{ item:{ w: 25 }},
};

let community = '金银湖科技园';
let site = { title: '资讯服务' };
let page ={ module:{}, }; // current page

let modules = [
  { title:'招聘信息', live:true, Code: 'Jobs', des:'社区及周边的招聘信息，包括全职、兼职、小时工等各种工作形式。', },
  { title:'房屋租售', live:true, Code: 'Houses', des:'房屋出租、出售、转让信息。', },
  { title:'传记工具', title2:'人物传记整理工具', live:true, Code:'Lifeditor', des:'整理人物传记的小工具，按时间排序，可以自动计算人的年龄，一边叙述人物的真实经历、心理状态和行为造作，一边用佛法分析它们的因果关系。', },
  { title:'日志查看', Code: 'Log', des:'查看日志', },

  { title:'本站内容', Code: 'Home', des:'本站提供的服务项目。', },
  { title:'最新资讯', Code: 'Home', des:'本站所有最新资讯的综合看板。', },
  { title:'求职信息', Code: 'Partners', des:'社区及周边的求职者信息，包括全职、兼职、小时工等各种求职方式。', },
  { title:'公司目录', Code: 'Companies', des:'社区及周边的所有公司的列表。', },
  { title:'服务目录', Code: 'Services', des:'社区及周边的所有服务列表，包括个人提供的服务和公司提供的服务。', },
  { title:'纯素超市', Code: 'Goods', des:'纯素版的朴朴、叮咚，订购后半小时内送货上门，所有商品都是纯植物原料的，不含动物制品。', },
  { title:'兴趣交友', des:'以兴趣爱好为主题的聚会交友活动。', },
  { title:'创业合作', des:'以创业项目演示为主题的聚会交友活动，招募创业合作伙伴，筹集资金。', },
  { title:'友友速递', des:'小伙伴资料库，根据每个人设置的交友偏好条件自动匹配最佳友人。', },
  { title:'追爱助攻', des:'提供帮助小伙伴们追求爱慕对象的服务。', },
  { title:'解忧谋士', des:'提供帮助大家解决生活中遇到的各种困惑烦恼的服务。', },
  { title:'物资流通', des:'二手物品出售、免费捐赠、物物交换、物品出租。', },
  { title:'交通物流', des:'发布顺风车、专车、货运等用车需求。', },
  { title:'关于本站', Code:'About', des:'本站的服务项目、宗旨、发展历史、未来规划。', },
];
for(let a of modules){
  if(a.Code) a.code=a.Code.toLowerCase(); else a.Code=a.code='';
  if(a.code=='home') a.route='/'; else a.route=`/${a.code}`;
  if(!a.title2) a.title2=a.title;
}
let live_modules=modules.filter(s=>s.live);

let buda = reactive({
  ui,
  community, site, page,
  live_modules, modules,
  jobs, houses, goods,
});

site.name=`${community}社区${site.title}`;

export default buda;