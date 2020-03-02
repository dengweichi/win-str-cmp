#include <node.h>
#include<uv.h>
#include<shlwapi.h>
#include<vector>  
#include <algorithm> 
#include<string>
#include <iostream>


enum compareType {
	DESC,
	ASC
};

void compare(const v8::FunctionCallbackInfo<v8::Value>& args, compareType type) {

	v8::Isolate* isolate = args.GetIsolate();
	v8::Local<v8::Array> array = args[0].As<v8::Array>();
	v8::Local<v8::Function> callBack = args[1].As<v8::Function>();

	HINSTANCE hDLL = LoadLibrary("shlwapi.dll");

	if (hDLL == 0)
	{
		return;
	}

	int (*strCompare)(PCWSTR, PCWSTR);

	void* fun= GetProcAddress(hDLL,"StrCmpLogicalW");


	strCompare = (int (*)(PCWSTR, PCWSTR))fun;



	size_t len = array->Length();

	std::vector<std::string> vec;

	for (size_t i = 0; i < len; i++)
	{
		v8::String::Utf8Value utf8_value(isolate, array->Get(i));
		std::string str(*utf8_value);
		vec.push_back(str);
	}

	std::sort(vec.begin(), vec.end(), [=](std::string& front_str, std::string& next_str) {
		std::wstring front_wstr;
		front_wstr.assign(front_str.begin(), front_str.end());
		std::wstring next_wstr;
		next_wstr.assign(next_str.begin(), next_str.end());

		int result = strCompare(front_wstr.data(), next_wstr.data());
		return  type == compareType::ASC ? result < 0 : result > 0;
		});

	 
	for (size_t i = 0; i < len; i++)
	{
		array->Set(i, v8::String::NewFromUtf8(isolate, vec[i].c_str()));
	}

	const int argc = 1;
	v8::Local<v8::Value> argv[argc] = {array};
	callBack->Call(v8::Null(isolate), 1, argv);
	FreeLibrary(hDLL);
}

void DESCMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
	compare(args, compareType::DESC);
}

void ASCMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
	compare(args, compareType::ASC);
}

void Initialize(v8::Local<v8::Object> exports) {
	NODE_SET_METHOD(exports, "strCompareDesc", DESCMethod);
	NODE_SET_METHOD(exports, "strCompareAsc", ASCMethod);
}


NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
