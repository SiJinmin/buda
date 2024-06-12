// save this content as excel.js file
// # npm i exceljs
// # node excel.js

const ExcelJS = require('exceljs');


async function ProcessSheet(sheet)
{
  let rows=sheet.rowCount, cols=sheet.columnCount;
  console.log(`  ${rows} rows, ${cols} cols`);
  for(let row=1; row<=rows; row++)
  {
    let r=sheet.getRow(row);
    if(row==1) { if(!r.font) r.font={};  r.font.color={ argb: 'FFFF0000'};  }
    for(let col=1; col<=cols; col++)
    {
      let cell=r.getCell(col); let v=cell.value;
      console.log(`  [${row}, ${col}] ${v}`);
    }
  }  
}


async function ProcessFile(ExcelFilePath)
{
  const workbook = new ExcelJS.Workbook();
  await workbook.xlsx.readFile(ExcelFilePath);
  let sheets=workbook.worksheets, sheetsCount=sheets.length;
  for(let sheet_index=0; sheet_index<sheetsCount; sheet_index++)
  {
    let sheet=sheets[sheet_index];    
    console.log(`sheet ${sheet.id}: ${sheet.name}`);
    await ProcessSheet(sheet);
  }
  
  // Iterate over all sheets
  // Note: workbook.worksheets.forEach will still work but this is better
  //--workbook.eachSheet(function(worksheet, sheetId) {     });

  // fetch sheet by name
  //--const worksheet2 = workbook.getWorksheet('My Sheet');

  // fetch sheet by id
  //--const worksheet3 = workbook.getWorksheet(1);


  await workbook.xlsx.writeFile(ExcelFilePath);
}


function CreateFile()
{
  const workbook = new ExcelJS.Workbook();

  workbook.creator = 'Me';
  workbook.lastModifiedBy = 'Her';
  workbook.created = new Date(1985, 8, 30);
  workbook.modified = new Date();
  workbook.lastPrinted = new Date(2016, 9, 27);
  
  const sheet = workbook.addWorksheet('My Sheet');
  workbook.removeWorksheet(sheet.id);
}

ProcessFile("C:\\Users\\Qiuzen\\Desktop\\艺术快递.xlsx");

