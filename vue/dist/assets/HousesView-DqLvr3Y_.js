import{b as a,o as e,c as o,F as r,r as l,a as n,t as s,e as c,i as m,j as h,u as y,d as p,g as f}from"./index-Dv3qImzH.js";import{_ as g}from"./Img-CnuzyXnn.js";const v={class:"grid_c"},k={class:"block"},b={class:"block_inner"},B={class:"title"},V={key:0,class:"contacts_c"},x=n("div",null,[n("span",{class:"field_title"},"联系人")],-1),N={class:"contacts"},w={class:"contact"},$={key:0},F={key:1},H={class:"community"},j={key:0},z={__name:"Houses",setup(u){return a(()=>{Buda.window_resize()}),(_,d)=>(e(),o("div",v,[(e(!0),o(r,null,l(y(p).houses.filter(t=>!t.closed),t=>(e(),o("div",k,[n("div",b,[n("div",B,s(t.community.name)+s(t.area)+"平米"+s(t.fitment)+s(t.format)+"出租，"+s(t.price)+"元/月",1),t.contacts&&t.contacts.length?(e(),o("div",V,[x,n("div",N,[(e(!0),o(r,null,l(t.contacts,i=>(e(),o("div",w,s(i),1))),256))])])):c("",!0),n("div",null,s(t.devices),1),n("div",null,[m("房子位于第"+s(t.floor)+"层",1),t.floors_count?(e(),o("span",$,"/共"+s(t.floors_count)+"层",1)):c("",!0),t.lift?(e(),o("span",F,"，有"+s(t.lift)+"部电梯",1)):c("",!0)]),n("div",H,[n("div",null,[m(s(t.community.name)+"小区位于"+s(t.addr),1),t.community.birth?(e(),o("span",j,"，建成于"+s(t.community.birth)+"，共有"+s(t.community.building_count)+"栋建筑，"+s(t.community.house_count)+"套房子，绿化率为"+s(t.community.green)+"，容积率为"+s(t.community.rate)+"，物业费按"+s(t.community.fee)+"元/平方/月收取，物业公司为"+s(t.community.manage)+"，开发商为"+s(t.community.developer)+"。 参考售价为"+s(t.sale)+"元/平方。",1)):c("",!0)])]),n("div",null,s(t.summary),1),t.imgs&&t.imgs.length?(e(),h(g,{key:1,imgs:t.imgs.map(i=>({alt:i,src:`/images/houses/small/${i}`})),img_class:{detail_image:!0}},null,8,["imgs"])):c("",!0)])]))),256))]))}},E={__name:"HousesView",setup(u){return(_,d)=>(e(),o("main",null,[f(z)]))}};export{E as default};
