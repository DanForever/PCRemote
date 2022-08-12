#ifndef __DEBUG_PRINT_H__
#define __DEBUG_PRINT_H__

#ifndef DISABLE_DEBUG_PRINT
#	define DEBUG_PRINT(format, ...) ::Debug::Print(format, ##__VA_ARGS__)
#	define DEBUG_PRINTLN(format, ...) ::Debug::Println(format, ##__VA_ARGS__)
#else
#	define DEBUG_PRINT(format, ...)
#	define DEBUG_PRINTLN(format, ...)
#endif

#include <cstring>

namespace Debug
{
	inline size_t PrintInternal(const char*& format)
	{
		const char* argStart = std::strchr(format, '{');

		if (!argStart)
		{
			size_t count = Serial.print(format);
			format += count;
			return count;
		}

		size_t length = argStart - format;
		Serial.write(format, length);
		format += length;

		return length;
	}

	template <typename Arg>
	inline size_t PrintInternal(const char*& format, Arg&& arg)
	{
		const char* argEnd = std::strchr(format, '}');

		if (argEnd)
			format = argEnd + 1;

		return Serial.print(std::forward<Arg>(arg));
	}

	inline size_t Print(const char* text)
	{
		return Serial.print(text);
	}

	template <typename Arg>
	inline size_t Print(const char* format, Arg&& arg)
	{
		size_t charactersPrinted = 0;

		charactersPrinted += PrintInternal(format);
		charactersPrinted += PrintInternal(format, std::forward<Arg>(arg));
		charactersPrinted += PrintInternal(format);

		return charactersPrinted;
	}

	template <typename Arg, typename... Args>
	inline size_t Print(const char* format, Arg&& arg, Args... args)
	{
		size_t charactersPrinted = 0;

		charactersPrinted += Print(format, std::forward<Arg>(arg));
		charactersPrinted += Print(format, std::forward<Args>(args)...);

		return charactersPrinted;
	}

	template <typename... Args>
	inline size_t Print(const char* format, Args... args)
	{
		size_t charactersPrinted = 0;

		charactersPrinted += Print(format, std::forward<Args>(args)...);

		return charactersPrinted;
	}

	template <typename... Args>
	inline size_t Println(const char* format, Args... args)
	{
		size_t charactersPrinted = 0;

		charactersPrinted += Print(format, std::forward<Args>(args)...);
		charactersPrinted += Serial.println("");

		return charactersPrinted;
	}
}

#endif // __DEBUG_PRINT_H__
