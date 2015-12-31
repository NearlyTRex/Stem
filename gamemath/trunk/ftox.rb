#!/usr/bin/ruby

def ftox(f)
	"0x" + (f * 0x10000).to_i.to_s(16).upcase.rjust(5, "0")
end
