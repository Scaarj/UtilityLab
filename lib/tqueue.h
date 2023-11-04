#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

#include "TQueue_global.h"

namespace utl {
/// \brief Threaded queue
/// \details Thread queue allows it to be safely used by multiple threads by simple interface.
/// Interface function is pushing, popping with thread block, stop popping and check on empty
template <typename T>
class TQUEUE_EXPORT TQueue {
public:
	TQueue() = default;
	TQueue(const TQueue&) = delete;
	TQueue& operator=(const TQueue&) = delete;

	///
	/// \brief onTakeData - put item type T in queue
	/// \param t - item for containing in queue
	///
	void push(T&& t) {
		std::unique_lock<std::mutex> ul(m);
		q.push(std::move(t));
		cv.notify_one();
	}

	///
	/// \brief pop - blocking popping item from queue
	/// \return return item if exist and std::nullopt if not
	///
	std::optional<T> pop() {
		std::unique_lock<std::mutex> ul(m);

		cv.wait(ul, [&]() {
			auto isStopped{s};
			s = false;
			return !q.empty() || isStopped;
		});

		if (q.empty()) {
			return std::nullopt;
		} else {
			auto value = q.front();
			q.pop();
			return value;
		}
	}

	///
	/// \brief empty - check queue on empty
	/// \return return true if queue is empty, false if not
	///
	bool empty() {
		std::unique_lock<std::mutex> ul(m);

		return q.empty();
	}

	///
	/// \brief stop - stop popping queue from another thread
	///
	void stop() {
		std::unique_lock<std::mutex> ul(m);

		s = true;
		cv.notify_one();
	}

private:
	std::queue<T> q;
	std::mutex m;
	std::condition_variable cv;
	bool s{false};
};
} // namespace utl
