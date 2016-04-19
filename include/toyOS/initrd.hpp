/*
 a boot module containing servers, drivers and configuration information
 stored as a tar file
*/
#pragma once

#include <cstddef>
#include <string>
#include <toyOS/multiboot2.hpp>

class TarHeader
{
	public:
		TarHeader(const TarHeader&) = delete;
		TarHeader& operator=(TarHeader const&) = delete;
		std::string getFileName() const;
		std::uint64_t getSize() const;
		const void* getContent() const { return _content; }
		bool isFile() const { return (typeFlag == '0' or typeFlag == '7'); }
		const TarHeader* next() const;	// returns the next file in tar or null
		bool isEnd() const;
	private:
		char v7FileName[100];
		char mode[8];
		char uid[8];
		char gid[8];
		char size[12];
		char mTime[12];
		char checksum[8];
		char typeFlag;
		char linkedFileName[100];
		char ustarIndicator[6];
		char ustarVersion[2];
		char ownerName[32];
		char groupName[32];
		char major[8];
		char minor[8];
		char fileNamePrefix[155];
		char padding[12];
		unsigned char _content[];
};

class Initrd
{
	public:
		Initrd() = default;
		Initrd(void* modStart, std::uint32_t length, const char* string)
			: initialised(true),
			module(modStart),
			length(length),
			string(string) {}
		//Initrd(const Initrd&) = delete;
		//Initrd& operator=(Initrd const&) = delete;
		const TarHeader* getFile(const std::string& name) const;
		bool isFilePresent(const std::string& name) const;
		std::size_t getFileLength(const std::string& name) const;
		bool copyFileToMemory(const std::string& name, void* ptr) const;
		std::size_t getNumberOfFiles() const;
		std::string getName(std::size_t index) const;
		// note: I'm not sure this class should really have all these functions, maybe just copyFileToMemory or getFile and getFileLength
		void debug() const;
	private:
		bool initialised = false;
		const void* module = nullptr;
		std::size_t length = 0;
		const char* string = nullptr;
};
