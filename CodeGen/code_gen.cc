#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
typedef std::pair<std::string, std::string> Arg;

std::string get_filename(const char* filename)
{
	std::string file(filename);
	for (int i = file.size() - 1; i >= 0; --i)
	{
		if (file[i] == '/')
		{
			return std::string(file, i + 1, file.size() - i - 1);
		}
	}
	return filename;
}
std::string get_proto_filename(const char* src_file, const char* dst_dir)
{
	std::string file(dst_dir);
	file += "/";
	file += get_filename(src_file);
	file += ".proto";
	return file;
}
std::string get_client_cc_filename(const char* src_file, const char* dst_dir)
{
	std::string file(dst_dir);
	file += "/";
	file += get_filename(src_file);
	file += ".client.cc";
	return file;
}
std::string get_server_cc_filename(const char* src_file, const char* dst_dir)
{
	std::string file(dst_dir);
	file += "/";
	file += get_filename(src_file);
	file += ".server.cc";
	return file;
}
std::string get_makefile_filename(const char* dst_dir)
{
	std::string file(dst_dir);
	file += "/";
	file += "Makefile";
	return file;
}

bool parse_func_def(const std::string& func,
	Arg& return_arg, std::string& name,
	std::vector<Arg>& func_args)
{
	std::regex regex("[\\s]*([a-zA-Z]+)[\\s]+([a-zA-Z]+)[\\s]*[\(](.*)[\)]");
	std::smatch result;
	std::string params;

	if (std::regex_search(func.cbegin(), func.cend(), result, regex))
	{
		if (result.size() != 4)
			return false;
		return_arg.first = result.str(1);
		return_arg.second = "result";
		name = result.str(2);
		params = result.str(3);
	}
	else
	{
		return false;
	}

	std::regex regex2("([a-zA-Z]+)[\\s]+([a-zA-Z]+)");
	while (std::regex_search(params.cbegin(), params.cend(), result, regex2))
	{
		if (result.size() != 3)
			return false;
		Arg arg;
		arg.first = result.str(1);
		arg.second = result.str(2);
		func_args.push_back(arg);
		params = std::string(result.suffix().first, result.suffix().second);
	}
	return true;
}

std::string get_proto_type(const std::string& type)
{
	if (type == "int")
		return "int32";
	else if (type == "long")
		return "int32";
	else if (type == "long long")
		return "int64";
	else
		return type;
}

void write_proto(const std::string& filename,
	const std::vector<Arg>& return_args,
	const std::vector<std::string>& names,
	const std::vector<std::vector<Arg>>& func_args_list)
{
	std::ofstream proto_fd(filename);
	if (!proto_fd.is_open())
		return;

	proto_fd << "syntax = \"proto3\";" << std::endl;
	proto_fd << "package RPC;" << std::endl << std::endl;
	for (int i = 0; i < return_args.size(); ++i)
	{
		proto_fd << "message RPC_" << names[i] << "_request{" << std::endl;
		proto_fd << "string method = 1;" << std::endl;
		proto_fd << "int32 query_id = 2;" << std::endl;
		for (int j = 0; j < func_args_list[i].size(); ++j)
		{
			proto_fd << get_proto_type(func_args_list[i][j].first) << " " 
				<< func_args_list[i][j].second << " = " 
				<< std::to_string(j + 3) << ";" << std::endl;
		}
		proto_fd << "}" << std::endl << std::endl << "message RPC_" << names[i] << "_response{" << std::endl;
		proto_fd << "string method = 1;" << std::endl;
		proto_fd << "int32 query_id = 2;" << std::endl;
		proto_fd << "int32 state = 3;" << std::endl;
		proto_fd << get_proto_type(return_args[i].first) << " "
			<< return_args[i].second << " = 4;" << std::endl << "}" << std::endl << std::endl;
	}
	proto_fd.close();
}

void write_server_cc(const std::string& filename,
	const std::string& head_file,
	const std::string& proto_head_file,
	const std::vector<Arg>& return_args,
	const std::vector<std::string>& names,
	const std::vector<std::vector<Arg>>& func_args_list)
{
	std::ofstream server_cc_fd(filename);
	if (!server_cc_fd.is_open())
		return ;

	server_cc_fd << "#include \"RPC_server_template.h\"" << std::endl;
	server_cc_fd << "#include \"" << proto_head_file << ".pb.h\"" << std::endl;
	server_cc_fd << "#include \"" << head_file << "\"" << std::endl << std::endl;

	for (int i = 0; i < return_args.size(); ++i)
	{
		server_cc_fd << "RPC_SERVER" << std::to_string(func_args_list[i].size()) << "("
			<< return_args[i].second << ", " << names[i];
		for (int j = 0; j < func_args_list[i].size(); ++j)
		{
			server_cc_fd << ", " << func_args_list[i][j].second;
		}
		server_cc_fd << ");" << std::endl;
	}
	server_cc_fd.close();
}

