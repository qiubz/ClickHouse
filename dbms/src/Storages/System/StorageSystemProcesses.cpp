#include <DB/Columns/ColumnString.h>
#include <DB/DataTypes/DataTypeString.h>
#include <DB/DataTypes/DataTypesNumberFixed.h>
#include <DB/DataStreams/OneBlockInputStream.h>
#include <DB/Interpreters/ProcessList.h>
#include <DB/Storages/System/StorageSystemProcesses.h>


namespace DB
{


StorageSystemProcesses::StorageSystemProcesses(const std::string & name_)
	: name(name_)
	, columns{
		{ "is_initial_query", 	std::make_shared<DataTypeUInt8>() },

		{ "user", 				std::make_shared<DataTypeString>() },
		{ "query_id", 			std::make_shared<DataTypeString>() },
		{ "address", 			std::make_shared<DataTypeString>() },
		{ "port", 				std::make_shared<DataTypeUInt16>() },

		{ "initial_user", 		std::make_shared<DataTypeString>() },
		{ "initial_query_id", 	std::make_shared<DataTypeString>() },
		{ "initial_address", 	std::make_shared<DataTypeString>() },
		{ "initial_port", 		std::make_shared<DataTypeUInt16>() },

		{ "interface",			std::make_shared<DataTypeUInt8>() },

		{ "os_user",				std::make_shared<DataTypeString>() },
		{ "client_hostname",		std::make_shared<DataTypeString>() },
		{ "client_name",			std::make_shared<DataTypeString>() },
		{ "client_version_major",	std::make_shared<DataTypeUInt64>() },
		{ "client_version_minor",	std::make_shared<DataTypeUInt64>() },
		{ "client_revision",		std::make_shared<DataTypeUInt64>() },

		{ "http_method",			std::make_shared<DataTypeUInt8>() },
		{ "http_user_agent",		std::make_shared<DataTypeString>() },

		{ "elapsed", 			std::make_shared<DataTypeFloat64>()	},
		{ "rows_read", 			std::make_shared<DataTypeUInt64>()	},
		{ "bytes_read",			std::make_shared<DataTypeUInt64>()	},
		{ "total_rows_approx",	std::make_shared<DataTypeUInt64>() },
		{ "memory_usage",		std::make_shared<DataTypeInt64>()	},
		{ "query", 				std::make_shared<DataTypeString>()	},
	}
{
}

StoragePtr StorageSystemProcesses::create(const std::string & name_)
{
	return make_shared(name_);
}


BlockInputStreams StorageSystemProcesses::read(
	const Names & column_names,
	ASTPtr query,
	const Context & context,
	const Settings & settings,
	QueryProcessingStage::Enum & processed_stage,
	const size_t max_block_size,
	const unsigned threads)
{
	check(column_names);
	processed_stage = QueryProcessingStage::FetchColumns;

	ProcessList::Info info = context.getProcessList().getInfo();

	Block block = getSampleBlock();

	for (const auto & process : info)
	{
		size_t i = 0;
		block.unsafeGetByPosition(i++).column->insert(UInt64(process.client_info.query_kind == ClientInfo::QueryKind::INITIAL_QUERY));
		block.unsafeGetByPosition(i++).column->insert(process.client_info.current_user);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.current_query_id);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.current_address.host().toString());
		block.unsafeGetByPosition(i++).column->insert(UInt64(process.client_info.current_address.port()));
		block.unsafeGetByPosition(i++).column->insert(process.client_info.initial_user);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.initial_query_id);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.initial_address.host().toString());
		block.unsafeGetByPosition(i++).column->insert(UInt64(process.client_info.initial_address.port()));
		block.unsafeGetByPosition(i++).column->insert(UInt64(process.client_info.interface));
		block.unsafeGetByPosition(i++).column->insert(process.client_info.os_user);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.client_hostname);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.client_name);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.client_version_major);
		block.unsafeGetByPosition(i++).column->insert(process.client_info.client_version_minor);
		block.unsafeGetByPosition(i++).column->insert(UInt64(process.client_info.client_revision));
		block.unsafeGetByPosition(i++).column->insert(UInt64(process.client_info.http_method));
		block.unsafeGetByPosition(i++).column->insert(process.client_info.http_user_agent);
		block.unsafeGetByPosition(i++).column->insert(process.elapsed_seconds);
		block.unsafeGetByPosition(i++).column->insert(process.rows);
		block.unsafeGetByPosition(i++).column->insert(process.bytes);
		block.unsafeGetByPosition(i++).column->insert(process.total_rows);
		block.unsafeGetByPosition(i++).column->insert(process.memory_usage);
		block.unsafeGetByPosition(i++).column->insert(process.query);
	}

	return BlockInputStreams(1, std::make_shared<OneBlockInputStream>(block));
}


}
