/**
 * @file       Utils.hpp
 * @author     Nicolas Pirard
 * @website    https://github.com/Anvently/
 * @modified   Wed Aug 06 2025
 * @license    Apache License 2.0
 */

#ifndef UTILS_HPP
# define UTILS_HPP
#include <chrono>

namespace Utils {
	template<typename DurationType>
	bool checkTimeout(const std::chrono::time_point<std::chrono::high_resolution_clock>& start_time,
							const DurationType& timeout) {
		auto now = std::chrono::high_resolution_clock::now();
		auto timeout_point = start_time + timeout;
		return now >= timeout_point;
	}

	void	printReceived(size_t nbytes, const char* received);
}

#endif
