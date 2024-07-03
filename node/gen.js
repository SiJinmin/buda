
let dir="D:\\武汉烟厂\\山东区域\\备档资料\\销区零售终端APP维护清单\\走访档案\\2024年6月\\200户走访档案\\";
let dir2="D:\\liujuan\\new_gen\\";


const fs = require('fs');
const path = require('path');
const ExcelJS = require('exceljs');


// get text in cell
function gv(cell) {
  let v=cell.value; if(!v) return '';
  v = (v.richText? v.richText.map(({ text }) => text).join(''): v.toString());
  return v.trim().toLowerCase();
}

async function make_console(filename, sheetname, list)
{
  const FilePath = path.join(dir2, `console_${filename}_${sheetname}.js`);    
  const file_content=`
let sheet_name="${sheetname}";
let list=${list};


let diffs='', diffj=[]; let nl='\\r\\n';
let noinput=document.getElementById('licensenum');
function check_one(list, itemno) 
{ 
  if(itemno>=list.length){ console.log(diffs); console.log(diffj); return; }
  console.log('checking '+itemno);
  let item=list[itemno]; let { no }=item; noinput.value=no; queryInfo(); 
  setTimeout(()=>{
    let diff={no, cols:[]}; let pushed_diff=false;
    let tbody=document.getElementById('detailTbodyId'); 
    if(!tbody || !tbody.children || tbody.children.length<1 || !tbody.children[0].children || tbody.children[0].children.length<4)
    { let msg='许可证不存在1: '+no; console.log(msg); diffs+=(msg+nl+nl); diffj.push(diff); check_one(list, itemno+1); return; }
    let tds=tbody.children[0].children; 
    if(tds[0].innerText.toLowerCase().trim()!=no) 
    { let msg='许可证不存在2: '+no; console.log(msg); diffs+=(msg+nl+nl); diffj.push(diff); check_one(list, itemno+1); return; }
    
    if(tds[1].innerText.toLowerCase().trim()!=item.company) 
    { if(!pushed_diff) { pushed_diff=true; diffj.push(diff); } diff.cols.push('company'); diffs+=(no+': '+nl+tds[1].innerText+nl+item.company+nl+nl);}
    if(tds[2].innerText.toLowerCase().trim()!=item.addr) 
    { if(!pushed_diff) { pushed_diff=true; diffj.push(diff); } diff.cols.push('addr'); diffs+=(no+': '+nl+tds[2].innerText+nl+item.addr+nl+nl);}
    if(!tds[3].innerText.toLowerCase().includes(item.date)) 
    { if(!pushed_diff) { pushed_diff=true; diffj.push(diff); } diff.cols.push('date');  diffs+=(no+': '+nl+tds[3].innerText+nl+item.date+nl+nl);}
    check_one(list, itemno+1);
  }, 1000);  
}
check_one(list, 0);
`;
  fs.writeFileSync(FilePath, file_content);
}
 
