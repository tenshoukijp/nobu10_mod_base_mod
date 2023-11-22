#include <string>
#include <msclr/marshal_cppstd.h>

System::String^ string_to_String(std::string str) {
	System::String^ managed_string = msclr::interop::marshal_as<System::String^>(str);
	return managed_string;
}

std::string String_to_string(System::String^ str) {
	std::string native_string = msclr::interop::marshal_as<std::string>(str);
	return native_string;
}