void write_client_cc(const std::string& filename,
	const std::string& head_file,
	const std::string& proto_head_file,
	const std::vector<Arg>& return_args,
	const std::vector<std::string>& names,
	const std::vector<std::vector<Arg>>& func_args_list)
{
	std::ofstream client_cc_fd(filename);
	if (!client_cc_fd.is_open())
		return;

	client_cc_fd << "#include \"RPC_client_template.h\"" << std::endl;
	client_cc_fd << "#include \"" << proto_head_file << ".pb.h\"" << std::endl;
	client_cc_fd << "#include \"" << head_file << "\"" << std::endl << std::endl;

	for (int i = 0; i < return_args.size(); ++i)
	{
		client_cc_fd << return_args[i].first << " " << names[i] << "(";
		for (int j = 0; j < func_args_list[i].size(); ++j)
		{
			client_cc_fd << func_args_list[i][j].first << " " << func_args_list[i][j].second;
			if (j < func_args_list[i].size() - 1)
				client_cc_fd << ", ";
		}
		client_cc_fd << ")" << std::endl << "{" << std::endl;

		client_cc_fd << "RPC_CLIENT" << std::to_string(func_args_list[i].size()) << "("
			<< return_args[i].second << ", " << names[i];
		for (int j = 0; j < func_args_list[i].size(); ++j)
		{
			client_cc_fd << ", " << func_args_list[i][j].second;
		}
		client_cc_fd << ");" << std::endl << "}" << std::endl << std::endl;
	}
	client_cc_fd.close();
}

void write_makefile(const std::string& filename, 
	const std::string& proto_head_file,
	const std::string& rpc_path)
{
	std::ofstream makefile_fd(filename);
	if (!makefile_fd.is_open())
		return;

	makefile_fd << "CC=g++" << std::endl;
	makefile_fd << "CFLAGS=-std=c++11 -g" << std::endl;
	makefile_fd << "PROTOBUF_PATH=" << rpc_path << "/protobuf" << std::endl;
	makefile_fd << "PROTOC=$(PROTOBUF_PATH)/bin/protoc" << std::endl;
	makefile_fd << "INCLUDE_DIR=$(PROTOBUF_PATH)/include" << std::endl;
	makefile_fd << "LIB_DIR=$(PROTOBUF_PATH)/lib" << std::endl;
	makefile_fd << "RPC_CLIENT_INCLUDE_DIR=" << rpc_path << "/RPC_client/include" << std::endl;
	makefile_fd << "RPC_CLIENT_LIB_DIR=" << rpc_path << "/RPC_client/lib" << std::endl;
	makefile_fd << "RPC_SERVER_INCLUDE_DIR=" << rpc_path << "/RPC_server/include" << std::endl;
	makefile_fd << "RPC_SERVER_LIB_DIR=" << rpc_path << "/RPC_server/lib" << std::endl;
	makefile_fd << "" << std::endl;
	makefile_fd << "all://TODO add your target" << std::endl;
	makefile_fd << "" << std::endl;
	makefile_fd << proto_head_file << ".pb.o:" << proto_head_file << ".pb.cc" << std::endl;
	makefile_fd << "\t$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $^" << std::endl;
	makefile_fd << "" << std::endl;
	makefile_fd << proto_head_file << ".client.o:" << proto_head_file << ".client.cc" << std::endl;
	makefile_fd << "\t$(CC) $(CFLAGS) -I$(RPC_CLIENT_INCLUDE_DIR) -I$(INCLUDE_DIR) -c $^" << std::endl;
	makefile_fd << "" << std::endl;
	makefile_fd << proto_head_file << ".server.o:" << proto_head_file << ".server.cc" << std::endl;
	makefile_fd << "\t$(CC) $(CFLAGS) -I$(RPC_SERVER_INCLUDE_DIR) -I$(INCLUDE_DIR) -c $^" << std::endl;
	makefile_fd << "" << std::endl;
	makefile_fd << proto_head_file << ".pb.cc :" << std::endl;
	makefile_fd << "\t$(PROTOC) " <<proto_head_file<<".proto --cpp_out=./" << std::endl;
	makefile_fd << "" << std::endl;
	makefile_fd << "clean :" << std::endl;
	makefile_fd << "\trm -f *.o " << proto_head_file << ".pb.*" << std::endl;
	
	makefile_fd.close();
}

void print_help()
{
	std::cout<<"Usage:code_gen rpc_def_file dst_dir"<<std::endl;
}
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		print_help();
		return 0;
	}
	char* src_file = argv[1];
	char* dst_dir = argv[2];

	std::ifstream rpc_def_fd(src_file);
	if (!rpc_def_fd.is_open())
		return 0;

	std::string head_file;
	std::getline(rpc_def_fd, head_file);
	std::vector<Arg> return_args;
	std::vector<std::string> names;
	std::vector<std::vector<Arg>> func_args_list;

	while (!rpc_def_fd.eof())
	{
		std::string func;
		std::getline(rpc_def_fd, func);
		if (func.empty())
			continue;
		Arg return_arg;
		std::string name;
		std::vector<Arg> func_args;
		if (parse_func_def(func, return_arg, name, func_args))
		{
			return_args.emplace_back(std::move(return_arg));
			names.emplace_back(std::move(name));
			func_args_list.emplace_back(std::move(func_args));
		}
		else
		{
			std::cout << "find error around:" << func << std::endl;
			return 0;
		}
	}

	write_proto(get_proto_filename(src_file, dst_dir),
		return_args, names, func_args_list);

	write_server_cc(get_server_cc_filename(src_file, dst_dir),
		head_file,
		get_filename(src_file),
		return_args, names, func_args_list);

	write_client_cc(get_client_cc_filename(src_file, dst_dir),
		head_file,
		get_filename(src_file),
		return_args, names, func_args_list);

	//write_makefile(get_makefile_filename(dst_dir),
	//	get_filename(src_file),
	//	rpc_path);
	return 0;
}
