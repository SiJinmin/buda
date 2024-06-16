window.Buda={ dom:{}, };

/* e is the textarea
   max is the max height of textarea
   er is the container div of textarea if there is any
   freeH is the empty area at the content end */
window.Buda.dom.setAreaH=function (e, fontsize=16, max=null, er=null, freeH=3, minh=2) {
    let s=e.style, sr; if (er) { sr=er.style; sr.height=s.height; }
    s.minHeight=minh+'em';
    s.height=(fontsize*(1+freeH))+"px"; let v=(e.scrollHeight/fontsize+0.3+freeH);
    if (max&&v>max) v=max; s.height=v+"em"; // console.log('height', s.height, s.minHeight, e.value);    
    if (sr) sr.height=s.height; return v;
};