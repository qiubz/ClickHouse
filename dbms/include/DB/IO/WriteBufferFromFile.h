#pragma once

#include <sys/types.h>

#include <DB/Common/CurrentMetrics.h>
#include <DB/IO/WriteBufferFromFileDescriptor.h>


namespace CurrentMetrics
{
	extern const Metric OpenFileForWrite;
}


namespace DB
{

/** Accepts path to file and opens it, or pre-opened file descriptor.
  * Closes file by himself (thus "owns" a file descriptor).
  */
class WriteBufferFromFile : public WriteBufferFromFileDescriptor
{
private:
	std::string file_name;
	CurrentMetrics::Increment metric_increment{CurrentMetrics::OpenFileForWrite};

public:
	WriteBufferFromFile(
		const std::string & file_name_,
		size_t buf_size = DBMS_DEFAULT_BUFFER_SIZE,
		int flags = -1,
		mode_t mode = 0666,
		char * existing_memory = nullptr,
		size_t alignment = 0);

	/// Use pre-opened file descriptor.
	WriteBufferFromFile(
		int fd,
		size_t buf_size = DBMS_DEFAULT_BUFFER_SIZE,
		int flags = -1,
		mode_t mode = 0666,
		char * existing_memory = nullptr,
		size_t alignment = 0);

	~WriteBufferFromFile() override;

	/// Close file before destruction of object.
	void close();

	std::string getFileName() const override
	{
		return file_name;
	}
};

}
