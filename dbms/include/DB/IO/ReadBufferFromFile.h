#pragma once

#include <DB/IO/ReadBufferFromFileDescriptor.h>
#include <DB/Common/CurrentMetrics.h>


namespace CurrentMetrics
{
	extern const Metric OpenFileForRead;
}

namespace DB
{

/** Accepts path to file and opens it, or pre-opened file descriptor.
  * Closes file by himself (thus "owns" a file descriptor).
  */
class ReadBufferFromFile : public ReadBufferFromFileDescriptor
{
private:
	std::string file_name;
	CurrentMetrics::Increment metric_increment{CurrentMetrics::OpenFileForRead};

public:
	ReadBufferFromFile(const std::string & file_name_, size_t buf_size = DBMS_DEFAULT_BUFFER_SIZE, int flags = -1,
		char * existing_memory = nullptr, size_t alignment = 0);

	/// Use pre-opened file descriptor.
	ReadBufferFromFile(int fd, size_t buf_size = DBMS_DEFAULT_BUFFER_SIZE, int flags = -1,
		char * existing_memory = nullptr, size_t alignment = 0);

	~ReadBufferFromFile() override;

	/// Close file before destruction of object.
	void close();

	std::string getFileName() const override
	{
		return file_name;
	}
};

}
