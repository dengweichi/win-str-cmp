
let strCmp =  require('../lib/index.js');

console.log("start");

strCmp.strCompareAsc(["2","1"],function(array){
    console.log(array);
});

// strCmp.strCompareDesc(["2","1"],function(array){
//     console.log(array);
// });

console.log("end");