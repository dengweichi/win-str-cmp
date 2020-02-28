
const winStrCmp = require('../build/Release/winStrCmp');


function validArgs(array,callBack) {
    if(!Array.isArray(array) || array.length === 0|| typeof callBack !== 'function') {
        throw Error("参数错误");
    }

    for(let i = 0; i < array.length;++i) {
        if(typeof array[i] !== 'string') {
            throw Error(`参数错误,${array[i]}不是字符串`);
        }
    }
}

exports.strCompareAsc = function(array,callBack) {
    validArgs(array,callBack);
    winStrCmp.strCompareAsc(array,callBack);
}

exports.strCompareDesc = function(array,callBack) {
    validArgs(array,callBack);
    winStrCmp.strCompareDesc(array,callBack);
}