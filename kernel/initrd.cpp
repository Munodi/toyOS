#include <toyOS/initrd.hpp>
#include <toyOS/bootTerminal.hpp>

std::string TarHeader::getFileName() const
{
	//bootTerminal::prints("getFileName()\n");
	return std::string(fileNamePrefix).append(v7FileName);
}

std::uint64_t TarHeader::getSize() const
{
	std::uint64_t size = 0;
	
	for(std::size_t pos = 0; this->size[pos]; ++pos)
	{
		size = size * 8 + this->size[pos] - '0';
	}
	
	return size;
}

bool TarHeader::isEnd() const
{
	std::uint8_t* p = (std::uint8_t*)this;
	for(int i = 0; i < 1024; ++i)	// archive ends with two zero-filled records
		if(p[i] != '\0')
			return false;
	return true;	
}

const TarHeader* TarHeader::next() const
{
	std::uint64_t fileSize = this->getSize();
	TarHeader* next = (TarHeader*)((std::uint8_t*)this + 512 + ((fileSize + 511) & ~511));
	return (next->isEnd() ? nullptr : next);
}

std::size_t Initrd::getNumberOfFiles() const
{
	std::size_t count = 0;
	
	for(const TarHeader *h = (TarHeader*)module; h; h = h->next())
	{
		++count;
	}
	return count;
}

const TarHeader* Initrd::getFile(const std::string& name) const
{
	for(const TarHeader *h = (TarHeader*)module; h; h = h->next())
	{
		if(h->getFileName() == name)
			return h;
	}
	return nullptr;
}

bool Initrd::isFilePresent(const std::string& name) const
{
	for(const TarHeader *h = (TarHeader*)module; h; h = h->next())
	{
		if(h->getFileName() == name)
			return true;
	}
	return false;
}

std::size_t Initrd::getFileLength(const std::string& name) const
{
	const TarHeader* file = getFile(name);
	return (file ? file->getSize() : 0);
}

bool Initrd::copyFileToMemory(const std::string& name, void* ptr) const
{
	const TarHeader* tar = getFile(name);
	if(tar)
		std::memcpy(ptr, tar->getContent(), (std::size_t)tar->getSize());	// cast to suppress warning on 32 bit systems
	return tar != nullptr;
}

std::string Initrd::getName(std::size_t index) const
{
	std::size_t count = 0;
	
	for(const TarHeader *h = (TarHeader*)module; h; h = h->next())
	{
		if(index == count++)
			return h->getFileName();
	}
	return std::string("ERROR");
}

void Initrd::debug() const
{
	bootTerminal::prints("address: ");
	bootTerminal::print10((std::uint32_t)module);
	bootTerminal::prints("\ncmdline: ");
	bootTerminal::prints(string);
	bootTerminal::prints("\nfiles in initrd:\n");
	for(std::size_t i = 0; i < this->getNumberOfFiles(); ++i)
	{
		bootTerminal::prints("file name: ");
		bootTerminal::prints(this->getName(i).c_str());
		bootTerminal::prints(", file size: ");
		bootTerminal::print10(this->getFileLength(this->getName(i)));
		bootTerminal::printc('\n');
	}
}
