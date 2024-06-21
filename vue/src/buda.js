import jobs from './jobs.js'
import live from './live.js'
import goods from './goods.js'

import { reactive } from 'vue'

let ui = { 
  w: window.innerWidth, h: window.innerHeight,
  rem: 16, 
  list :{ item:{ w: 25 }},
};

let community = '金银湖科技园';
let site = { title: '资讯服务' };
let page ={ name:'' }; // current page

let services = [
  { title:'招聘信息', Code: 'Jobs', des:'社区及周边的招聘信息，包括全职、兼职、小时工等各种工作形式。', },
  { title:'求职信息', Code: 'Partners', des:'社区及周边的求职者信息，包括全职、兼职、小时工等各种求职方式。', },
  { title:'房屋租售', Code: 'Houses', des:'房屋出租、出售、转让信息。', },
  { title:'公司目录', Code: 'Companies', hide:true, des:'社区及周边的所有公司的列表。', },
  { title:'服务目录', Code: 'Services', hide:true, des:'社区及周边的所有服务列表，包括个人提供的服务和公司提供的服务。', },
  { title:'最新资讯', Code: 'Home', hide:true, des:'本站所有最新资讯的综合看板。', },
  { title:'纯素超市', Code: 'Goods', hide:true, des:'纯素版的朴朴、叮咚，订购后半小时内送货上门，所有商品都是纯植物原料的，不含动物制品。', },
  { title:'兴趣交友', hide:true, des:'以兴趣爱好为主题的聚会交友活动。', },
  { title:'创业合作', hide:true, des:'以创业项目演示为主题的聚会交友活动，招募创业合作伙伴，筹集资金。', },
  { title:'友友速递', hide:true, des:'小伙伴资料库，根据每个人设置的交友偏好条件自动匹配最佳友人。', },
  { title:'追爱助攻', hide:true, des:'提供帮助小伙伴们追求爱慕对象的服务。', },
  { title:'解忧谋士', hide:true, des:'提供帮助大家解决生活中遇到的各种困惑烦恼的服务。', },
  { title:'物资流通', hide:true, des:'二手物品出售、免费捐赠、物物交换、物品出租。', },
  { title:'交通物流', hide:true, des:'发布顺风车、专车、货运等用车需求。', },
  { title:'传记整理', hide:true, Code:'Lifeditor', des:'整理人物传记的小工具，按时间排序，可以自动计算人的年龄。', },
  { title:'关于本站', hide:true, Code:'About', des:'本站的服务项目、宗旨、发展历史、未来规划。', },
];
for(let a of services){
  if(a.Code) a.code=a.Code.toLowerCase(); else a.Code=a.code='';
  if(a.code=='home') a.route='/'; else a.route=`/${a.code}`;
}

let buda = reactive({
  ui,
  community, site, page,
  services,
  jobs, live, goods,
});

site.name=`${community}社区${site.title}`;

export default buda;