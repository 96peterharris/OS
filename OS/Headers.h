#pragma once


#include "RAM.hpp"
#include "Virtual_Mem.hpp"
#include "Files_And_Directory_Management.hpp"
#include "CPU_Scheduling.hpp"

class Ram;
class Virtual_Mem;
class CPU_Scheduling;
class Filesystem;

class System {
public:
	static Ram RAM;
	static Virtual_Mem VM;
	static CPU_Scheduling CPU;
	static Filesystem FS;
	System();
};