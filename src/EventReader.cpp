// EventReader.cpp : main project file.
#include "stdafx.h"
#pragma comment(lib, "node")
#include <node.h>
#include <v8.h>
#include <msclr\marshal_cppstd.h>

using namespace v8;
using namespace System;
using namespace System::Diagnostics;
using namespace msclr::interop;



std::string managedStringToStdString( System::String ^s )
{
	if(s != nullptr){
		Console::WriteLine(s);
		System::Text::Encoding ^u8 = System::Text::Encoding::UTF8;
		array<unsigned char> ^bytes = u8->GetBytes( s );
		pin_ptr<unsigned char> pinnedPtr = &bytes[0];
		return std::string( (char*)pinnedPtr );
	}else{
		return "";
	}
}

Handle<Value> readLog(const Arguments& args) {
    HandleScope scope;
	Handle<v8::Object> returnObject=v8::Object::New();
	Handle<v8::Array> logs=v8::Array::New();
	returnObject->Set(v8::String::NewSymbol("entries"), logs);
	Handle<Value> count = args[1];

	Handle<Value> dateFrom = args[2];
	Handle<Value> dateTill = args[3];
	DateTime^ dateTimeFrom = nullptr;
	DateTime^ dateTimeTill = nullptr;

	if(dateFrom->IsNumber() && dateFrom->NumberValue()>0){
		dateTimeFrom = gcnew DateTime(1970, 1, 1);
		TimeSpan ts = TimeSpan::FromMilliseconds(dateFrom->NumberValue());
		dateTimeFrom=dateTimeFrom->Add(ts);
	}

	if(dateTill->IsNumber() && dateTill->NumberValue()>0){
		dateTimeTill = gcnew DateTime(1970, 1, 1);
		TimeSpan ts = TimeSpan::FromMilliseconds(dateTill->NumberValue());
		dateTimeTill=dateTimeTill->Add(ts);
	}
	Console::WriteLine(dateTimeFrom != nullptr);

	Handle<v8::String> v8LogName = args[0]->ToString();

	System::String^ logName = gcnew System::String(std::string(*v8::String::Utf8Value(v8LogName)).c_str());
	int i = 0;
	EventLog^ myLog = gcnew EventLog(logName);
	if (dateTimeFrom != nullptr)
		for (int k = 0; k < myLog->Entries->Count; k++){
			
			EventLogEntry^ myEntry = myLog->Entries[k];
			Handle<v8::Object> log = v8::Object::New();
			
			//log->Set(v8::String::NewSymbol("message"), v8::String::NewSymbol(marshal_as<std::string>(myEntry->Message).c_str()));

			log->Set(v8::String::NewSymbol("message"), v8::String::NewSymbol(managedStringToStdString(myEntry->Message).c_str()));

			log->Set(v8::String::NewSymbol("source"), v8::String::NewSymbol(managedStringToStdString(myEntry->Source).c_str()));
			log->Set(v8::String::NewSymbol("time"), v8::String::NewSymbol(managedStringToStdString(myEntry->TimeGenerated.ToString()).c_str()));
			log->Set(v8::String::NewSymbol("displayName"), v8::String::NewSymbol(managedStringToStdString(myLog->LogDisplayName).c_str()));
			log->Set(v8::String::NewSymbol("logName"), v8::String::NewSymbol(managedStringToStdString(myLog->Log).c_str()));
			log->Set(v8::String::NewSymbol("entries"), v8::Number::New(myLog->Entries->Count));
			Console::WriteLine("read" + myEntry->Category);

			log->Set(v8::String::NewSymbol("category"), v8::String::NewSymbol(managedStringToStdString(myEntry->Category).c_str()));
			log->Set(v8::String::NewSymbol("entryType"), v8::String::NewSymbol(managedStringToStdString(myEntry->EntryType.ToString()).c_str()));
			try{
				log->Set(v8::String::NewSymbol("userName"), v8::String::NewSymbol(managedStringToStdString(myEntry->UserName).c_str()));
				log->Set(v8::String::NewSymbol("machineName"), v8::String::NewSymbol(managedStringToStdString(myEntry->MachineName).c_str()));
			}
			catch (System::Exception^ se){
				Console::WriteLine(se->Message);
			}

			if (dateTimeFrom != nullptr)
			if (myEntry->TimeGenerated.CompareTo(dateTimeFrom)<0)
				continue;

			if (dateTimeTill != nullptr)
			if (myEntry->TimeGenerated.CompareTo(dateTimeTill)>0)
				continue;

			Console::WriteLine("setting log " + myEntry->Message);
			logs->Set(i++, log);


			if (count->IsInt32() && count->Int32Value() == i)
				break;
		}
	else
		for (int k = myLog->Entries->Count - 1; k > 0;k--){
			EventLogEntry^ myEntry = myLog->Entries[k];
			Handle<v8::Object> log = v8::Object::New();
			//log->Set(v8::String::NewSymbol("message"), v8::String::NewSymbol(marshal_as<std::string>(myEntry->Message).c_str()));

			log->Set(v8::String::NewSymbol("message"), v8::String::NewSymbol(managedStringToStdString(myEntry->Message).c_str()));

			log->Set(v8::String::NewSymbol("source"), v8::String::NewSymbol(managedStringToStdString(myEntry->Source).c_str()));
			log->Set(v8::String::NewSymbol("time"), v8::String::NewSymbol(managedStringToStdString(myEntry->TimeGenerated.ToString()).c_str()));
			log->Set(v8::String::NewSymbol("displayName"), v8::String::NewSymbol(managedStringToStdString(myLog->LogDisplayName).c_str()));
			log->Set(v8::String::NewSymbol("logName"), v8::String::NewSymbol(managedStringToStdString(myLog->Log).c_str()));
			log->Set(v8::String::NewSymbol("entries"), v8::Number::New(myLog->Entries->Count));

			log->Set(v8::String::NewSymbol("category"), v8::String::NewSymbol(managedStringToStdString(myEntry->Category).c_str()));
			log->Set(v8::String::NewSymbol("entryType"), v8::String::NewSymbol(managedStringToStdString(myEntry->EntryType.ToString()).c_str()));
			Console::WriteLine('u'+myEntry->UserName);
			try{
				log->Set(v8::String::NewSymbol("userName"), v8::String::NewSymbol(managedStringToStdString(myEntry->UserName).c_str()));
				log->Set(v8::String::NewSymbol("machineName"), v8::String::NewSymbol(managedStringToStdString(myEntry->MachineName).c_str()));
			}catch (System::Exception^ se){
				Console::WriteLine(se->Message);
			}
			if(dateTimeFrom != nullptr)
				if(myEntry->TimeGenerated.CompareTo(dateTimeFrom)<0)
					continue;

			if(dateTimeTill != nullptr)
				if(myEntry->TimeGenerated.CompareTo(dateTimeTill)>0)
					continue;

			logs->Set(i++, log);


			if (count->IsInt32() && count->Int32Value() == i)
				break;
		}

	return scope.Close(logs);
}


Handle<Value> getLogs(const Arguments& args) {
	HandleScope scope;
	Handle<v8::Array> logs=v8::Array::New();
	uint32_t i=0;
	try{
		for each (EventLog^ log in EventLog::GetEventLogs()){
			Handle<v8::Object> logObject = v8::Object::New();
			logObject->Set(v8::String::NewSymbol("displayName"), v8::String::NewSymbol(marshal_as<std::string>(log->LogDisplayName).c_str()));
			logObject->Set(v8::String::NewSymbol("logName"), v8::String::NewSymbol(marshal_as<std::string>(log->Log).c_str()));
			logObject->Set(v8::String::NewSymbol("entries"), v8::Integer::New(log->Entries->Count));
			logs->Set(i++, logObject);
		}
	}
	catch (Security::SecurityException^ se){
		Console::WriteLine(se->Message);
	}

	return scope.Close(logs);
}

void init (Handle<v8::Object> target) {
    HandleScope scope;
    target->Set(v8::String::NewSymbol("readLog"), FunctionTemplate::New(readLog)->GetFunction());
	target->Set(v8::String::NewSymbol("getLogs"), FunctionTemplate::New(getLogs)->GetFunction());

}
NODE_MODULE(EventReader, init);