#pragma once

#include <string>

// 「System::String^」⇒「std::string」へ。.NETのString^文字列を C++のstring文字列へ
std::string to_native_string(System::String^ str);

// 「std::string」⇒「System::String^」へ。C++のstring文字列を .NETのString^文字列へ
System::String^ to_managed_string(std::string str);

#pragma once
