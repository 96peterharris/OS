#pragma once

#include "RAM.hpp"
#include "Virtual_Mem.hpp"
#include "Files_And_Directory_Management.hpp"
#include "CPU_Scheduling.hpp"

class System {
public:
	static Ram RAM;
	static Virtual_Mem VM;
	static CPU_Scheduling CPU;
	static Filesystem FS;
};