import{_ as u,b as m,o as s,c as t,F as a,r as l,a as c,t as i,e as _,u as p,d as h,p as v,f as g,g as b}from"./index-Cf83duXP.js";const f=n=>(v("data-v-a09b4e14"),n=n(),g(),n),k={class:"grid_c"},y={class:"block"},w={class:"block_inner"},x={class:"company"},B={key:0,class:"contacts_c"},I=f(()=>c("div",{class:"field_label"},"联系人：",-1)),S={class:"contacts"},V={class:"contact"},J=["title","src"],N=["title","src"],$={__name:"Jobs",setup(n){return m(()=>{Buda.window_resize()}),(r,d)=>(s(),t("div",k,[(s(!0),t(a,null,l(p(h).jobs.filter(e=>!e.closed),e=>(s(),t("div",y,[c("div",w,[c("div",x,i(e.company),1),c("div",null,"地址："+i(e.location),1),e.contacts&&e.contacts.length?(s(),t("div",B,[I,c("div",S,[(s(!0),t(a,null,l(e.contacts,o=>(s(),t("div",V,i(o),1))),256))])])):_("",!0),c("div",null,i(e.summary),1),e.detail&&e.detail.length?(s(!0),t(a,{key:1},l(e.detail,o=>(s(),t("div",null,[c("img",{title:o,src:`/images/companies/small/${o}`,class:"detail_image"},null,8,J)]))),256)):_("",!0),e.env&&e.env.length?(s(!0),t(a,{key:2},l(e.env,o=>(s(),t("div",null,[c("img",{title:o,src:`/images/companies/small/${o}`,class:"detail_image"},null,8,N)]))),256)):_("",!0)])]))),256))]))}},F=u($,[["__scopeId","data-v-a09b4e14"]]),z={__name:"JobsView",setup(n){return(r,d)=>(s(),t("main",null,[b(F)]))}};export{z as default};
