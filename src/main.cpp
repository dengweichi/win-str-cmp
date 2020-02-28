#include <node.h>
#include<uv.h>
#include<shlwapi.h>
#include<vector>  
#include <algorithm> 

typedef struct {
	v8::Local<v8::Array> array;
	v8::Local<v8::Function> callBack;
	v8::Isolate* isolate;
	compareType type;
} compareData;


void workCallBack(uv_work_t* req) {
	compareData* data = static_cast<compareData*>(req->data);
	int len = data->array->Length();
	
	std::vector<std::wstring> vec;
	
	for (size_t i = 0; i < len; i++)
	{
		v8::String::Utf8Value utf8_value(data->isolate, data->array->Get(i));
		std::string str(*utf8_value);
		std::wstring wstr;
		wstr.assign(str.begin(), str.end());
		vec.push_back(wstr);
	}
	std::sort(vec.begin(), vec.end(), [=](std::wstring* front, std::wstring* next) {
		int result = StrCmpLogicalW(front->data(), next->data());
		return  data->type == compareType::ASC ? result < 0 : result > 0;
		});
	v8::Local<v8::Array> array;
	for (size_t i = 0; i < len; i++)
	{
		std::wstring wstr = vec[i];
		std::string str;
		str.assign(wstr.begin(), wstr.end());

		array->Set(i, v8::String::NewFromUtf8(data->isolate, str.c_str()));
	}
	v8::Local<v8::Value> args[1];
	args[0] = array;
	data->callBack->Call(v8::Null(data->isolate),1,args);
}

void afterWorkCallBack(uv_work_t* req, int status) {
	compareData* data = static_cast<compareData*>(req->data);
	delete data;
	delete req;
}

enum compareType {
	DESC,
	ASC
};

void compare(const v8::FunctionCallbackInfo<v8::Value>& args, compareType type) {
	compareData* data = new compareData();
	data->array = args[0].As<v8::Array>();
	data->callBack = args[1].As<v8::Function>();
	data->isolate = args.GetIsolate();
	data->type = type;
	uv_work_t *req = new uv_work_t();
	req->data = static_cast<void*>(data);
	uv_queue_work(uv_default_loop(), req, workCallBack, afterWorkCallBack);
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
