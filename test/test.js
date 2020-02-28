
let strCmp =  require('../src/index.js');

strCmp.strCompareAsc(["2","1"],function(array){
    console.log(array);
});

strCmp.strCompareDesc(["2","1"],function(array){
    console.log(array);
});