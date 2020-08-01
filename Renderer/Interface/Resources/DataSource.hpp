#pragma once


namespace Renderer
{
	struct DataSource
	{
		const void *data;
		size_t sizeInBytes;

		DataSource(const void *data, size_t sizeInBytes) : data{ data }, sizeInBytes{ sizeInBytes } {}
		
	};
	
	
}