const fs = require('fs');
const path = require('path');
const sharp = require('sharp');

const TargetImagesFolder="../vue/public/images";


function traverseFolder(folderPath, func) {
  const items = fs.readdirSync(folderPath);
  items.forEach(item => {    
    const itemPath = path.join(folderPath, item);
    const stats = fs.statSync(itemPath);
    if (stats.isDirectory()) {
      if(item=="small") return;
      traverseFolder(itemPath, func);
    } else if (stats.isFile()) {
      func(itemPath, folderPath, item);
    }
  });
}

function resize_image(p, folderPath, fileName){
  let small_folder=path.join(folderPath, 'small');
  if(!fs.existsSync(small_folder)){ fs.mkdirSync(small_folder); } else {
    const stats = fs.statSync(small_folder); 
    if (stats.isFile()) {
      console.log("cannot create small images folder, it's used by a file: ", small_folder);
      return;
    } 
  }  
  let small_path=path.join(small_folder, fileName); console.log(small_path);
  if(fs.existsSync(small_path)){ fs.unlinkSync(small_path); }
  sharp(p).resize(400).toFile(small_path, (err, info) => {  });
}
traverseFolder(TargetImagesFolder, resize_image);
