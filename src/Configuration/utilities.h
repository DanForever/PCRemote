/*
* PC Remote
* Firmware for an IOT-device that can remotely press the power switch
* of an ATX style PC as well as monitor its current power state (on/off)
* https://github.com/DanForever/PCRemote
* Copyright (C) 2022 Daniel Neve
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __CONFIGURATION_UTILITIES_H__
#define __CONFIGURATION_UTILITIES_H__

#include <utility>
#include <string.h>

namespace Utility
{
	template <typename SourceType, typename DestType, typename Arg>
	inline void ReadInternal(const SourceType& source, DestType& destination, Arg arg)
	{
		const char* value = source[arg];
		destination = value;
	}

	template <typename SourceType, typename DestType, typename Arg, typename... Args>
	inline void ReadInternal(const SourceType& source, DestType& destination, Arg arg, Args... args)
	{
		auto nextSource = source[arg];
		ReadInternal(nextSource, destination, std::forward<Args>(args)...);
	}

	template <typename SourceType, typename DestType, typename... Args>
	inline void Read(const SourceType& source, DestType& destination, Args... args)
	{
		ReadInternal(source, destination, std::forward<Args>(args)...);
	}

	template <typename SourceType, typename DestType, typename Arg>
	inline void ReadDefaultInternal(const SourceType& source, DestType& destination, const DestType& defaultValue, Arg arg)
	{
		const JsonVariantConst& jsonVariant = source[arg];

		destination = jsonVariant.isNull() ? defaultValue : jsonVariant;
	}

	template <typename SourceType, typename DestType, typename Arg, typename... Args>
	inline void ReadDefaultInternal(const SourceType& source, DestType& destination, const DestType& defaultValue, Arg arg, Args... args)
	{
		auto nextSource = source[arg];
		ReadDefaultInternal(nextSource, destination, defaultValue, std::forward<Args>(args)...);
	}

	template <typename SourceType, typename DestType, typename... Args>
	inline void ReadDefault(const SourceType& source, DestType& destination, const DestType& defaultValue, Args... args)
	{
		ReadDefaultInternal(source, destination, defaultValue, std::forward<Args>(args)...);
	}

	template <typename DestType, typename SourceType, typename Arg>
	inline void WriteInternal(DestType& destination, SourceType& source, Arg&& arg)
	{
		destination[arg] = source;
	}

	template <typename DestType, typename SourceType, typename Arg, typename... Args>
	inline void WriteInternal(DestType& destination, SourceType& source, Arg&& arg, Args... args)
	{
		auto next = destination[arg];
		WriteInternal(next, source, std::forward<Args>(args)...);
	}

	template <typename DestType, typename SourceType, typename... Args>
	inline void Write(DestType& destination, SourceType& source, Args... args)
	{
		WriteInternal(destination, source, std::forward<Args>(args)...);
	}
}

#endif // __CONFIGURATION_UTILITIES_H__
