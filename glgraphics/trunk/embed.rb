#!/usr/bin/ruby

name = ARGV[0]
input_file = ARGV[1]
output_file = ARGV[2]

input_bytes = ""
File.open(input_file, 'r') do |file|
	input_bytes = file.read()
end

def escape_c_string(bytes)
	return bytes.gsub("\\", "\\\\").gsub("\"", "\\\"").gsub("\n", "\\n\"\n\"")
end

File.open(output_file, 'w') do |file|
	file.puts("#ifndef __#{name}__")
	file.puts("#define __#{name}__")
	file.puts("")
	file.puts("static const char STATIC_#{name}[] = \"#{escape_c_string(input_bytes)}\";")
	file.puts("")
	file.puts("#endif")
end
