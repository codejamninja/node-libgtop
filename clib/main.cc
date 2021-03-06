#include <glibtop/netlist.h>
#include <glibtop/netload.h>
#include <glibtop/procargs.h>
#include <glibtop/proclist.h>
#include <glibtop/uptime.h>
#include <iostream>
#include <nan.h>
#include <v8.h>

void GetProclist(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Array> v8Pids = Nan::New<v8::Array>();
  glibtop_proclist proclist;
  pid_t* proclistResult;
  proclistResult = glibtop_get_proclist(&proclist, 0, 1);
  for (unsigned int i = 0; i < proclist.number; i++) {
    v8Pids->Set(i, Nan::New<v8::Number>(proclistResult[i]));
  }
  info.GetReturnValue().Set(v8Pids);
  g_free(proclistResult);
}

void GetNetload(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  glibtop_netload netload;
  v8::Local<v8::Object> v8Netload = Nan::New<v8::Object>();
  if (info.Length() < 1) {
    return;
  } else if (info[0]->IsNull()) {
    return;
  } else if (info[0]->IsUndefined()) {
    return;
  }
  Nan::Utf8String v8Str(info[0]);
  std::string cStr(*v8Str);
  const char* interface = cStr.c_str();
  glibtop_get_netload(&netload, interface);
  Nan::Set(v8Netload, Nan::New("address").ToLocalChecked(), Nan::New<v8::Number>(netload.address));
  Nan::Set(v8Netload, Nan::New("address6").ToLocalChecked(), Nan::New<v8::Number>(netload.address6[16]));
  Nan::Set(v8Netload, Nan::New("bytesIn").ToLocalChecked(), Nan::New<v8::Number>(netload.bytes_in));
  Nan::Set(v8Netload, Nan::New("bytesOut").ToLocalChecked(), Nan::New<v8::Number>(netload.bytes_out));
  Nan::Set(v8Netload, Nan::New("bytesTotal").ToLocalChecked(), Nan::New<v8::Number>(netload.bytes_total));
  Nan::Set(v8Netload, Nan::New("collisions").ToLocalChecked(), Nan::New<v8::Number>(netload.collisions));
  Nan::Set(v8Netload, Nan::New("errorsIn").ToLocalChecked(), Nan::New<v8::Number>(netload.errors_in));
  Nan::Set(v8Netload, Nan::New("errorsOut").ToLocalChecked(), Nan::New<v8::Number>(netload.errors_out));
  Nan::Set(v8Netload, Nan::New("errorsTotal").ToLocalChecked(), Nan::New<v8::Number>(netload.errors_total));
  Nan::Set(v8Netload, Nan::New("hwaddress").ToLocalChecked(), Nan::New<v8::Number>(netload.hwaddress[8]));
  Nan::Set(v8Netload, Nan::New("mtu").ToLocalChecked(), Nan::New<v8::Number>(netload.mtu));
  Nan::Set(v8Netload, Nan::New("packetsIn").ToLocalChecked(), Nan::New<v8::Number>(netload.packets_in));
  Nan::Set(v8Netload, Nan::New("packetsOut").ToLocalChecked(), Nan::New<v8::Number>(netload.packets_out));
  Nan::Set(v8Netload, Nan::New("packetsTotal").ToLocalChecked(), Nan::New<v8::Number>(netload.packets_total));
  Nan::Set(v8Netload, Nan::New("prefix6").ToLocalChecked(), Nan::New<v8::Number>(netload.prefix6[16]));
  Nan::Set(v8Netload, Nan::New("scope6").ToLocalChecked(), Nan::New<v8::Number>(netload.scope6));
  Nan::Set(v8Netload, Nan::New("subnet").ToLocalChecked(), Nan::New<v8::Number>(netload.subnet));
  info.GetReturnValue().Set(v8Netload);
}

void GetUptime(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> v8Uptime = Nan::New<v8::Object>();
  glibtop_uptime uptime;
  glibtop_get_uptime(&uptime);
  Nan::Set(v8Uptime, Nan::New("bootTime").ToLocalChecked(), Nan::New<v8::Number>(uptime.boot_time));
  Nan::Set(v8Uptime, Nan::New("idletime").ToLocalChecked(), Nan::New<v8::Number>(uptime.idletime));
  Nan::Set(v8Uptime, Nan::New("uptime").ToLocalChecked(), Nan::New<v8::Number>(uptime.uptime));
  info.GetReturnValue().Set(v8Uptime);
}

void GetNetlist(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Array> v8Netlist = Nan::New<v8::Array>();
  glibtop_netlist netlist;
  char** netlistResult;
  netlistResult = glibtop_get_netlist(&netlist);
  for (unsigned int i = 0; i < netlist.number; i++) {
    v8Netlist->Set(i, Nan::New(netlistResult[i]).ToLocalChecked());
  }
  info.GetReturnValue().Set(v8Netlist);
}

void GetProcArgs(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Array> v8ProcArgs = Nan::New<v8::Array>();
  glibtop_proc_args procArgs;
  char* procArgsResult;
  if (info.Length() < 1) {
    return;
  } else if (info[0]->IsNull()) {
    return;
  } else if (info[0]->IsUndefined()) {
    return;
  } else if (!info[0]->IsInt32()) {
    return;
  }
  int32_t pid = Nan::To<int32_t>(info[0]).FromMaybe(0);
  procArgsResult = glibtop_get_proc_args(&procArgs, pid, 0);
  info.GetReturnValue().Set(Nan::New(procArgsResult).ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("getProclist").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetProclist)->GetFunction(context).ToLocalChecked());
  exports->Set(context,
               Nan::New("getNetload").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetNetload)->GetFunction(context).ToLocalChecked());
  exports->Set(context,
               Nan::New("getUptime").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetUptime)->GetFunction(context).ToLocalChecked());
  exports->Set(context,
               Nan::New("getNetlist").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetNetlist)->GetFunction(context).ToLocalChecked());
  exports->Set(context,
               Nan::New("getProcArgs").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetProcArgs)->GetFunction(context).ToLocalChecked());
}

NODE_MODULE(gtop, Init)