async function make_mark(filename, sheetname)
{
  const FilePath = path.join(dir2, `mark_${filename}_${sheetname}.js`);    
  const file_content=`
let diffs=
;

let filename='${filename}';
let sheet_name="${sheetname}";

let dir="D:\\\\武汉烟厂\\\\山东区域\\\\备档资料\\\\销区零售终端APP维护清单\\\\走访档案\\\\2024年6月\\\\200户走访档案\\\\";

const ExcelJS = require('exceljs');
const fs = require('fs');
 
let style_red={font:{color:{ argb: 'FFFF0000'}}};
let style_black={font:{color:{ argb: 'FF000000'}}};
let style_blue={font:{color:{ argb: 'FF0000FF'}}};

function gv(cell) {
  let v=cell.value; if(!v) return '';
  v = (v.richText? v.richText.map(({ text }) => text).join(''): v.toString());
  return v.trim().toLowerCase();
}
 
async function ProcessSheet(sheet)
{
  let rows=sheet.rowCount, cols=sheet.columnCount;
  let found_first_row=false, no_col=0, addr_col=0, date_col=0, company_col=0, result=[];
  for(let row=1; row<=rows; row++)
  {
    console.log('row: ', row);
    let r=sheet.getRow(row);
    if(found_first_row)
    {
      let no_cell=r.getCell(no_col), no=gv(no_cell); if(!no){ for(let col=1; col<=cols; col++) r.getCell(col).style=style_black; continue; }
      let diff; if(diff=diffs.find(d=>d.no==no))
      {  
        let dcols=diff.cols;
        if(dcols.length==0) { for(let col=1; col<=cols; col++) r.getCell(col).style=style_black; no_cell.style=style_blue; continue;}
        for(let col=1; col<=cols; col++)
        {
          let cell=r.getCell(col); 
          if((company_col==col && dcols.includes('company'))||(addr_col==col && dcols.includes('addr'))||(date_col==col && dcols.includes('date'))) 
          { cell.style=style_red; }
          else cell.style=style_black;
        } 
      }
      else
      {  
        for(let col=1; col<=cols; col++) r.getCell(col).style=style_black;
      }
    }
    else
    {
      if(gv(r.getCell(1))=='区域' && gv(r.getCell(3))=='人员')
      { 
        // console.log('成功找到区域和人员列');
        for(let col=3; col<=cols; col++)
        {
          let cell=r.getCell(col); let v=gv(cell);
          if(v=='专卖许可证名称') company_col=col;     
          else if(v=='地址') addr_col=col;     
          else if(v=='许可证到期日期') date_col=col;     
          else if(v=='编号') no_col=col;
        }
        if(company_col==0 || addr_col==0 || date_col==0 || no_col==0) {  console.log('未找到指定的4列'); return;  }
        else { found_first_row=true; console.log('成功找到指定的4列'); }
      }
    }
  } 
}
 
 
async function ProcessFile(ExcelFilePath)
{
  const workbook = new ExcelJS.Workbook(); await workbook.xlsx.readFile(ExcelFilePath);
  let sheets=workbook.worksheets, sheetsCount=sheets.length;
  for(let sheet_index=0; sheet_index<sheetsCount; sheet_index++)
  {
    let sheet=sheets[sheet_index]; if(sheet.name==sheet_name) { await ProcessSheet(sheet); break; }
  }
  await workbook.xlsx.writeFile(ExcelFilePath);
} 
ProcessFile(dir+filename+'.xlsx');
`;
  fs.writeFileSync(FilePath, file_content);
}

async function traverseFolder(folderPath) {
  const items = fs.readdirSync(folderPath);
  for(let item of items)
  {
    console.log(item);
    if(item.toLowerCase().includes(".xlsx"))
    {
      const filename=item.substring(0, item.length-5); const ExcelFilePath = path.join(folderPath, item);      
      const workbook = new ExcelJS.Workbook(); await workbook.xlsx.readFile(ExcelFilePath);
      let sheets=workbook.worksheets, sheetsCount=sheets.length;
      for(let sheet_index=0; sheet_index<sheetsCount; sheet_index++)
      {
        let sheet=sheets[sheet_index]; console.log(`sheet ${sheet.id}: ${sheet.name}`); //await ProcessSheet(sheet);
        let rows=sheet.rowCount, cols=sheet.columnCount; console.log(`${rows} rows, ${cols} cols`);
        let found_first_row=false, no_col=0, addr_col=0, date_col=0, company_col=0, result=[];
        for(let row=1; row<=rows; row++)
        {
          let r=sheet.getRow(row);
          if(found_first_row)
          {
            let no=gv(r.getCell(no_col)); if(!no) continue;
            result.push({no, addr: gv(r.getCell(addr_col)), company: gv(r.getCell(company_col)), date: gv(r.getCell(date_col)), });
          }
          else
          {
            if(gv(r.getCell(1))=='区域' && gv(r.getCell(3))=='人员')
            { 
              //console.log('成功找到区域和人员列');
              for(let col=3; col<=cols; col++)
              {
                let cell=r.getCell(col); let v=gv(cell);
                if(v=='专卖许可证名称') company_col=col;     
                else if(v=='地址') addr_col=col;     
                else if(v=='许可证到期日期') date_col=col;     
                else if(v=='编号') no_col=col;               
                // console.log(`  [${row}, ${col}] ${v}`);
              }
              if(company_col==0 || addr_col==0 || date_col==0 || no_col==0) {  console.log('未找到指定的4列'); return;  }
              else { found_first_row=true; console.log('成功找到指定的4列'); }
            }
          }
        }
        //console.log(result.length, result[0]);
        try 
        {
          await make_console(filename, sheet.name, JSON.stringify(result)); await make_mark(filename, sheet.name);
          fs.writeFileSync(`${dir2}${filename}_${sheet.name}_差异.txt`, '');
        } catch (err) {   console.error(err);  }

      }        
      
    }
  }
}

traverseFolder(dir);

