import{_,b as a,o as s,c as i,F as l,r,a as n,t,e as u,u as v,d as p,p as h,f as g,g as y}from"./index-Cf83duXP.js";const f=c=>(h("data-v-d882e3e3"),c=c(),g(),c),k={class:"grid_c"},b={class:"block"},w={class:"block_inner"},x={class:"title"},B={class:"community"},I={key:0,class:"contacts_c"},L=f(()=>n("div",null,"联系人：",-1)),S=["title","src"],V=["title","src"],N={__name:"Live",setup(c){return a(()=>{Buda.window_resize()}),(d,m)=>(s(),i("div",k,[(s(!0),i(l,null,r(v(p).live.filter(e=>!e.closed),e=>(s(),i("div",b,[n("div",w,[n("div",x,t(e.community.name)+t(e.area)+"平米"+t(e.fitment)+t(e.format)+"出租，"+t(e.price)+"元/月",1),n("div",null,"房东："+t(e.owner.name)+" "+t(e.owner.tel),1),n("div",null,t(e.devices),1),n("div",null,"第"+t(e.floor)+"层/共"+t(e.floors_count)+"层，有"+t(e.lift)+"部电梯",1),n("div",B,[n("div",null,t(e.community.name)+"小区位于"+t(e.addr)+"，建成于"+t(e.community.birth)+"，共有"+t(e.community.building_count)+"栋建筑，"+t(e.community.house_count)+"套房子，绿化率为"+t(e.community.green)+"，容积率为"+t(e.community.rate)+"，物业费按"+t(e.community.fee)+"元/平方/月收取，物业公司为"+t(e.community.manage)+"，开发商为"+t(e.community.developer)+"。 参考售价为"+t(e.sale)+"元/平方。",1)]),e.contacts&&e.contacts.length?(s(),i("div",I,[L,n("div",null,[(s(!0),i(l,null,r(e.contacts,o=>(s(),i("div",null,t(o),1))),256))])])):u("",!0),n("div",null,t(e.summary),1),e.detail&&e.detail.length?(s(!0),i(l,{key:1},r(e.detail,o=>(s(),i("div",null,[n("img",{title:o,src:`/images/companies/small/${o}`,class:"detail_image"},null,8,S)]))),256)):u("",!0),e.env&&e.env.length?(s(!0),i(l,{key:2},r(e.env,o=>(s(),i("div",null,[n("img",{title:o,src:`/images/companies/small/${o}`,class:"detail_image"},null,8,V)]))),256)):u("",!0)])]))),256))]))}},$=_(N,[["__scopeId","data-v-d882e3e3"]]),z={__name:"LiveView",setup(c){return(d,m)=>(s(),i("main",null,[y($)]))}};export{z as default};
